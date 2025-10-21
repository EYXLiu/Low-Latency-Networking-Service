#pragma once
#include "connection.h"
#include <memory>

class WriteCoalescer {
public:
    static void queue_message(Connection* conn, std::unique_ptr<Message> msg);
    static void flush(Connection* conn);
};
