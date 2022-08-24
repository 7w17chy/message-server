#include "../inc/server.hpp"
#include "../inc/command_parser.hpp"
#include "../inc/client.hpp"
#include "../inc/global_state.hpp"
#include "../inc/util/types.hpp"
#include "../inc/connection_util.hpp"

#include <SFML/Network/Packet.hpp>

#include <array>
#include <iostream>

using namespace server;

tl::expected<i32, str> Server::dispatch_clients() noexcept
{
    sf::TcpListener incoming_connections;
    incoming_connections.listen(8085);
    std::cout << "Listening on port: " << incoming_connections.getLocalPort() << "\n";

    for (auto new_connection = std::make_unique<sf::TcpSocket>() ;; 
         new_connection = std::make_unique<sf::TcpSocket>()) 
    {
        if (auto res = incoming_connections.accept(*new_connection);
            res == sf::Socket::Done) 
        {
            auto cmd = receive_from_socket(*new_connection);

            // client identified with their nickname
            // TODO: basic password checking
            if (cmd->type == command::CommandType::IDENTIFY) {
                auto client = std::make_shared<client::Client>(client::Client { 
                    .nick = std::move(cmd->arguments[0]),
                    .connection = std::move(new_connection), 
                });
                this->server_state.add_client(client);

                // send to welcome room
                std::shared_ptr<room::Room> welcome_room = 
                    *this->server_state.search_room("welcome");
                welcome_room->on_client_join(client);
            } else {
                std::cerr << "Couldn't accept connection: authentication required!\n";
                continue;
            }
        }
    }
}
