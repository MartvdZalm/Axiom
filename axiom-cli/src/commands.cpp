#include <axiom/ast/printer.hpp>
#include <axiom/cli/commands.hpp>
#include <axiom/cli/pipeline.hpp>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fstream>
#include <iostream>

namespace axiom::cli
{

namespace
{

void print_usage()
{
    fmt::print("axiom — Axiom state-machine compiler\n"
               "\n"
               "Usage:\n"
               "  axiom parse <file>       Parse and report success or diagnostics\n"
               "  axiom ast <file>         Print AST\n"
               "  axiom generate <file>    Generate C++ into output directory\n"
               "\n"
               "Options (generate):\n"
               "  -o <dir>                  Output directory (default: generated)\n\n");
}

} // namespace

CommandLine parse_args(int argc, char** argv)
{
    CommandLine line{};
    if (argc < 3)
    {
        return line;
    }

    const std::string verb = argv[1];
    if (verb == "parse")
    {
        line.command = CommandKind::Parse;
    }
    else if (verb == "ast")
    {
        line.command = CommandKind::Ast;
    }
    else if (verb == "generate")
    {
        line.command = CommandKind::Generate;
    }
    else
    {
        return line;
    }

    if (line.command == CommandKind::Generate)
    {
        for (int i = 2; i < argc; ++i)
        {
            const std::string arg = argv[i];
            if (arg == "-o")
            {
                if (i + 1 >= argc)
                {
                    line.input_path.clear();
                    return line;
                }
                line.output_dir = argv[++i];
                continue;
            }
            if (line.input_path.empty())
            {
                line.input_path = arg;
            }
        }
        return line;
    }

    line.input_path = argv[2];
    return line;
}

int run(const CommandLine& options)
{
    if (options.input_path.empty())
    {
        print_usage();
        return 1;
    }

    core::SourceManager sources;
    Pipeline pipeline(sources);

    switch (options.command)
    {
    case CommandKind::Parse:
    {
        const auto result = pipeline.parse_file(options.input_path);
        if (!result.succeeded())
        {
            std::cerr << result.diagnostics().format(sources);
            std::cerr << pipeline.diagnostics().format(sources);
            return 1;
        }
        fmt::print("parse succeeded: {}\n", options.input_path);
        return 0;
    }
    case CommandKind::Ast:
    {
        const auto result = pipeline.parse_file(options.input_path);
        if (!result.succeeded())
        {
            std::cerr << result.diagnostics().format(sources);
            return 1;
        }
        ast::AstPrinter printer{};
        fmt::print("{}", printer.print(result.value()));
        return 0;
    }
    case CommandKind::Generate:
    {
        const auto result = pipeline.compile({options.input_path});
        if (!result.succeeded())
        {
            std::cerr << pipeline.diagnostics().format(sources);
            return 1;
        }

        std::error_code ec;
        std::filesystem::create_directories(options.output_dir, ec);

        for (const auto& file : result.value().generated.files)
        {
            const auto out_path = std::filesystem::path(options.output_dir) / file.path;
            std::ofstream out(out_path);
            if (!out)
            {
                std::cerr << "failed to write " << out_path.string() << "\n";
                return 1;
            }
            out << file.contents;
            fmt::print("wrote {}\n", out_path.string());
        }
        return 0;
    }
    }

    print_usage();
    return 1;
}

} // namespace axiom::cli
