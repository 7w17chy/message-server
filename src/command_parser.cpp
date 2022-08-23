#include "../inc/command_parser.hpp"

#include <optional>
#include <string>
#include <vector>
#include <utility>
#include <map>

namespace command 
{

using namespace std;
using namespace tl;

expected<vector<string>, str> split_at(const string& string, char predicate) noexcept
{
    usize start = 0;
    usize end = 0;
    vector<std::string> result;
    result.reserve(10); // capacity != size

    while (end != string.length()) {
        if (string[end] == predicate) {
            // skip double whitespace
            if (start == end) {
                end += 1;
                continue;
            }

            auto substr = string.substr(start, end);
            result.push_back(move(substr));

            start = end + 1;
            end += 2;
            continue;
        }

        end += 1;
    }

    if (result.empty()) 
        return tl::unexpected<str>("Invalid command/parameter list!");
    else 
        return result;
}

expected<pair<CommandType, vector<string>>, str> parse_command_type(const expected<vector<string>, str>& raw) noexcept
{
    map<string, CommandType> command_types = {
        { "JOIN",  CommandType::JOIN },
        { "QUIT",  CommandType::QUIT },
        { "IDENT", CommandType::IDENTIFY }
    };

    return raw.and_then([&command_types](vector<string> args) -> expected<pair<CommandType, vector<string>>, str> { 
        if (!command_types.contains(args[0]))
            return tl::unexpected<str>("Not a valid command!");

        auto type = command_types[args[0]];

        return pair<CommandType, vector<string>>(type, move(args));
    });
}

// TODO check if arguments match the command type
expected<pair<CommandType, vector<string>>, str> parse_command_arguments(const expected<pair<CommandType, vector<string>>, str>& raw) noexcept
{
    return raw;
}

expected<Command, str> construct_command(const expected<pair<CommandType, vector<string>>, str>& cmd) noexcept
{
    return cmd.and_then([](pair<CommandType, vector<string>> c) -> expected<Command, str> {
        auto [command, arguments] = c;
        return Command { command, arguments };
    });
}

tl::expected<Command, str> parse_command(const string& raw)
{
    return split_at(raw, ' ')
           .and_then(&parse_command_type)
           .and_then(&parse_command_arguments)
           .and_then(&construct_command);
}

} // command
