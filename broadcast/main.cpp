#include "maelstrom-node/handler.hpp"
#include "maelstrom-node/message.hpp"
#include "maelstrom-node/node.hpp"

#include <filesystem>
#include <fmt/ranges.h>
#include <memory>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <string_view>
#include <unordered_set>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

namespace mf = maelstrom_node::msg_field;

std::filesystem::path get_executable_path()
{
#ifdef __APPLE__
    uint32_t size = 0;
    // First call to determine the required buffer size
    _NSGetExecutablePath( nullptr, &size );
    std::vector<char> buffer( size );
    if( _NSGetExecutablePath( buffer.data(), &size ) != 0 ) {
        throw std::runtime_error(
            "Unable to get executable path using _NSGetExecutablePath" );
    }
    // _NSGetExecutablePath might return a relative path,
    // so we canonicalize it to get the absolute path.
    return std::filesystem::canonical( buffer.data() ).parent_path();
#elif defined( __linux__ )
    // On Linux, /proc/self/exe is a symlink to the executable.
    return std::filesystem::read_symlink( "/proc/self/exe" ).parent_path();
#else
    throw std::runtime_error( "get_executable_path not implemented on this platform" );
#endif
}

using maelstrom_node::handler;

constexpr uint64_t hash( std::string_view str )
{
    uint64_t hash = 0;
#pragma unroll
    for( char chr : str ) {
        hash = ( hash * 131 ) + chr;
    }
    return hash;
}

constexpr uint64_t operator"" _hash( const char *str, size_t len )
{
    return hash( std::string_view( str, len ) );
}

class broadcast_handler : public handler
{
public:
    broadcast_handler() = default;

    void process( maelstrom_node::sender     *sender,
                  maelstrom_node::message_ptr msg ) override
    {
        switch( hash( msg->get_value<mf::data_type>() ) ) {
        case "topology"_hash: {
            auto topo = msg->get_value<mf::topology>();
            spdlog::info( "\"topology\" message: {}", topo );
            neighbors_ = topo[sender->node_id()];
            spdlog::info( "neighbors: {}", neighbors_ );

            auto repl = msg->make_reply();
            repl->set_value<mf::data_type>( "topology_ok" );
            sender->send( repl );
            break;
        }
        case "broadcast"_hash: {
            int message = msg->get_value_raw<int>( "message" );
            spdlog::info( "recieved \"broadcast\": {}", message );
            auto repl = msg->make_reply();

            if( messages_.find( message ) == messages_.end() ) {
                messages_.insert( message );
                auto from = msg->get_value<maelstrom_node::msg_field::source>();

                msg->set_value<maelstrom_node::msg_field::source>(
                    msg->get_value<maelstrom_node::msg_field::destination>() );
                for( auto neighbor : neighbors_ ) {
                    msg->set_value<maelstrom_node::msg_field::destination>( neighbor );

                    sender->send( msg );
                }
            }

            sender->send( repl );
            break;
        }
        case "read"_hash: {
            spdlog::info( "recieved \"read\"" );
            auto repl = msg->make_reply();
            repl->set_value_raw<std::unordered_set<int>>( "messages", messages_ );
            sender->send( repl );
            break;
        }
        default:
            spdlog::error( "unexpected message: {}", msg->as_string() );
            break;
        }
    }

private:
    std::vector<std::string> neighbors_;
    std::unordered_set<int>  messages_;
};

int main()
{
    auto file_logger = spdlog::basic_logger_mt(
        "file_logger", get_executable_path() / "broadcast_log.txt" );
    spdlog::set_default_logger( file_logger );
    spdlog::flush_on( spdlog::level::trace );
    spdlog::set_pattern( "[%L][%T.%f][%P/%t] %v" );

    try {
        auto                 handler = std::make_unique<broadcast_handler>();
        maelstrom_node::node node{ std::move( handler ) };
        node.run();
    } catch( const std::exception &e ) {
        spdlog::error( "main() execution error: {}", e.what() );
    }

    return 0;
}
