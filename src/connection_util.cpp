#include <string>
#include <iostream>
#include <SFML/Network.hpp>

#include "../inc/util/types.hpp"
#include "../inc/command_parser.hpp"
#include "../inc/connection_util.hpp"

void trim_string(std::string& string) noexcept
{
    for (auto it = string.rbegin(); it != string.rend(); it++) 
        if (auto val = *it; val == '}') 
            break;
        else
            string.pop_back();
}

tl::expected<command::Command, str> receive_from_socket(sf::TcpSocket& socket)
{
    std::array<char, 1128> buffer;
    // FIXME: check if buffer space wasn't sufficient!
    usize n_bytes_received = 0;
    socket.receive(buffer.data(), buffer.size(), n_bytes_received);
    if (n_bytes_received == 1128)
        std::cerr << "receive_from_socket: Probably exceeded buffer size!\n";
    auto as_string = std::string(buffer.data());
    trim_string(as_string);
    std::cout << "Received data: `" << as_string << "`\n";
    auto cmd = command::try_from_json(std::move(as_string));

    return cmd;
}
