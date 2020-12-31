////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/DataGeneratorListener.hpp
/// @brief Declaration of DataGeneratorListener class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

namespace uni
{
namespace application
{
class DataGeneratorListener
{
public:
    virtual ~DataGeneratorListener( ) = default;

public:
    /// Notify and transfer new block
    virtual void on_block_generated( const std::string block ) = 0;

    /// Notify that job is done
    virtual void on_generation_finished( const std::string thread_name ) = 0;
};

}  // namespace application
}  // namespace uni
