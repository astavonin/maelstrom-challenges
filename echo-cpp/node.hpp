#pragma once

#include "echo-cpp/handler.hpp"
#include "echo-cpp/message.hpp"

#include <boost/asio.hpp>
#include <string>

namespace maelstrom_node
{
namespace asio = boost::asio;

class node : public sender
{
public:
    explicit node( handler_ptr handler );

    void run();

    void send( message_ptr message ) override;

protected:
    void handle_read( const boost::system::error_code &error,
                      std::size_t                      bytes_transferred );
    void signal_handler( const boost::system::error_code &error, int signal_number );

private:
    using stream_ptr = std::unique_ptr<asio::posix::stream_descriptor>;

    std::string node_id_;

    handler_ptr handler_;

    asio::io_context       ioc_;
    stream_ptr             input_stream_;
    stream_ptr             output_stream_;
    boost::asio::streambuf buffer_;
};

} // namespace maelstrom_node
