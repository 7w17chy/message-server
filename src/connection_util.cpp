#include "../inc/command_parser.hpp"
#include "../inc/tl/expected.hpp"
#include "../inc/util/types.hpp"
#include "../inc/connection_util.hpp"

#include <SFML/Network.hpp>

#include <string>
#include <array>
#include <optional>
#include <iostream>

namespace util
{

// trim garbage from the end of the string
void trim_string(std::string& string) noexcept
{
    for (auto it = string.rbegin(); it != string.rend(); it++) 
        if (auto val = *it; val == '}') 
            break;
        else
            string.pop_back();
}

template<usize buffer_size>
tl::expected<command::Command, str> receive_from_socket(sf::TcpSocket& socket)
{
    std::array<char, buffer_size> buffer;
    // FIXME: check if buffer space wasn't sufficient!
    usize n_bytes_received = 0;
    socket.receive(buffer.data(), buffer.size(), n_bytes_received);
    if (n_bytes_received == buffer_size)
        std::cerr << "receive_from_socket: Probably exceeded buffer size!\n";
    auto as_string = std::string(buffer.data());
    trim_string(as_string);
    std::cout << "Received data: `" << as_string << "`\n";
    auto cmd = command::try_from_json(std::move(as_string));

    return cmd;
}

} // util
