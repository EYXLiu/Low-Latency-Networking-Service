#pragma once
#include <unordered_map>
#include <vector>
#include <mutex>
#include "connection.h"
#include "message.h"

class Router {
public:
    void join_room(Connection* conn, uint32_t room_id);
    void leave_room(Connection* conn, uint32_t room_id);
    void broadcast(uint32_t room_id, std::unique_ptr<Message> msg);

private:
    std::unordered_map<uint32_t, std::vector<Connection*>> room_map_;
        std::mutex mtx_;
};
