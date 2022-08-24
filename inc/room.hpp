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
    std::shared_ptr<msd::channel<std::shared_ptr<client::Client>>> new_users;
public:
    Room()
        : participating_clients()
        , client_connections()
        , new_users(std::make_shared<msd::channel<std::shared_ptr<client::Client>>>(100))
    {}

    Room(Room&& moved)
        : participating_clients(std::move(moved.participating_clients))
        , client_connections(std::move(moved.client_connections))
        , new_users(moved.new_users)
    {}

    std::shared_ptr<msd::channel<std::shared_ptr<client::Client>>> get_channel();

    void broadcast(const client::Client&, const std::string&) noexcept;

    void remove_client(const client::Client&) noexcept;

    void on_client_join(std::shared_ptr<client::Client>) noexcept;

    // 1. check for new users who want to join
    // 2. check for upcoming messages and broadcast them if necessary
    std::optional<str> main_loop() noexcept;
};

} // room
