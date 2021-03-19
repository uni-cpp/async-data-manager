////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/Config.hpp
/// @brief Declaration Config class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>
#include <uni/common/Log.hpp>

namespace uni
{
namespace application
{
class Config
{
public:
    Config( ) = default;
    explicit Config( const std::string& path_to_config );

public:
    uint32_t generators_count{};
    uint32_t blocks_count{};
    uint32_t block_size_bytes{};
    uint32_t hashers_count{};

private:
    std::string temporary_member{ "test" };
    LOG_CLASS( Config, LOG_IT( temporary_member ), LOG_IT( temporary_member ) );
};

}  // namespace application
}  // namespace uni
