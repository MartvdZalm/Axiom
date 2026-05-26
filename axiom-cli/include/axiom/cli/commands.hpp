#pragma once

#include <string>

namespace axiom::cli {

enum class CommandKind {
    Parse,
    Ast,
    Generate,
};

struct CommandLine {
    CommandKind command{CommandKind::Parse};
    std::string input_path;
    std::string output_dir{"generated"};
};

[[nodiscard]] int run(const CommandLine& options);

[[nodiscard]] CommandLine parse_args(int argc, char** argv);

} // namespace axiom::cli
