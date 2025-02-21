#include "utils/path.hpp"

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

namespace utils
{

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

} // namespace utils
