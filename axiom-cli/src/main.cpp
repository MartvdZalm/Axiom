#include <axiom/cli/commands.hpp>

int main(int argc, char** argv)
{
    const auto options = axiom::cli::parse_args(argc, argv);
    return axiom::cli::run(options);
}
