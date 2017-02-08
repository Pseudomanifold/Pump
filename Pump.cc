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
#include <sstream>
#include <stdexcept>
#include <string>

namespace pump
{

namespace detail
{

Edge makeEdge( const std::string& sourceDescription, const std::string& targetDescription )
{
  std::regex reDescription( "([^\\.]+)\\.([[:digit:]]+)" );
  std::smatch matches;

  Edge edge;

  if( std::regex_match( sourceDescription, matches, reDescription ) )
  {
    edge.source          = matches[1];
    edge.sourcePortIndex = static_cast<unsigned>( std::stoul( matches[2] ) );
  }

  if( std::regex_match( targetDescription, matches, reDescription ) )
  {
    edge.target          = matches[1];
    edge.targetPortIndex = static_cast<unsigned>( std::stoul( matches[2] ) );
  }

  return edge;
}

}

void Pump::load( const std::string& filename )
{
  std::ifstream in( filename );
  if( !in )
    throw std::runtime_error( "Unable to read input file" );

  std::regex reNode( "[[:space:]]*([[:alnum:]]+):[[:space:]]+(.+)[[:space:]]*$" );
  std::regex reEdge( "[[:space:]]*([^[:space:]]+)[[:space:]]*->[[:space:]]*([^[:space:]]+)" );

  std::string line;

  while( std::getline( in, line ) )
  {
    if( line.empty() || line.front() == '#' )
      continue;

    std::smatch matches;

    if( std::regex_match( line, matches, reNode ) )
    {
      std::string id          = matches[1];
      std::string command     = matches[2];
      std::string description = get_stdout( command + " --description" );

      std::cerr << "* Line contains node: " << command << "\n"
                << "* ID:                 " << id << "\n"
                << "* Description length: " << description.size() << "\n";

      this->add(
        Node::fromDescription( description, id, command ) );
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

  // This ensures that edges with the same source are being ordered
  // according to their port indices. The proper traversal ordering
  // is only determined afterwards.
  std::sort( _edges.begin(), _edges.end() );

  // Sort edges --------------------------------------------------------

  std::map<Node, std::vector<Edge> > outgoingEdgeMap;
  std::map<Node, unsigned>           readyInputs;

  for( auto&& edge : _edges )
    outgoingEdgeMap[ this->get( edge.source) ].push_back( edge );

  // Contains nodes in the order in which they can be executed in the
  // workflow. This is akin to a topological ordering of the nodes in
  // the network.
  std::vector<Node> sortedNodes;
  sortedNodes.reserve( _nodes.size() );

  // Contains edge in the order in which they can be executed in the
  // workflow. It could be possible that multiple edges are ready to
  // be processed at the same time, but this is not (yet) handled.
  std::vector<Edge> sortedEdges;
  sortedEdges.reserve( _edges.size() );

  std::queue<Node> nodes;

  auto processNode = [this,
                      &nodes,
                      &outgoingEdgeMap,
                      &readyInputs,
                      &sortedEdges] ( const Node& node )
  {
    auto&& outgoingEdges = outgoingEdgeMap[node];

    sortedEdges.insert( sortedEdges.end(),
        outgoingEdges.begin(), outgoingEdges.end() );

    for( auto&& edge : outgoingEdges )
    {
      auto&& target         = this->get( edge.target );
      readyInputs[ target ] = readyInputs[ target ] + 1;

      if( readyInputs[ target ] == target.inputs() )
        nodes.push( target );
    }
  };

  for( auto&& node : _nodes )
  {
    if( node.isSource() )
    {
      processNode( node );
      sortedNodes.push_back( node );
    }
  }

  while( !nodes.empty() )
  {
    auto node = nodes.front();
    nodes.pop();

    sortedNodes.push_back( node );
    processNode( node );
  }

  _nodes.swap( sortedNodes );
  _edges.swap( sortedEdges );
}

void Pump::save( const std::string& filename )
{
  throw std::runtime_error( "Not yet implemented" );
}

void Pump::run()
{
  for( auto&& node : _nodes )
  {
    if( node.isSource() )
    {
      std::cerr << "* Executing node '" << node.name() << "'...\n";
      node.execute();
    }
  }

  std::map<Node, unsigned> readyInputs;
  std::queue<Node> nodeQueue;

  for( auto&& edge : _edges )
  {
    auto&& source = this->get( edge.source );
    std::cerr << "* Node '" << source.name() << "' has finished; processing edge...\n";

    this->processEdge( edge.source, edge.sourcePortIndex,
                       edge.target, edge.targetPortIndex );

    auto&& target       = this->get( edge.target );
    readyInputs[target] = readyInputs[target] + 1;

    if( readyInputs[target] == target.inputs() )
    {
      std::cerr << "* Node '" << target.name() << "' is ready for execution\n";
      std::cerr << "* Executing node '" << target.name() << "'...\n";

      readyInputs[target] = 0;
      nodeQueue.push( target );

      target.execute();
    }
  }
}

std::string Pump::toMakefile() const noexcept
{
  std::ostringstream stream;

  // Create "all" target for dependency resolution ---------------------

  stream << "all:";

  for( auto&& node : _nodes )
    stream << " " << node.name();

  stream << "\n\n";

  // Create individual node targets ------------------------------------

  for( auto&& node : _nodes )
  {
    stream << node.toMakefileRule();

    auto&& source = node;
    auto&& edges  = this->getOutgoingEdges( node.id() );

    for( auto&& e : edges )
    {
      auto&& target = this->get( e.target );
      auto&& output = source.output( e.sourcePortIndex - 1 );
      auto&& input  = target.input(  e.targetPortIndex - 1 );

      // TODO: Make behaviour configurable. It might be more useful to
      // move or copy outputs in order to keep the output directory
      // clean.
      //
      // As an advantage, the linking preserves the structure of the
      // workflow and permits changes at different points of it.
      stream << "\t" << "ln -sf " << output << " " << input << "\n";
    }

    stream << "\n";

  }

  return stream.str();
}

void Pump::add( Node&& node )
{
  _nodes.emplace_back( node );
}

void Pump::add( Edge&& edge )
{
  _edges.emplace_back( edge );
}

Node Pump::get( const std::string& id ) const noexcept
{
  auto it = std::find_if( _nodes.begin(), _nodes.end(),
                          [&id] ( const Node& node )
                          {
                            return node.id() == id;
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

  // TODO: Would copying work as well?
  // TODO: Would linking work as well?
  mv( sourceFile, targetFile );
}

std::vector<Edge> Pump::getOutgoingEdges( const std::string& id ) const noexcept
{
  std::vector<Edge> edges;

  std::copy_if( _edges.begin(), _edges.end(), std::back_inserter( edges ),
                [&id] ( const Edge& e )
                {
                  return e.source == id;
                } );

  return edges;
}

} // namespace pump

int main( int argc, char** argv )
{
  // Sounds like the beginning of a motivational video...
  pump::Pump pump;
  pump.load( "../examples/simple.workflow" );
  pump.run();

  std::cout << pump.toMakefile() << "\n";
}
