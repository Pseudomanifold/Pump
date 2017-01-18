#ifndef PUMP_PUMP_HH__
#define PUMP_PUMP_HH__

#include <string>
#include <vector>

#include "Edge.hh"
#include "Node.hh"

namespace pump
{

class Pump
{
public:
  void load( const std::string& filename );
  void save( const std::string& filename );

  /** Attempts to process all edges in the plan once */
  void run();

  /**
    Converts the loaded plan to a Makefile. The Makefile will encode all
    dependencies of the workflow automatically. Moreover, the \c Pump is
    going to ensure that output files of nodes are being copied to their
    respective targets.
  */

  std::string toMakefile() const noexcept;

private:
  void add( Node&& node );
  void add( Edge&& edge );

  Node get( const std::string& id ) const noexcept;

  void processEdge( const std::string& source, unsigned int sourcePort,
                    const std::string& target, unsigned int targetPort );

  std::vector<Node> _nodes;
  std::vector<Edge> _edges; // TODO: This needs to become a DAG
};

} // namespace pump

#endif
