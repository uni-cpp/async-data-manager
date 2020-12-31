////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/Hasher.hpp
/// @brief Declaration of Hasher class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uni/application/DispatchQueue.hpp"
#include "uni/application/HasherListener.hpp"

#include <uni/common/Thread.hpp>

#include <atomic>

namespace uni
{
namespace application
{
class Hasher : public uni::common::Thread
{
public:
    Hasher( const std::string& name, HasherListener* listener, DispatchQueue& queue );
    ~Hasher( ) override;

    // Thread interface
protected:
    void run( ) override;
    void on_start( ) override;
    void on_stop( ) override;

private:
    std::atomic< bool > m_is_working{ true };
    // Should be weak_ptr
    HasherListener* m_listener{ nullptr };
    // Should be weak_ptr
    DispatchQueue& m_queue;
};

}  // namespace application
}  // namespace uni
