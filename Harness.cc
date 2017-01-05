#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "Processes.hh"

int main( int argc, char** argv )
{
  std::vector<std::string> arguments;
  arguments.reserve( argc );

  for( int i = 0; i < argc; i++ )
    arguments.push_back( argv[i] );

  unsigned inputs  = 0;
  unsigned outputs = 1;

  std::string command;

  std::regex reInputArgument( "-i([[:digit:]]+)" );
  std::regex reCommandArgument( "-c(.+)" );

  bool descriptionRequired = false;

  for( auto&& argument : arguments )
  {
    if( argument.substr( 0, 2 ) == "-i" )
    {
      std::smatch matches;
      std::regex_match( argument, matches, reInputArgument );

      inputs = static_cast<unsigned>( std::stoul( matches[1] ) );
    }
    else if( argument.substr( 0, 2 ) == "-c" )
    {
      std::smatch matches;
      std::regex_match( argument, matches, reCommandArgument );

      command = matches[1];
    }

    // Eat all other arguments first before deciding about whether to
    // show the self-description or not.
    else if( argument == "--description" )
      descriptionRequired = true;
  }

  // Stores the names of the input files that are fed to the harness
  // auxiliary program.
  std::vector<std::string> inputFiles;

  for( unsigned i = 0; i < inputs; i++ )
  {
    std::ostringstream stream;

    stream << command // FIXME: Better extraction of command name?
           << "_"
           << i
           << ".in";

    inputFiles.push_back( stream.str() );
  }

  std::vector<std::string> outputFiles;
  outputFiles.push_back( command + ".out" );

  // Print description and exit ----------------------------------------

  if( descriptionRequired )
  {
    std::cout << "name: " << command << "\n"
              << "command: " << arguments.front() << " -i" << inputs << " -c" << command << "\n";

    for( unsigned i = 0; i < inputs; i++ )
      std::cout << "input: " << inputFiles.at(i) << "\n";

    std::cout << "output: " << outputFiles.front() << "\n";
    return 0;
  }

  // Command execution -------------------------------------------------

  std::string completeCommand  = command;
  for( unsigned i = 0; i < inputs; i++ )
  {
    completeCommand += " ";
    completeCommand += inputFiles.at(i);
  }

  // TODO: Make the number of outputs configurable
  // TODO: Permit storing STDERR
  completeCommand += " > " + outputFiles.front();

  auto output = pump::get_stdout( completeCommand );

  // Storing output ----------------------------------------------------

  std::ofstream out( outputFiles.front() );
  out << output;
}
