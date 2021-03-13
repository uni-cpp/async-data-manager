////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/Application.hpp
/// @brief Declaration Manager class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uni/application/Config.hpp"
#include "uni/application/DataGenerator.hpp"
#include "uni/application/DataGeneratorListener.hpp"
#include "uni/application/DispatchQueue.hpp"
#include "uni/application/Hasher.hpp"
#include "uni/application/HasherListener.hpp"
#include <uni/common/ErrorCode.hpp>

#include <condition_variable>
#include <vector>

namespace uni
{
namespace application
{
class Application
    : public DataGeneratorListener
    , public HasherListener
{
public:
    explicit Application( const std::string& path_to_config );
    ~Application( ) = default;

public:
    ErrorCode run( );

    // Listeners could be incapsulated in Impl
    // DataGeneratorListener
private:
    void on_block_generated( const std::string block ) override;
    void on_generation_finished( const std::string thread_name ) override;

    // HasherListener
private:
    void on_block_hashing_finished( const std::string& hash ) override;
    void on_job_finished( bool is_success ) override;

private:
    ErrorCode start_async_generators( );
    ErrorCode start_async_hashers( );
    ErrorCode stop( );

private:
    Config m_config{};

    std::mutex m_mutex_generator{};
    bool m_is_job_done{ false };
    DispatchQueue m_queue{};
    std::vector< std::unique_ptr< DataGenerator > > m_generators{};

    std::mutex m_mutex_hasher{};
    std::condition_variable cv{};
    std::unique_ptr< Hasher > m_hasher{};
};

}  // namespace application
}  // namespace uni
