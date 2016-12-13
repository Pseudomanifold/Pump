#include "Node.hh"

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

Node Node::fromDescription( const std::string& description )
{
  std::istringstream stream( description );

  std::regex reTag( "[[:space:]]*([^[:space:]]+):[[:space:]]+([^[:space:]]*)" );
  std::string line;

  std::string keyName   = "name";
  std::string keyInput  = "input";
  std::string keyOutput = "output";

  Node node;

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
