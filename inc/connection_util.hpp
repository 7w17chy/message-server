#pragma once

#include <string>
#include <iostream>
#include <SFML/Network.hpp>

#include "../inc/util/types.hpp"
#include "../inc/command_parser.hpp"

// trim garbage from the end of the string
void trim_string(std::string& string) noexcept;

tl::expected<command::Command, str> receive_from_socket(sf::TcpSocket& socket);
