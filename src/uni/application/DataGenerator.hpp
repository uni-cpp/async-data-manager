////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/DataGenerator.hpp
/// @brief Declaration of DataGenerator class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uni/application/DataGeneratorListener.hpp"

#include <uni/common/Thread.hpp>

#include <atomic>

namespace uni
{
namespace application
{
class DataGenerator : public uni::common::Thread
{
public:
    DataGenerator( const std::string& name, DataGeneratorListener* listener, uint32_t block_size_bytes );
    ~DataGenerator( ) override;

    // Thread
protected:
    void run( ) override;
    void on_start( ) override;
    void on_stop( ) override;

private:
    std::atomic< bool > m_is_working{ true };
    // Could be weak_ptr?
    DataGeneratorListener* m_listener{ nullptr };
    uint32_t m_block_size_byte{};
};

}  // namespace application
}  // namespace uni
