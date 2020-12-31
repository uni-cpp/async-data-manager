////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/DispatchQueue.hpp
/// @brief Declaration of DispatchQueue class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <deque>
#include <mutex>
#include <string>

namespace uni
{
namespace application
{
class DispatchQueue
{
public:
    void push( const std::string& new_element );
    void push( std::string&& new_element );

    bool pop_front( std::string& element_out );

    bool empty( ) const;
    size_t size( ) const;

private:
    mutable std::mutex m_mutex{};
    std::deque< std::string > m_queue{};
};

}  // namespace application
}  // namespace uni
