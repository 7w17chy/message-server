#pragma once

#include <string>
#include <memory>
#include <SFML/Network.hpp>

namespace client {

struct Client 
{
    std::string nick;
    std::unique_ptr<sf::Socket> connection;
};

} // client
