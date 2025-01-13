#pragma once

#include "echo-cpp/message.hpp"

#include <memory>

namespace maelstrom_node
{

class handler
{
public:
    handler()                             = default;
    virtual ~handler()                    = default;
    handler( const handler & )            = default;
    handler( handler && )                 = default;
    handler &operator=( const handler & ) = default;
    handler &operator=( handler && )      = default;

    virtual void process( sender *sender, message_ptr msg ) = 0;
};

using handler_ptr = std::unique_ptr<handler>;

} // namespace maelstrom_node
