#pragma once

#include <list>
#include <map>
#include <memory>
#include <mutex>

#include <SFML/Network.hpp>

#include "../inc/client.hpp"
#include "../inc/room.hpp"
#include "../inc/global_state.hpp"
#include "../inc/msd/channel.hpp"
#include "../inc/util/types.hpp"
#include "../inc/tl/expected.hpp"

namespace server {

// FIXME: refactor to use the factory pattern
class Server
{
private:
    state::GlobalServerState& server_state;
public:
    explicit Server(state::GlobalServerState& gs)
        : server_state(gs)
    {}

    tl::expected<i32, str> dispatch_clients() noexcept;
};

} // server
