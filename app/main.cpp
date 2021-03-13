////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file app/main.cpp
/// @brief Extra calculations of hash sums for data blocks in multiple threads. uni::Thread approach
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2020-2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <uni/application/Application.hpp>

auto
main( int /*argc*/, char* * /*argv*/ ) -> int
{
    int exit_code{ 0 };

    uni::application::Application app( "Config.json" );
    if( !app.run( ) )
    {
        exit_code = 1;
    }

    return exit_code;
}
