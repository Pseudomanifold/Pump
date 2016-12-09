#include <iostream>
#include <fstream>
#include <random>
#include <string>

#include <cmath>

void description( const std::string& name )
{
  std::cout << "---\n"
            << "name: "        << name                << "\n"
            << "description: " << ""                  << "\n"
            << "output: "      << "generate_data.out" << "\n";
}

int main( int argc, char** argv )
{
  if( argc == 2 )
  {
    std::string name     = argv[0];
    std::string argument = argv[1];
    if( argument == "--description" )
    {
      description( name );
      return 0;
    }
  }

  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution( 0.0, 2*M_PI );

  std::ofstream out( "generate_data.out" );

  for( int i = 0; i < 100; i++ )
    out << distribution( generator ) << "\n";
}
