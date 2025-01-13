#include "echo-cpp/node.hpp"

#include "echo-cpp/message.hpp"

#include <boost/algorithm/string.hpp>
#include <ctime>
#include <exception>
#include <memory>
#include <spdlog/spdlog.h>

namespace maelstrom_node
{

static constexpr size_t BUFF_SIZE = 1024;

node::node( handler_ptr handler )
    : handler_( std::move( handler ) )
{
}

void node::signal_handler( const boost::system::error_code &error,
                           int /*signal_number*/ )
{
    if( !error ) {
        ioc_.stop();
    } else {
        spdlog::error( "Error in signal handler: ", error.message() );
    }
}

void node::run()
{
    input_stream_ = std::make_unique<asio::posix::stream_descriptor>(
        ioc_, ::dup( STDIN_FILENO ) );
    output_stream_ = std::make_unique<asio::posix::stream_descriptor>(
        ioc_, ::dup( STDOUT_FILENO ) );

    input_stream_->async_read_some(
        buffer_.prepare( BUFF_SIZE ),
        [this]( const auto &error, auto bytes_transferred ) {
            this->handle_read( error, bytes_transferred );
        } );

    boost::asio::signal_set signals( ioc_, SIGINT, SIGTERM );

    signals.async_wait( [this]( const auto &error, auto signal_number ) {
        this->signal_handler( error, signal_number );
    } );

    ioc_.run();
}

void node::handle_read( const boost::system::error_code &error,
                        std::size_t                      bytes_transferred )
{
    if( error ) {
        spdlog::error( "handle_read failed with: {}", error.message() );
        return;
    }

    buffer_.commit( bytes_transferred );
    std::string tmp{ asio::buffers_begin( buffer_.data() ),
                     asio::buffers_end( buffer_.data() ) };
    buffer_.consume( bytes_transferred );

    message_ptr msg;
    try {
        msg = std::make_shared<message>( tmp );
        spdlog::info( "node({}) message content: {}", node_id_, msg->as_string() );
        if( msg->payload().type() == "init" ) {
            node_id_     = msg->payload().node_id();
            auto init_ok = msg->make_replay();
            init_ok->payload().type( "init_ok" );
            this->send( init_ok );
        } else {
            handler_->process( this, msg );
        }
    } catch( std::exception &ex ) {
        spdlog::error( "node({}) processing error: {}", node_id_, ex.what() );
    }
    input_stream_->async_read_some(
        buffer_.prepare( BUFF_SIZE ),
        [this]( const auto &error, auto bytes_transferred ) {
            this->handle_read( error, bytes_transferred );
        } );
}

void node::send( message_ptr message )
{
    auto msg_string = message->as_string();
    spdlog::info( "node({}) sent: {}", node_id_, msg_string );
    msg_string
        += "\n"; // we shall have this '\n' at the end of any message we sent back.
    asio::async_write( *output_stream_, asio::buffer( msg_string ),
                       []( const boost::system::error_code &error, std::size_t ) {
                           if( error ) {
                               spdlog::error( "unable to write: {}", error.message() );
                           }
                       } );
}

} // namespace maelstrom_node
