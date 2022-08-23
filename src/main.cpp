#include <iostream>

#include <SFML/Network.hpp>

#include "../inc/util/types.hpp"
#include "../inc/util/mainpp.hpp"
#include "../inc/tl/expected.hpp"

#include "../inc/server.hpp"
#include "../inc/global_state.hpp"

static state::GlobalServerState server_state;

tl::expected<i32, str> entry(util::Arguments)
{
    auto welcome_room = std::make_shared<room::Room>();
    server::Server server(welcome_room, server_state);
    return server.dispatch_clients();
}
