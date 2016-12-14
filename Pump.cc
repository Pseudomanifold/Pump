#include "Filesystem.hh"
#include "Node.hh"
#include "Pump.hh"
#include "Processes.hh"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>

namespace pump
{

namespace detail
{

Edge makeEdge( const std::string& sourceDescription, const std::string& targetDescription )
{
  std::regex reDescription( "([^\\.]+)\\.(out|in)\\.([[:digit:]]+)" );
  std::smatch matches;

  Edge edge;

  if( std::regex_match( sourceDescription, matches, reDescription ) )
  {
    edge.source          = matches[1];
    edge.isSourceInput   = matches[2] == "in";
    edge.sourcePortIndex = static_cast<unsigned>( std::stoul( matches[3] ) );
  }

  if( std::regex_match( targetDescription, matches, reDescription ) )
  {
    edge.target          = matches[1];
    edge.isTargetInput   = matches[2] == "in";
    edge.targetPortIndex = static_cast<unsigned>( std::stoul( matches[3] ) );
  }

  return edge;
}

}

void Pump::load( const std::string& filename )
{
  std::ifstream in( filename );
  if( !in )
    throw std::runtime_error( "Unable to read input file" );

  std::regex reNode( "[[:space:]]*node:[[:space:]]+([^[:space:]]+)" );
  std::regex reEdge( "[[:space:]]*([^[:space:]]+)[[:space:]]*->[[:space:]]*([^[:space:]]+)" );

  std::string line;

  while( std::getline( in, line ) )
  {
    if( line.empty() || line.front() == '#' )
      continue;

    std::smatch matches;

    if( std::regex_match( line, matches, reNode ) )
    {
      std::string command     = matches[1];
      std::string description = get_stdout( command + " --description" );

      std::cerr << "* Line contains node: " << command << "\n"
                << "* Description length: " << description.size() << "\n";

      this->add(
        Node::fromDescription( description, command ) );
    }
    else if( std::regex_match( line, matches, reEdge ) )
    {
      std::string source = matches[1];
      std::string target = matches[2];

      std::cerr << "* Line contains edge: " << line   << "\n"
                << "* Source:             " << source << "\n"
                << "* Target:             " << target << "\n";

      this->add(
        detail::makeEdge( source, target ) );
    }
    else
      std::cerr << "* Unknown line: " << line << "\n";
  }
}

void Pump::save( const std::string& filename )
{
  throw std::runtime_error( "Not yet implemented" );
}

void Pump::run()
{
  std::set<Node> finishedNodes;

  for( auto&& node : _nodes )
  {
    if( node.isSource() )
    {
      std::cerr << "* Executing node '" << node.name() << "'...\n";
      node.execute();

      finishedNodes.insert( node );
    }
  }

  std::map<Node, unsigned> readyInputs;
  std::queue<Node> nodeQueue;

  // TODO: Need proper traversal order. This is of course incorrect for
  // most of the networks that could be employed...
  for( auto&& edge : _edges )
  {
    auto&& source = this->get( edge.source );
    if( finishedNodes.find( source ) != finishedNodes.end() )
    {
      std::cerr << "* Source '" << source.name() << "' has finished; processing edge...\n";

      this->processEdge( edge.source, edge.sourcePortIndex,
                         edge.target, edge.targetPortIndex );

      auto&& target       = this->get( edge.target );
      readyInputs[target] = readyInputs[target] + 1;

      if( readyInputs[target] == target.inputs() )
      {
        std::cerr << "* Node '" << target.name() << "' is ready for execution\n";

        readyInputs[target] = 0;
        nodeQueue.push( target );
      }
    }
  }

  while( !nodeQueue.empty() )
  {
    auto node = nodeQueue.front();
    nodeQueue.pop();

    std::cerr << "* Executing queued node '" << node.name() << "'...\n";

    node.execute();
  }
}

void Pump::add( Node&& node )
{
  _nodes.emplace_back( node );
}

void Pump::add( Edge&& edge )
{
  _edges.emplace_back( edge );
}

Node Pump::get( const std::string& name )
{
  auto it = std::find_if( _nodes.begin(), _nodes.end(),
                          [&name] ( const Node& node )
                          {
                            return node.name() == name;
                          } );

  if( it != _nodes.end() )
    return *it;
  else
    return Node();
}

void Pump::processEdge( const std::string& source, unsigned int sourcePort,
                        const std::string& target, unsigned int targetPort )
{
  // TODO: Do I really want to map ports starting from 1, or should
  // I rather use the default zero-based indices?

  auto&& sourceNode = this->get( source );
  auto&& targetNode = this->get( target );

  auto&& sourceFile = sourceNode.output(  sourcePort - 1 );
  auto&& targetFile = targetNode.input(   targetPort - 1 );

  mv( sourceFile, targetFile );
}

} // namespace pump

int main( int argc, char** argv )
{
  // Sounds like the beginning of a motivational video...
  pump::Pump pump;
  pump.load( "../examples/simple.workflow" );
  pump.run();
}
