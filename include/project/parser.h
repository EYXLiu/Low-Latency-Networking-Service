#pragma once
#include <optional>
#include <memory>
#include "message.h"
#include "connection.h"

class Parser {
public: 
    static std::optional<std::unique_ptr<Message>> try_parse(Connection& conn);
};
