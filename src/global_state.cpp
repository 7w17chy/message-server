#include "../inc/global_state.hpp"
#include "../inc/tl/expected.hpp"

using namespace state;
using GSS =  GlobalServerState;

void GlobalServerState::create_room(const std::string& name) 
{
    // insert into room map
    auto [lock, rooms] = GlobalServerState::MutexLock<RoomMap>::lock_blocking(this->active_rooms);
    auto room = std::make_shared<room::Room>();
    rooms[name] = room;

    // get a hold onto the rooms channel
    auto [lock2, corridor] =
        GlobalServerState::MutexLock<Corridor>::lock_blocking(this->corridor);
    corridor[name] = rooms[name]->get_channel();
}

void GlobalServerState::add_room(std::shared_ptr<room::Room> new_room, const std::string& name)
{
    auto [lock, rooms] = GlobalServerState::MutexLock<RoomMap>::lock_blocking(this->active_rooms);
    rooms[name] = new_room;

    // get a hold onto the rooms channel
    auto [lock2, corridor] =
        GlobalServerState::MutexLock<Corridor>::lock_blocking(this->corridor);
    corridor[name] = rooms[name]->get_channel();
}

void GlobalServerState::destroy_room(const std::string& name)
{
    auto [lock, rooms] = 
        GlobalServerState::MutexLock<RoomMap>::lock_blocking(this->active_rooms);
    rooms.erase(name);
}

void GlobalServerState::add_client(std::shared_ptr<client::Client> client) 
{
    auto [lock, clients] = 
        GlobalServerState::MutexLock<ClientMap>::lock_blocking(this->connected_clients);
    clients[client->nick] = client;
}

void GlobalServerState::remove_client(const client::Client& client)
{
    auto [lock, clients] = 
        GlobalServerState::MutexLock<ClientMap>::lock_blocking(this->connected_clients);
    clients.erase(client.nick);
}

std::optional<std::shared_ptr<room::Room>> GlobalServerState::search_room(const std::string& name)
{
    auto [lock, rooms] = 
        GlobalServerState::MutexLock<RoomMap>::lock_blocking(this->active_rooms);
    if (rooms.contains(name))
        return rooms[name];
    return std::nullopt;
}
