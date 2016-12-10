#ifndef PUMP_PUMP_HH__
#define PUMP_PUMP_HH__

#include <string>

namespace pump
{

class Pump
{
public:
  void load( const std::string& filename );
  void save( const std::string& filename );

};

} // namespace pump

#endif
