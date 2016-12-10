#ifndef PUMP_NODE_HH__
#define PUMP_NODE_HH__

#include <string>
#include <vector>

namespace pump
{

class Node
{
public:
  Node( const std::string& name );

  void addInput( const std::string& name );
  void addOutput( const std::string& name );

  std::size_t numInputs()  const noexcept;
  std::size_t numOutputs() const noexcept;

  bool isSource() const noexcept;
  bool isSink()   const noexcept;

private:
  std::string _name;

  std::vector<std::string> _inputs;
  std::vector<std::string> _outputs;
};

} // namespace pump

#endif
