#include "project/write_coalescer.h"
#include <iostream>

void WriteCoalescer::queue_message(Connection* conn, std::unique_ptr<Message> msg) {
    conn->send_message(std::move(msg));

}

void WriteCoalescer::flush(Connection* conn) {
    conn->flush_write_buffer();
}
