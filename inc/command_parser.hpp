#pragma once

#include "../inc/json.hpp"
#include "../inc/util/types.hpp"
#include "../inc/tl/expected.hpp"

#include <vector>

namespace command 
{

struct SerializedCommand 
{
    std::string command;
    std::vector<std::string> command_arguments;
};

enum class CommandType
{
    JOIN,
    QUIT,
    SEND,
    IDENTIFY,
};

static std::map<std::string, CommandType> known_commands = {
    { "join", CommandType::JOIN },
    { "quit", CommandType::QUIT },
    { "send", CommandType::SEND },
    { "ident", CommandType::IDENTIFY },
};

struct Command
{
    CommandType type;
    std::vector<std::string> arguments;

    explicit Command(SerializedCommand&&);
};

tl::expected<Command, str> try_from_json(const std::string& raw) noexcept;

} // command
