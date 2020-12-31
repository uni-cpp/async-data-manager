////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/HasherListener.hpp
/// @brief Declaration of HasherListener class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

namespace uni
{
namespace application
{
class HasherListener
{
public:
    virtual ~HasherListener( ) = default;

    virtual void on_block_hashing_finished( const std::string& hash ) = 0;
    virtual void on_job_finished( bool is_success ) = 0;


public:
};

}  // namespace application
}  // namespace uni
