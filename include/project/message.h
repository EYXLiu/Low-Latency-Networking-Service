#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

struct Message {
    uint16_t length;
    uint8_t type;
    uint32_t room_id;
    std::string payload;

    std::vector<uint8_t> serialize() const;
    static std::unique_ptr<Message> deserialize(const std::vector<uint8_t>& buffer);
    std::unique_ptr<Message> clone() const;
};