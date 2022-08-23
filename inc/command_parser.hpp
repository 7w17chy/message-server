#pragma once

#include <vector>
#include <string>

#include "../inc/util/types.hpp"
#include "../inc/tl/expected.hpp"

namespace command 
{

enum class CommandType
{
    JOIN,
    QUIT,
    IDENTIFY,
};

struct Command
{
    CommandType type;
    std::vector<std::string> arguments;
};

tl::expected<Command, str> parse_command(const std::string&);

}; // command
