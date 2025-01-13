#include "message.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <format>
#include <spdlog/spdlog.h>
#include <stdexcept>

using json = nlohmann::json;

namespace maelstrom_node
{

message::message()
{
    data_["body"] = nullptr;
}

message::message( const std::string &data )
{
    init( data );
}

void message::init( const std::string &data )
{
    try {
        data_ = json::parse( data );

        auto fields = std::vector{ "body", "src", "dest" };

        if( !std::ranges::all_of( fields, [this]( const auto &field ) {
                return data_.contains( field );
            } ) ) {
            throw std::logic_error( std::format(
                "invalid object \"{}\", one of {} is missing", data, fields ) );
        }
        payload_.data( data_.at( "body" ) );
    } catch( json::exception &ex ) {
        throw std::logic_error(
            std::format( "unable to parce: \"{}\", error: {}", data, ex.what() ) );
    }
}

[[nodiscard]] std::string message::as_string()
{
    data_.at( "body" ) = payload_.data();
    auto msg_string    = data_.dump();
    boost::trim_left( msg_string );
    boost::trim_right( msg_string );
    return msg_string;
}

[[nodiscard]] message_ptr message::make_replay() const
{
    auto replay = std::make_shared<message>();

    replay->data_["src"]                  = data_.at( "dest" );
    replay->data_["dest"]                 = data_.at( "src" );
    replay->payload_.data_["in_reply_to"] = payload_.data_.at( "msg_id" );

    return replay;
}

[[nodiscard]] std::string message::payload::type() const
{
    return data_.value( "type", "" );
}

void message::payload::type( const std::string &type )
{
    data_["type"] = type;
}

[[nodiscard]] std::string message::payload::node_id() const
{
    return get_value<std::string>( "node_id" );
}

[[nodiscard]] std::string message::payload::node_ids() const
{
    throw std::logic_error( "not implemented yet" );
    // return data_.value( "node_id", "" );
};

[[nodiscard]] int message::payload::msg_id() const
{
    return get_value<int>( "msg_id" );
};

} // namespace maelstrom_node
