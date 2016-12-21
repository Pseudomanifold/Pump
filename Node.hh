#ifndef PUMP_NODE_HH__
#define PUMP_NODE_HH__

#include <string>
#include <vector>

namespace pump
{

class Node
{
public:

  // Constructors ------------------------------------------------------

  /**
    Creates an invalid node with an unspecified command. Using this node
    does not have any negative consequences on a workflow, however.
  */

  Node() = default;

  // Attributes --------------------------------------------------------

  void        setID( const std::string& id );
  std::string    id() const noexcept;

  void        setName( const std::string& name );
  std::string    name() const noexcept;

  void        setCommand( const std::string& command );
  std::string    command() const noexcept;

  void addInput( const std::string& name );
  void addOutput( const std::string& name );

  std::string input( unsigned index ) const noexcept;
  std::size_t inputs()                const noexcept;

  std::string output( unsigned index ) const noexcept;
  std::size_t outputs()                const noexcept;

  bool isSource() const noexcept;
  bool isSink()   const noexcept;

  // Comparison predicates ---------------------------------------------

  /** Compares nodes by names and commands  */
  bool operator<( const Node& other ) const noexcept;

  /** Compares nodes for equality in name and command */
  bool operator==( const Node& other ) const noexcept;

  /** @overload operator==( const Node& other ) */
  bool operator!=( const Node& other ) const noexcept;

  // -------------------------------------------------------------------

  void execute();

  // -------------------------------------------------------------------

  /**
    Builds a node from its textual description. If successful, a new
    node along with its ports will be returned.
  */

  static Node fromDescription( const std::string& description,
                               const std::string& id,
                               const std::string& command );

private:
  std::string _id   = "undefined";
  std::string _name = "undefined";
  std::string _command;

  std::vector<std::string> _inputs;
  std::vector<std::string> _outputs;
};

} // namespace pump

#endif
