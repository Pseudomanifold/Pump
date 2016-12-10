#include "Node.hh"

namespace pump
{

Node::Node( const std::string& name )
  : _name( name )
{
}

void Node::addInput( const std::string& name )
{
  _inputs.push_back( name );
}

void Node::addOutput( const std::string& name )
{
  _outputs.push_back( name );
}

bool Node::isSource() const noexcept
{
  return _inputs.empty();
}

bool Node::isSink() const noexcept
{
  return _outputs.empty();
}

} // namespace pump
