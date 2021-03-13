////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/Application.cpp
/// @brief Implementation of Application class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2020-2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "uni/application/Application.hpp"
#include <uni/common/Log.hpp>

#include <algorithm>

namespace uni
{
namespace application
{
Application::Application( const std::string& path_to_config )
    : m_config{ path_to_config }
{
}

bool
Application::run( )
{
    bool is_success = start_async_generators( );
    if( !is_success )
    {
        return false;
    }

    is_success = start_async_hashers( );
    if( !is_success )
    {
        return false;
    }

    {
        std::unique_lock< std::mutex > lk( m_mutex_hasher );
        cv.wait( lk, [this] { return m_is_job_done; } );
    }

    stop( );

    return true;
}

bool
Application::start_async_generators( )
{
    for( size_t generator_number = 0U; generator_number < m_config.m_generators_count; ++generator_number )
    {
        const std::string name{ "Generator №" + std::to_string( generator_number ) };
        m_generators.emplace_back( std::make_unique< DataGenerator >( name, this, m_config.m_block_size_bytes ) );
        if( !m_generators.back( )->start( ) )
        {
            LOG_ERROR_MSG( m_generators.back( )->get_name( ) );
            return false;
        }
    }

    return true;
}

bool
Application::start_async_hashers( )
{
    const std::string name{ "Hasher pool" };
    m_hasher = std::make_unique< Hasher >( name, this, m_queue );

    if( !m_hasher->start( ) )
    {
        LOG_ERROR_MSG( m_hasher->get_name( ) );
        return false;
    }
    return true;
}

void
Application::on_block_generated( const std::string block )
{
    // Should be Dispatched to the properly thread here instead of mutex

    {
        std::lock_guard< std::mutex > lock( m_mutex_generator );
        static size_t block_counter{ 0U };
        ++block_counter;

        if( block_counter <= m_config.m_blocks_total )
        {
            m_queue.push( block );
            return;
        }
        else
        {
            LOG_DEBUG_MSG( "Redundant block received. Ignored" );
        }
    }
}

void
Application::on_generation_finished( const std::string thread_name )
{
    LOG_INFO_MSG( "Thread name = ", thread_name, " have been finilized!" );
}

void
Application::on_block_hashing_finished( const std::string& hash )
{
    static size_t counter{ 0U };
    ++counter;

    LOG_INFO_MSG( "Hash = ", hash, " counter = ", counter );

    if( m_config.m_blocks_total == counter )
    {
        {
            std::unique_lock< std::mutex > lk( m_mutex_hasher );
            m_is_job_done = true;
        }
        cv.notify_one( );
    }
}

void
Application::on_job_finished( bool is_success )
{
    LOG_INFO_MSG( "Job finished with result = ", is_success );
}

void
Application::stop( )
{
    for( auto& generator : m_generators )
    {
        if( generator )
        {
            const auto is_success = generator->stop( );
            if( !is_success )
            {
                LOG_ERROR_MSG( generator->get_name( ), "couldn't stop" );
            }
        }
    }

    m_generators.clear( );

    LOG_DEBUG_MSG( m_queue.size( ) );
}

}  // namespace application
}  // namespace uni
