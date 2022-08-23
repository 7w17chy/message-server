#pragma once

#include <map>
#include <list>
#include <memory>
#include <mutex>
#include <optional>
#include <SFML/Network.hpp>

#include "client.hpp"

#include "../inc/util/types.hpp"
#include "../inc/msd/channel.hpp"

namespace room {

class Room
{
private:
    std::map<std::string, std::shared_ptr<client::Client>> participating_clients;
    sf::SocketSelector client_connections;
    // users who want to join the channel
    std::shared_ptr<msd::channel<client::Client>> new_users;
private:
    void client_add(std::shared_ptr<client::Client> client);
public:
    Room()
        : participating_clients()
        , client_connections()
        , new_users(std::make_shared<msd::channel<client::Client>>(100))
    {}

    Room(Room&& moved)
        : participating_clients(std::move(moved.participating_clients))
        , client_connections(std::move(moved.client_connections))
        , new_users(moved.new_users)
    {}

    std::shared_ptr<msd::channel<client::Client>> get_channel();

    std::optional<str> broadcast(const client::Client& author);

    // 1. check for new users who want to join
    // 2. check for upcoming messages and broadcast them if necessary
    std::optional<str> main_loop() noexcept;
};

} // room
