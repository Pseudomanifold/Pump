#include <iostream>
#include <fstream>
#include <random>
#include <string>

#include <cmath>

std::string name   = "generate_data";
std::string output = "generate_data.out";

void description()
{
  std::cout << "---\n"
            << "name: "        << name   << "\n"
            << "description: " << ""     << "\n"
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

  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution( 0.0, 2*M_PI );

  std::ofstream out( output );

  for( int i = 0; i < 100; i++ )
    out << distribution( generator ) << "\n";
}
