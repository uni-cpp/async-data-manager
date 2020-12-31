////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/CrcComputer.hpp
/// @brief Implementation of CrcComputer class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "uni/application/Hasher.hpp"

#include <iostream>

namespace uni
{
namespace application
{
Hasher::Hasher( const std::string& name, HasherListener* listener, DispatchQueue& queue )
    : Thread{ name }
    , m_listener{ listener }
    , m_queue{ queue }
{
}

Hasher::~Hasher( )
{
    m_is_working = false;
    m_listener = nullptr;
}

void
Hasher::run( )
{
    std::cout << "-----===== Hasher::run(" << get_name( ) << ") =====-----" << std::endl;

    std::hash< std::string > hasher;

    while( m_is_working )
    {
        std::string block;
        if( m_queue.pop_front( block ) )
        {
            // hash it in all threads
            // Compare
            size_t hash = hasher( block );
            m_listener->on_block_hashing_finished( std::to_string( hash ) );
        }
    }

    if( m_listener != nullptr )
    {
        m_listener->on_job_finished( m_queue.empty( ) );
    }
}

void
Hasher::on_start( )
{
}

void
Hasher::on_stop( )
{
    m_is_working = false;
}

}  // namespace application
}  // namespace uni
