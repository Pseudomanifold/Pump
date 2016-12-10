#ifndef PUMP_NODE_HH__
#define PUMP_NODE_HH__

#include <string>
#include <vector>

namespace pump
{

class Node
{
public:
  Node() = default;
  Node( const std::string& name );

  void        setName( const std::string& name );
  std::string    name() const noexcept;

  void addInput( const std::string& name );
  void addOutput( const std::string& name );

  std::size_t numInputs()  const noexcept;
  std::size_t numOutputs() const noexcept;

  bool isSource() const noexcept;
  bool isSink()   const noexcept;

  /**
    Builds a node from its textual description. If successful, a new
    node along with its ports will be returned.
  */

  static Node fromDescription( const std::string& description );

private:
  std::string _name = "undefined";

  std::vector<std::string> _inputs;
  std::vector<std::string> _outputs;
};

} // namespace pump

#endif
