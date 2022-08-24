#pragma once

#include <utility>
#include <map>
#include <string>
#include <mutex>
#include <memory>
#include <optional>

#include "../inc/client.hpp"
#include "../inc/room.hpp"

namespace state 
{

// FIXME: use singleton pattern!
class GlobalServerState final
{
private:
    template<class T>
    class MutexLock
    {
    private:
        std::mutex& mutex;
    public:
        MutexLock(std::mutex& m)
            : mutex(m)
        {}

        static std::optional<std::pair<MutexLock<T>, T&>> try_lock(std::pair<std::mutex, T>& pair)
        {
            auto& [mutex, val] = pair;
            if (!mutex.try_lock())
                return std::nullopt;
            return { MutexLock(mutex), val };
        }

        static std::pair<MutexLock<T>, T&> lock_blocking(std::pair<std::mutex, T>& pair)
        {
            auto& [mutex, val] = pair;
            mutex.lock();
            return { MutexLock(mutex), val };
        }

        ~MutexLock() { mutex.unlock(); }
    };

    using ClientMap = std::map<std::string, std::shared_ptr<client::Client>>;
    using RoomMap = std::map<std::string, std::shared_ptr<room::Room>>;
    using Corridor = std::map<std::string, std::shared_ptr<msd::channel<std::shared_ptr<client::Client>>>>;
    using Mutex = std::mutex;

    std::pair<Mutex, ClientMap> connected_clients;
    std::pair<Mutex, RoomMap> active_rooms;
    std::pair<Mutex, Corridor> corridor;
public:
    void create_room(const std::string& name);
    void destroy_room(const std::string& name);
    void add_client(std::shared_ptr<client::Client> client);
    void remove_client(const client::Client& client);
    std::optional<std::shared_ptr<room::Room>> search_room(const std::string& name);
};

} // state
