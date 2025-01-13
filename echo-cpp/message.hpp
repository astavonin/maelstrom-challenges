#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

namespace maelstrom_node
{

class message;
using message_ptr = std::shared_ptr<message>;

class message
{
public:
    class payload
    {
    public:
        payload() = default;

        template <typename T>
        [[nodiscard]] auto get_value( const std::string_view field ) const
        {
            return data_.value( field, T{} );
        }

        template <typename T>
        void set_value( const std::string_view field, T value )
        {
            data_[field] = value;
        }

        [[nodiscard]] std::string type() const;
        void                      type( const std::string &type );
        [[nodiscard]] std::string node_id() const;
        [[nodiscard]] std::string node_ids() const;
        [[nodiscard]] int         msg_id() const;

    private:
        friend class message;

        void data( nlohmann::json &data ) { data_ = data; }

        nlohmann::json &data() { return data_; }

        nlohmann::json data_;
    };

    message();
    explicit message( const std::string &data );
    ~message()                            = default;
    message( const message &msg )         = default;
    message( message && )                 = delete;
    message &operator=( const message & ) = default;
    message &operator=( message && )      = delete;

    [[nodiscard]] std::string as_string();

    std::string source() const { return data_.value( "src", "" ); }

    void source( const std::string_view &src ) { data_["src"] = src; }

    std::string destination() const { return data_.value( "dest", "" ); }

    void destination( const std::string_view &dst ) { data_["dest"] = dst; }

    payload &payload() { return payload_; }

    [[nodiscard]] message_ptr make_replay() const;

protected:
    void init( const std::string &data );

private:
    nlohmann::json data_;

    class payload payload_;
};

class sender
{
public:
    sender()                            = default;
    virtual ~sender()                   = default;
    sender( sender && )                 = default;
    sender( const sender & )            = default;
    sender &operator=( sender && )      = default;
    sender &operator=( const sender & ) = default;

    virtual void send( message_ptr message ) = 0;
};

} // namespace maelstrom_node
