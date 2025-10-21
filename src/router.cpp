#include "project/router.h"
#include "project/write_coalescer.h"

void Router::join_room(Connection* conn, uint32_t room_id) {
    std::lock_guard<std::mutex> lock(mtx_);
    room_map_[room_id].push_back(conn);
}

void Router::leave_room(Connection* conn, uint32_t room_id) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto& vec = room_map_[room_id];
    vec.erase(std::remove(vec.begin(), vec.end(), conn), vec.end());
}

void Router::broadcast(uint32_t room_id, std::unique_ptr<Message> msg) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = room_map_.find(room_id);
    if (it == room_map_.end()) return;

    for (auto* conn : it->second) {
        auto msg_copy = std::make_unique<Message>(*msg);
        WriteCoalescer::queue_message(conn, std::move(msg_copy));
    }
}
