#include "../inc/command_parser.hpp"
#include "../inc/connection_util.hpp"
#include "../inc/json.hpp"
#include <iostream>


namespace command 
{

tl::expected<Command, str> try_from_json(const std::string& raw) noexcept
{
    try {
        std::cout << "Reached try_from_json!\n";
        auto parsed_json = nlohmann::json::parse(raw);
        SerializedCommand parsed = {
            .command = parsed_json["cmd"],
            .command_arguments = parsed_json["args"]
        };
        std::cout << parsed.command << ", " << parsed.command_arguments[0] << "\n";

        // check if the command is valid or not
        if (!known_commands.contains(parsed.command))
            return tl::unexpected<str>("Invalid command!");

        return Command(std::move(parsed));
    } catch (nlohmann::json::exception& ex) {
        std::cout << ex.what() << "\n";
        return tl::unexpected<str>("Couldn't parse command from json!");
    }
}

Command::Command(SerializedCommand&& sc)
    : arguments(std::move(sc.command_arguments)) 
{
    // no checking necessary -- that has been done in `try_from_json`
    this->type = known_commands[sc.command];
}

} // command
