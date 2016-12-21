#include "Node.hh"
#include "Processes.hh"

#include <istream>
#include <regex>
#include <sstream>

#include <iostream>

namespace pump
{

Node::Node( const std::string& name )
  : _name( name )
{
}

void Node::setID( const std::string& id )
{
  _id = id;
}

std::string Node::id() const noexcept
{
  return _id;
}

void Node::setName( const std::string& name )
{
  _name = name;
}

std::string Node::name() const noexcept
{
  return _name;
}

void Node::setCommand( const std::string& command )
{
  _command = command;
}

std::string Node::command() const noexcept
{
  if( _command.empty() )
    return _name;
  else
    return _command;
}

void Node::addInput( const std::string& name )
{
  _inputs.push_back( name );
}

void Node::addOutput( const std::string& name )
{
  _outputs.push_back( name );
}

std::string Node::input( unsigned index ) const noexcept
{
  if( index < _inputs.size() )
    return _inputs[index];
  else
    return {};
}

std::size_t Node::inputs() const noexcept
{
  return _inputs.size();
}

std::string Node::output( unsigned index ) const noexcept
{
  if( index < _outputs.size() )
    return _outputs[index];
  else
    return {};
}

std::size_t Node::outputs() const noexcept
{
  return _outputs.size();
}

bool Node::isSource() const noexcept
{
  return _inputs.empty();
}

bool Node::isSink() const noexcept
{
  return _outputs.empty();
}

bool Node::operator<( const Node& other ) const noexcept
{
  return this->name() < other.name() && this->command() < other.command();
}

bool Node::operator==( const Node& other ) const noexcept
{
  return this->name() == other.name() && this->command() == other.command();
}

bool Node::operator!=( const Node& other ) const noexcept
{
  return !this->operator==( other );
}

void Node::execute()
{
  get_stdout( this->command() );
}

Node Node::fromDescription( const std::string& description,
                            const std::string& id,
                            const std::string& command )
{
  std::istringstream stream( description );

  std::regex reTag( "[[:space:]]*([^[:space:]]+):[[:space:]]+([^[:space:]]*)" );
  std::string line;

  std::string keyName   = "name";
  std::string keyInput  = "input";
  std::string keyOutput = "output";

  Node node;
  node.setID( id );
  node.setCommand( command );

  while( std::getline( stream, line ) )
  {
    std::smatch matches;

    if( std::regex_match( line, matches, reTag ) )
    {
      std::string key   = matches[1];
      std::string value = matches[2];

      if( key == keyName )
        node.setName( value );
      else if( key == keyInput )
        node.addInput( value );
      else if( key == keyOutput )
        node.addOutput( value );
      else
        std::cerr << "* Ignoring unknown key '" << key << "'\n";
    }
  }

  return node;
}

} // namespace pump
