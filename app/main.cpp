////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file app/main.cpp
/// @brief Redundant calculations of crc sums for data blocks in multiple threads.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <uni/application/Application.hpp>

#include <sysexits.h>
#include <cstdint>
#include <uni/common/Thread.hpp>

int
main( int /*argc*/, char** /*argv*/ )
{
    uni::common::Thread::set_current_thread_name( "Application" );

    int exit_code{ EX_OK };

    uni::application::Application app( "Config.json" );
    if( !app.run( ) )
    {
        exit_code = EX_SOFTWARE;
    }

    return exit_code;
}
