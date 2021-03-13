////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/application/DataGenerator.cpp
/// @brief Implementation of DataGenerator class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "uni/application/DataGenerator.hpp"

#include <uni/common/Log.hpp>

#include <algorithm>
#include <climits>
#include <random>

namespace uni
{
namespace application
{
namespace
{
using random_engine = std::independent_bits_engine< std::default_random_engine, CHAR_BIT, unsigned char >;
}  // anonymous namespace

DataGenerator::DataGenerator( const std::string& name, DataGeneratorListener* listener, uint32_t block_size_bytes )
    : Thread{ name }
    , m_listener{ listener }
    , m_block_size_byte{ block_size_bytes }
{
}

DataGenerator::~DataGenerator( )
{
    m_is_working = false;
    m_listener = nullptr;
}

void
DataGenerator::run( )
{
    LOG_INFO_MSG( "-----===== DataGenerator::run(", get_name( ), ") =====-----" );

    random_engine random_engine;
    size_t block_number{ 0U };
    while( m_is_working )
    {
        std::string block_data{};
        block_data.resize( m_block_size_byte );
        std::generate( std::begin( block_data ), std::end( block_data ), std::ref( random_engine ) );
        if( m_listener != nullptr )
        {
            LOG_DEBUG_MSG( "Thread name = ", get_name( ), ". Block id = ", block_number );
            m_listener->on_block_generated( std::move( block_data ) );
        }
        ++block_number;
    }

    if( m_listener != nullptr )
    {
        m_listener->on_generation_finished( get_name( ) );
    }
}

void
DataGenerator::on_start( )
{
}

void
DataGenerator::on_stop( )
{
    m_is_working = false;
}

}  // namespace application
}  // namespace uni
