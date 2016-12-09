#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include <cmath>

std::string name;
std::string input  = "analyse_data.in";
std::string output = "analyse_data.out";

void description()
{
  std::cout << "---\n"
            << "name: "        << name   << "\n"
            << "description: " << ""     << "\n"
            << "input: "       << input  << "\n"
            << "output: "      << output << "\n";
}

int main( int argc, char** argv )
{
  if( argc == 2 )
  {
    name                 = argv[0];
    std::string argument = argv[1];

    if( argument == "--description" )
    {
      description();
      return 0;
    }
  }

  std::ifstream in( input );
  if( !in )
    return -1;

  std::vector<double> values;

  std::copy( std::istream_iterator<double>( in ), std::istream_iterator<double>(),
             std::back_inserter( values ) );

  if( values.empty() )
    return -1;

  auto itMinMax = std::minmax_element( values.begin(), values.end() );
  auto min      = *itMinMax.first;
  auto max      = *itMinMax.second;
  auto mean     = std::accumulate( values.begin(), values.end(), 0.0 ) / static_cast<double>( values.size() );

  std::ofstream out( output );
  if( !out )
    return -1;

  out << "---\n"
      << "min: " << min << "\n"
      << "max: " << max << "\n"
      << "mean: " << mean << "\n";
}
