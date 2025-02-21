#pragma once

#include <string_view>

namespace utils
{

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

} // namespace utils
