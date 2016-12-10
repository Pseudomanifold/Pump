#ifndef PUMP_PROCESSES_HH__
#define PUMP_PROCESSES_HH__

#include <string>

namespace pump
{

/*
  Executes the given command and returns its output, i.e. everything
  that is written to STDOUT. An empty string is not necessarily
  indicative of an error.
*/

std::string get_stdout( const std::string& command );

} // namespace pump

#endif
