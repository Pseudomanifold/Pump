#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cmath>

std::string name   = "pass_data";
std::string input  = name + ".in";
std::string output = name + ".out";

void description()
{
  std::cout << "name: "        << name   << "\n"
            << "description: " << ""     << "\n"
            << "input: "       << input  << "\n"
            << "output: "      << output << "\n";
}

int main( int argc, char** argv )
{
  if( argc == 2 )
  {
    std::string argument = argv[1];

    if( argument == "--description" )
    {
      description();
      return 0;
    }
  }

  std::vector<std::string> lines;

  std::ifstream in( input );
  if( !in )
    return -1;

  std::string line;
  while( std::getline( in, line ) )
    lines.push_back( line );

  if( lines.empty() )
    return -1;

  std::ofstream out( output );
  if( !out )
    return -1;

  for( auto&& line : lines )
    out << line << "\n";
}
