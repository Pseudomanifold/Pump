#ifndef PUMP_EDGE_HH__
#define PUMP_EDGE_HH__

#include <string>

namespace pump
{

struct Edge
{
  std::string source;
  std::string target;

  unsigned sourcePortIndex;
  unsigned targetPortIndex;

  bool operator<( const Edge& other ) const noexcept;
};

} // namespace pump

#endif
