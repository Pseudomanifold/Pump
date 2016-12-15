#include "Edge.hh"

namespace pump
{

bool Edge::operator<( const Edge& other ) const noexcept
{
  if( this->source == other.source )
    return this->sourcePortIndex < other.sourcePortIndex;
  else
    return this->source < other.source;
}

} // namespace pump
