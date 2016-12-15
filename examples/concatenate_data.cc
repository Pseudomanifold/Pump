#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cmath>

std::string name   = "concatenate_data";
std::string input1 = name + "_1.in";
std::string input2 = name + "_2.in";
std::string output = name + ".out";

void description()
{
  std::cout << "name: "        << name   << "\n"
            << "description: " << ""     << "\n"
            << "input: "       << input1 << "\n"
            << "input: "       << input2 << "\n"
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

  std::ifstream in1( input1 );
  std::ifstream in2( input2 );

  if( !in1 || !in2 )
    return -1;

  std::string line;
  while( std::getline( in1, line ) )
    lines.push_back( line );

  while( std::getline( in2, line ) )
    lines.push_back( line );

  if( lines.empty() )
    return -1;

  std::ofstream out( output );
  if( !out )
    return -1;

  for( auto&& line : lines )
    out << line << "\n";
}
