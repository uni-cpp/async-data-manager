////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/Application.hpp
/// @brief Declaration Manager class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uni/application/DataGenerator.hpp"
#include "uni/application/DataGeneratorListener.hpp"
#include "uni/application/DispatchQueue.hpp"
#include "uni/application/Hasher.hpp"
#include "uni/application/HasherListener.hpp"

#include <condition_variable>
#include <cstdint>
#include <iostream>
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
    bool run( );
    bool start_async_generators( );
    bool start_async_hashers( );

    // DataGeneratorListener interface
public:
    void on_block_generated( const std::string block ) override;
    void on_generation_finished( const std::string thread_name ) override;

    // HasherListener interface
public:
    void on_block_hashing_finished( const std::string& hash ) override;
    void on_job_finished( bool is_success ) override;


private:
    void stop( );

private:
    uint32_t m_generators_count{};
    uint32_t m_blocks_total{};
    uint32_t m_block_size_bytes{};
    uint32_t m_hashers_count{};

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