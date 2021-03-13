////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/Application.hpp
/// @brief Declaration application class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2020-2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <string>

namespace uni
{
namespace application
{
class Application
{
public:
    explicit Application( const std::string& path_to_config );
    ~Application( );

public:
    bool run( );

private:
    class Impl;
    std::unique_ptr< Impl > m_impl;
};

}  // namespace application
}  // namespace uni
