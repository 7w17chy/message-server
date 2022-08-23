#pragma once

#include "types.hpp"
#include "../tl/expected.hpp"

#include <vector>
#include <iostream>
#include <optional>

namespace util {

struct Arguments
{
    i32 count;
    std::vector<std::string> args;
};

} // util

tl::expected<i32, str> entry(util::Arguments);
int main(int argc, char* argv[])
{
    std::vector<std::string> args(argc);
    for (int i = 0; i < argc; i++)
        args[i] = argv[i];

    auto result = entry({
        .count = argc,
        .args = args,
    });

    if (!result) {
        std::cerr << "[ERROR] " << result.error() << "\n";
        return -1;
    }

    return result.value();
}
