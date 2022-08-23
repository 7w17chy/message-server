#include "../inc/command_parser.hpp"
#include "../inc/json.hpp"

using namespace command;

tl::expected<Command, str> try_from_json(const std::string& raw) noexcept
{
    try {
        nlohmann::json jparse(raw);
        auto parsed = jparse.get<SerializedCommand>();

        // check if the command is valid or not
        if (!known_commands.contains(parsed.command))
            return tl::unexpected<str>("Invalid command!");

        return Command(std::move(parsed));
    } catch (nlohmann::json::exception& ex) {
        return tl::unexpected<str>("Couldn't parse command from json!");
    }
}


Command::Command(SerializedCommand&& sc)
    : arguments(std::move(sc.command_arguments))
{
    // no checking necessary -- that has been done in `try_from_json`
    this->type = known_commands[sc.command];
}
