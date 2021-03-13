////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/Config.cpp
/// @brief Definition Config class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <uni/application/Config.hpp>
#include <uni/common/Log.hpp>

#include <fstream>

namespace uni
{
namespace application
{
Config::Config( const std::string& path_to_config )
{
    LOG_TRACE_MSG( "" );

    // Add error handling for ifstream and json parsing

    std::ifstream config_stream( path_to_config );

    const nlohmann::json::parser_callback_t CALLBACK{ nullptr };
    const bool DISALLOW_EXCEPTIONS{ false };
    const bool COMMENTS_IGNORED{ true };
    const auto jf = nlohmann::json::parse( config_stream, CALLBACK, DISALLOW_EXCEPTIONS, COMMENTS_IGNORED );

    if( !jf.is_object( ) )
    {
        LOG_ERROR_MSG( "Error happened while config was parsing. Use default values." );
        return;
    }

    generators_count = jf[ "Generators" ];
    blocks_count = jf[ "Blocks" ];
    block_size_bytes = jf[ "Block_size_bytes" ];
    hashers_count = jf[ "Hashers" ];

    LOG_INFO_MSG( generators_count, blocks_count, block_size_bytes, hashers_count );
}

}  // namespace application
}  // namespace uni
