////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/CrcComputer.hpp
/// @brief Implementation of CrcComputer class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "uni/application/Hasher.hpp"

#include <uni/common/Log.hpp>

namespace uni
{
namespace application
{
Hasher::Hasher( const std::string& name, HasherListener* listener, DispatchQueue& queue )
    : Thread{ { name } }
    , m_listener{ listener }
    , m_queue{ queue }
{
    LOG_TRACE_MSG( "" );
}

Hasher::~Hasher( )
{
    LOG_TRACE_MSG( "" );

    m_is_working = false;
    m_listener = nullptr;
}

void
Hasher::run( )
{
    LOG_TRACE_MSG( "-----===== Hasher::run(", get_name( ), ") =====-----" );

    std::hash< std::string > hasher;

    while( m_is_working )
    {
        std::string block;
        if( m_queue.pop_front( block ) )
        {
            // hash it in all threads
            // Compare
            size_t hash = hasher( block );
            m_listener->on_block_hashing_finished( std::to_string( hash ) );
        }
    }

    if( m_listener != nullptr )
    {
        m_listener->on_job_finished( m_queue.empty( ) );
    }
}

void
Hasher::on_start( )
{
    LOG_TRACE_MSG( "" );
}

void
Hasher::on_stop( )
{
    LOG_TRACE_MSG( "" );

    m_is_working = false;
}

}  // namespace application
}  // namespace uni
