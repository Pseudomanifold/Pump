#ifndef PUMP_PUMP_HH__
#define PUMP_PUMP_HH__

#include <string>
#include <vector>

#include "Node.hh"

namespace pump
{

class Pump
{
public:
  void load( const std::string& filename );
  void save( const std::string& filename );

private:
  void add( Node&& node );
  Node get( const std::string& name );

  std::vector<Node> _nodes;
};

} // namespace pump

#endif
