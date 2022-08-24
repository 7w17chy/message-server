#include "../inc/room.hpp"
#include "../inc/command_parser.hpp"
#include "../inc/connection_util.hpp"

#include <iostream>
#include <optional>

using namespace room;

std::shared_ptr<msd::channel<std::shared_ptr<client::Client>>> Room::get_channel()
{
    return this->new_users;
}

void Room::on_client_join(std::shared_ptr<client::Client> new_client) noexcept
{
    this->client_connections.add(*new_client->connection);
    this->participating_clients[new_client->nick] = new_client;
}

void Room::broadcast(const client::Client& author, const std::string& message) noexcept
{
    for (auto& [_, client] : this->participating_clients) {
        // don't send the message to its author
        if (client->nick == author.nick)
            continue;

        client->connection->send(message.data(), message.size());
    }
}

void Room::remove_client(const client::Client& to_remove) noexcept
{
    // since the client authored the command, we can be sure they're in the map
    this->participating_clients.erase(to_remove.nick);
}

// 1. check for new users who want to join
// 2. check for upcoming messages and broadcast them if necessary
std::optional<str> Room::main_loop() noexcept
{
    for (;;) {
        for (auto new_user : *this->new_users)
            this->on_client_join(new_user);

        if (client_connections.wait()) {
            for (auto& [_, client] : this->participating_clients) {
                // FIXME: copied from `server.cpp` -> extract into util function
                if (this->client_connections.isReady(*client->connection)) {
                    auto cmd = receive_from_socket(*client->connection);
                    switch (cmd->type) {
                    case command::CommandType::SEND:
                        this->broadcast(*client, cmd->arguments[0]);
                        break;
                    case command::CommandType::QUIT:
                        this->remove_client(*client);
                        break;
                    case command::CommandType::JOIN:
                        // TODO
                        break;
                    case command::CommandType::IDENTIFY:
                        // error handling
                        break;
                    }
                }
            }
        }
    }

    return std::nullopt;
}
