#include "project/parser.h"
#include <iostream>

std::optional<std::unique_ptr<Message>> Parser::try_parse(Connection& conn) {
    auto& buf = conn.get_read_buffer();
    if (buf.size() < 7) return std::nullopt;

    uint16_t len = (buf[0] << 8) | buf[1];
    if (buf.size() < 7 + len) return std::nullopt;

    auto msg = std::make_unique<Message>();
    msg->length = len;
    msg->type = buf[2];
    msg->room_id = (buf[3] << 24) | (buf[4] << 16) | (buf[5] << 8) | buf[6];
    msg->payload = std::string(buf.begin() + 7, buf.begin() + 7 + len);

    buf.erase(buf.begin(), buf.begin() + 7 + len);
    return msg;
}
