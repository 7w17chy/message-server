#pragma once

#include <string>
#include <SFML/Network.hpp>

#include "../inc/util/types.hpp"
#include "../inc/command_parser.hpp"

namespace util
{

void trim_string(std::string& string) noexcept;

template<usize buffer_size>
tl::expected<command::Command, str> receive_from_socket(sf::TcpSocket& socket);

} // util
