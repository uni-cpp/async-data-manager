////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file app/main.cpp
/// @brief Extra calculations of crc sums for data blocks in multiple threads.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <uni/application/Application.hpp>

#include <uni/common/Thread.hpp>

int
main( int /*argc*/, char** /*argv*/ )
{
    uni::common::Thread::set_current_thread_name( "Application" );

    int exit_code{ 0 };

    uni::application::Application app( "Config.json" );
    if( !app.run( ) )
    {
        exit_code = 1;
    }

    return exit_code;
}
