#ifndef PUMP_FILESYSTEM_HH__
#define PUMP_FILESYSTEM_HH__

#include <string>

namespace pump
{

/**
  Wrapper function for moving a file from a certain source to a certain
  target. The wrapper tries to duplicate the functionality of the \c mv
  command.
*/

bool mv( const std::string& source, const std::string& target );

} // namespace pump

#endif
