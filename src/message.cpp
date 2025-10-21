#include "project/message.h"

std::vector<uint8_t> Message::serialize() const {
    std::vector<uint8_t> out;

    uint16_t len = static_cast<uint16_t>(1 + 4 + payload.size());
    out.push_back((len >> 8) & 0xFF);
    out.push_back(len & 0xFF);

    out.push_back(type);

    out.push_back((room_id >> 24) & 0xFF);
    out.push_back((room_id >> 16) & 0xFF);
    out.push_back((room_id >> 8) & 0xFF);
    out.push_back(room_id & 0xFF);

    out.insert(out.end(), payload.begin(), payload.end());

    return out;
}

std::unique_ptr<Message> Message::deserialize(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 7) return nullptr;

    uint16_t len = (buffer[0] << 8) | buffer[1];
    if (buffer.size() < 2 + len) return nullptr;

    auto msg = std::make_unique<Message>();
    msg->length = len;
    msg->type = buffer[2];
    msg->room_id = (buffer[3] << 24) | (buffer[4] << 16) | (buffer[5] << 8) | buffer[6];
    msg->payload = std::string(buffer.begin() + 7, buffer.begin() + 2 + len);

    return msg;
}

std::unique_ptr<Message> Message::clone() const {
    return std::make_unique<Message>(*this);
}