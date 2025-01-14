#include "maelstrom-node/handler.hpp"
#include "maelstrom-node/message.hpp"
#include "maelstrom-node/node.hpp"

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <memory>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <string>

using maelstrom_node::handler;

class echo_handler : public handler
{
public:
    echo_handler() = default;

    void process( maelstrom_node::sender     *sender,
                  maelstrom_node::message_ptr msg ) override
    {
        auto replay = msg->make_replay();
        replay->payload().type( "echo_ok" );
        replay->payload().set_value( "echo",
                                     msg->payload().get_value<std::string>( "echo" ) );

        sender->send( replay );
    }

private:
};

int main()
{
    auto file_logger = spdlog::basic_logger_mt( "file_logger", "maelstrom_log.txt" );
    spdlog::set_default_logger( file_logger );
    spdlog::flush_on( spdlog::level::trace );

    try {
        auto                 handler = std::make_unique<echo_handler>();
        maelstrom_node::node node{ std::move( handler ) };
        node.run();
    } catch( const std::exception &e ) {
        spdlog::error( "main() execution error: {}", e.what() );
    }

    return 0;
}
