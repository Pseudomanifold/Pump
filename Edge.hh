#ifndef PUMP_EDGE_HH__
#define PUMP_EDGE_HH__

namespace pump
{

struct Edge
{
  std::string source;
  std::string target;

  bool isSourceInput;
  bool isTargetInput;

  unsigned sourcePortIndex;
  unsigned targetPortIndex;
};

} // namespace pump

#endif
