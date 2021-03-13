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
    // Add error handling for ifstream and json parsing

    std::ifstream config_stream( path_to_config );

    const nlohmann::json::parser_callback_t CALLBACK{ nullptr };
    const bool DISALLOW_EXCEPTIONS{ false };
    const bool COMMENTS_IGNORED{ true };
    const auto jf = nlohmann::json::parse( config_stream, CALLBACK, DISALLOW_EXCEPTIONS, COMMENTS_IGNORED );

    if( !jf.is_object( ) )
    {
        return;
    }

    m_generators_count = jf[ "Generators" ];
    m_blocks_total = jf[ "Blocks" ];
    m_block_size_bytes = jf[ "Block_size_bytes" ];
    m_hashers_count = jf[ "Hashers" ];

    LOG_INFO_MSG( m_generators_count, m_blocks_total, m_block_size_bytes, m_hashers_count );
}

}  // namespace application
}  // namespace uni
