////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/Application.cpp
/// @brief Implementation of Application class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2020-2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "uni/application/Application.hpp"

#include "uni/application/Config.hpp"
#include "uni/application/DataGenerator.hpp"
#include "uni/application/DataGeneratorListener.hpp"
#include "uni/application/DispatchQueue.hpp"
#include "uni/application/Hasher.hpp"
#include "uni/application/HasherListener.hpp"

#include <uni/common/ErrorCode.hpp>
#include <uni/common/Log.hpp>

#include <condition_variable>
#include <vector>

namespace uni::application
{
class Application::Impl
    : public DataGeneratorListener
    , public HasherListener
{
public:
    explicit Impl( const std::string& path_to_config );

    auto run( ) -> ErrorCode;

private:
    // DataGeneratorListener
    void on_block_generated( const std::string block ) override;
    void on_generation_finished( const std::string thread_name ) override;

    // HasherListener
    void on_block_hashing_finished( const std::string& hash ) override;
    void on_job_finished( bool is_success ) override;

    auto start_async_generators( ) -> ErrorCode;
    auto start_async_hashers( ) -> ErrorCode;
    auto stop( ) -> ErrorCode;

    Config m_config{};

    std::mutex m_mutex_generator{};
    bool m_is_job_done{ false };
    DispatchQueue m_queue{};
    std::vector< std::unique_ptr< DataGenerator > > m_generators{};

    std::mutex m_mutex_hasher{};
    std::condition_variable cv{};
    std::unique_ptr< Hasher > m_hasher{};
};


Application::Impl::Impl( const std::string& path_to_config )
    : m_config{ path_to_config }
{
    LOG_TRACE_MSG( "" );
}

auto
Application::Impl::run( ) -> ErrorCode
{
    LOG_TRACE_MSG( "" );

    ErrorCode error_code = start_async_generators( );
    if( ErrorCode::NONE != error_code )
    {
        LOG_ERROR_MSG( "Async generators did not start." );
        return error_code;
    }

    error_code = start_async_hashers( );
    if( ErrorCode::NONE != error_code )
    {
        LOG_ERROR_MSG( "Async hashers did not start." );
        return error_code;
    }

    {
        std::unique_lock< std::mutex > lk( m_mutex_hasher );
        cv.wait( lk, [this] { return m_is_job_done; } );
    }

    stop( );

    return ErrorCode::NONE;
}

auto
Application::Impl::start_async_generators( ) -> ErrorCode
{
    LOG_TRACE_MSG( "" );

    for( size_t generator_number = 0U; generator_number < m_config.generators_count; ++generator_number )
    {
        const std::string name{ "Generator №" + std::to_string( generator_number ) };

        m_generators.emplace_back( std::make_unique< DataGenerator >( name, this, m_config.block_size_bytes ) );
        if( !m_generators.back( ) )
        {
            LOG_ERROR_MSG( "Generator did not created - ", generator_number );
            return ErrorCode::INTERNAL;
        }

        const auto error_code = m_generators.back( )->start( );
        if( ErrorCode::NONE != error_code )
        {
            LOG_ERROR_MSG( "Generator did not start - ", generator_number, m_generators.back( )->get_name( ) );
            return error_code;
        }
    }

    return ErrorCode::NONE;
}

auto
Application::Impl::start_async_hashers( ) -> ErrorCode
{
    LOG_TRACE_MSG( "" );

    const std::string name{ "Hasher pool" };
    m_hasher = std::make_unique< Hasher >( name, this, m_queue );
    if( !m_hasher )
    {
        LOG_ERROR_MSG( "Hasher was not created." );
        return ErrorCode::INTERNAL;
    }

    const auto error_code = m_hasher->start( );
    if( ErrorCode::NONE != error_code )
    {
        LOG_ERROR_MSG( "Hasher did not start. ", m_hasher->get_name( ) );
        return ErrorCode::INTERNAL;
    }

    return ErrorCode::NONE;
}

void
Application::Impl::on_block_generated( const std::string block )
{
    LOG_TRACE_MSG( "" );

    // Could be Dispatched to the properly thread here instead of mutex
    std::lock_guard< std::mutex > lock( m_mutex_generator );
    static size_t block_counter{ 0U };
    ++block_counter;

    if( block_counter <= m_config.blocks_count )
    {
        m_queue.push( block );
        return;
    }
    else
    {
        LOG_DEBUG_MSG( "Redundant block received. Ignored" );
    }
}

void
Application::Impl::on_generation_finished( const std::string thread_name )
{
    LOG_TRACE_MSG( "" );

    LOG_INFO_MSG( "Thread name = ", thread_name, " have been finilized!" );
}

void
Application::Impl::on_block_hashing_finished( const std::string& hash )
{
    LOG_TRACE_MSG( "" );

    static size_t counter{ 0U };
    ++counter;

    LOG_INFO_MSG( "Hash = ", hash, " counter = ", counter );

    if( m_config.blocks_count == counter )
    {
        {
            std::unique_lock< std::mutex > lk( m_mutex_hasher );
            m_is_job_done = true;
        }
        cv.notify_one( );
    }
}

void
Application::Impl::on_job_finished( bool is_success )
{
    LOG_TRACE_MSG( "" );

    LOG_INFO_MSG( "Job finished with result = ", is_success );
}

auto
Application::Impl::stop( ) -> ErrorCode
{
    LOG_TRACE_MSG( "" );

    for( auto& generator : m_generators )
    {
        if( generator )
        {
            const auto error_code = generator->stop( );
            if( ErrorCode::NONE != error_code )
            {
                LOG_ERROR_MSG( "Generator ", generator->get_name( ), " couldn't stop" );
            }
        }
    }

    m_generators.clear( );

    LOG_DEBUG_MSG( m_queue.size( ) );

    return ErrorCode::NONE;
}


Application::Application( const std::string& path_to_config )
{
    m_impl = std::make_unique< Impl >( path_to_config );

    if( !m_impl )
    {
        LOG_FATAL_MSG( "Application was not created" );
    }
}

// Destructor should be here.
// See additional info here - https://www.fluentcpp.com/2017/09/22/make-pimpl-using-unique_ptr/
Application::~Application( ) = default;

auto
Application::run( ) -> bool
{
    REQUIRED( m_impl, "Empty pointer to impl", false );

    const auto error = m_impl->run( );

    return ErrorCode::NONE != error;
}

}  // namespace uni::application
