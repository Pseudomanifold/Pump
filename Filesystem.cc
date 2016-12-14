#include "Filesystem.hh"

#include <stdio.h>

namespace pump
{

bool mv( const std::string& source, const std::string& target )
{
  return rename( source.c_str(), target.c_str() ) == 0;
}

} // namespace pump
