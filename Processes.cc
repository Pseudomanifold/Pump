#include "Processes.hh"

#include <memory>

#include <stdio.h>

namespace pump
{

std::string get_stdout( const std::string& command )
{
  std::shared_ptr<FILE> pipe( popen( command.c_str(), "r" ), pclose );

  std::string output;

  if( pipe )
  {
    constexpr int size = 512;
    char buffer[size]  = { 0 };

    while( !feof( pipe.get() ) )
    {
      if( fgets( buffer, size, pipe.get() ) != nullptr )
        output += buffer;
    }
  }

  return output;
}

} // namespace pump
