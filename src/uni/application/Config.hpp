////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/Config.hpp
/// @brief Declaration Config class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>

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
    uint32_t m_generators_count{};
    uint32_t m_blocks_total{};
    uint32_t m_block_size_bytes{};
    uint32_t m_hashers_count{};
};

}  // namespace application
}  // namespace uni
