#include "../inc/room.hpp"

using namespace room;

std::shared_ptr<msd::channel<client::Client>> Room::get_channel()
{
    return this->new_users;
}
