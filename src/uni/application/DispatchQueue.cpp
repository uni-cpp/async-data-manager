////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/DispatchQueue.cpp
/// @brief Implementation of DispatchQueue class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "uni/application/DispatchQueue.hpp"

#include <uni/common/Log.hpp>

namespace uni
{
namespace application
{
void
DispatchQueue::push( const std::string& new_element )
{
    std::lock_guard< std::mutex > lock( m_mutex );
    m_queue.push_back( new_element );
}

void
DispatchQueue::push( std::string&& new_element )
{
    std::lock_guard< std::mutex > lock( m_mutex );
    m_queue.push_back( std::move( new_element ) );
}

bool
DispatchQueue::pop_front( std::string& element_out )
{
    std::lock_guard< std::mutex > lock( m_mutex );
    if( m_queue.empty( ) )
    {
        return false;
    }

    element_out = std::move( m_queue.front( ) );
    m_queue.pop_front( );

    return true;
}


bool
DispatchQueue::empty( ) const
{
    return m_queue.empty( );
}

size_t
DispatchQueue::size( ) const
{
    std::lock_guard< std::mutex > lock( m_mutex );
    size_t size = m_queue.size( );

    return size;
}

}  // namespace application
}  // namespace uni
