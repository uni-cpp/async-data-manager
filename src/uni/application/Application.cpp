////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/Application.cpp
/// @brief Implementation of Application class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "uni/application/Application.hpp"

#include <unistd.h>
#include <algorithm>

namespace uni
{
namespace application
{
Application::Application( const std::string& path_to_config )
{
    ( void ) path_to_config;

    // Data should be parsed from json to config struct
    constexpr uint32_t GENERATORS_COUNT = 5U;
    constexpr uint32_t BLOCKS_TOTAL = 1000U * GENERATORS_COUNT;
    constexpr uint32_t BLOCK_SIZE_BYTES = 100000U;
    constexpr uint32_t HASHERS_COUNT = 10U;

    m_generators_count = GENERATORS_COUNT;
    m_blocks_total = BLOCKS_TOTAL;
    m_block_size_bytes = BLOCK_SIZE_BYTES;
    m_hashers_count = HASHERS_COUNT;

    std::cout << m_generators_count << " " << m_blocks_total << " " << m_block_size_bytes << " " << m_hashers_count << std::endl;
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
    for( size_t generator_number = 0U; generator_number < m_generators_count; ++generator_number )
    {
        const std::string name{ "Generator №" + std::to_string( generator_number ) };
        m_generators.emplace_back( std::make_unique< DataGenerator >( name, this, m_block_size_bytes ) );
        if( !m_generators.back( )->start( ) )
        {
            std::cout << m_generators.back( )->get_name( ) << " couldn't start." << std::endl;
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
        std::cout << m_hasher->get_name( ) << " couldn't start." << std::endl;
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

        if( block_counter <= m_blocks_total )
        {
            m_queue.push( block );
            return;
        }
        else
        {
            std::cout << "Redundant block received. Ignored" << std::endl;
        }
    }
}

void
Application::on_generation_finished( const std::string thread_name )
{
    std::cout << "Thread name = " << thread_name << " have been finilized!" << std::endl;
}

void
Application::on_block_hashing_finished( const std::string& hash )
{
    static size_t counter{ 0U };
    ++counter;

    std::cout << "Hash = " << hash << " counter = " << counter << std::endl;

    if( m_blocks_total == counter )
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
    std::cout << "Job finished with result = " << is_success << std::endl;
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
                std::cout << generator->get_name( ) << "couldn't stop" << std::endl;
            }
        }
    }

    m_generators.clear( );

    std::cout << m_queue.size( ) << std::endl;
}

}  // namespace application
}  // namespace uni
