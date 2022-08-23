#include "../inc/server.hpp"
#include "../inc/command_parser.hpp"
#include "../inc/client.hpp"
#include "../inc/global_state.hpp"
#include "../inc/global_state.hpp"

#include "../inc/util/types.hpp"

#include <SFML/Network/Packet.hpp>

#include <array>
#include <iostream>

using namespace server;

tl::expected<i32, str> Server::dispatch_clients() noexcept
{
    sf::TcpListener incoming_connections;
    incoming_connections.listen(8080);
    std::cout << "Listening on port: " << incoming_connections.getLocalPort() << "\n";

    for (auto new_connection = std::make_unique<sf::TcpSocket>() ;; 
         new_connection = std::make_unique<sf::TcpSocket>()) 
    {
        if (auto res = incoming_connections.accept(*new_connection);
            res == sf::Socket::Done) 
        {
            // client connection process
            std::array<char, 1024> buffer;
            usize n_bytes_received = 0;
            new_connection->receive(buffer.data(), buffer.size(), n_bytes_received);
            auto as_string = std::string(buffer.data());
            auto cmd = command::parse_command(std::move(as_string));
            // FIXME: client should receive an error message!
            if (!cmd) {
                std::cerr << "Couldn't accept connection: invalid command!\n";
                continue;
            }

            // client identified with their nickname
            // TODO: basic password checking
            if (cmd->type == command::CommandType::IDENTIFY) {
                auto client = std::make_shared<client::Client>(client::Client { 
                    .nick = std::move(cmd->arguments[0]),
                    .connection = std::move(new_connection), 
                });
                this->server_state.add_client(client);
                // TODO: send client to welcome room
            } else {
                std::cerr << "Couldn't accept connection: authentication required!\n";
                continue;
            }
        }
    }
}
