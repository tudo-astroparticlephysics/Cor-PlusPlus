/* CorPP.cpp
 * this file is part of CorPP for CORSIKA
 *
 * Copyright (C) <2017> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "import_functions.h"

#include "boost/program_options.hpp"

#include <iostream>


int main(int argc, char** argv)
{
	try
	{
		/// Variables to be set depending on command line arguments

		std::string steering;
		std::vector<std::string> parameter;

		/// Set commandline arguments
		boost::program_options::options_description desc("Options");
		desc.add_options()("help, h", "Print help messages")
				("steering,s", boost::program_options::value<std::string>(&steering)->required(), "Path to steering information")
				(",P", boost::program_options::value< decltype(parameter)>(&parameter), "Forwards special parameter to individual modules");

		/// Boost default code
		boost::program_options::variables_map vm;

		try
		{
			/// can throw
			boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

			/// --help / -h option
			if (vm.count("help"))
			{
				std::cout << "Cor-PlusPlus Help Text" << std::endl
						<< desc << std::endl;
				return 0;
			}

			boost::program_options::notify(vm); // throws on error, so do after help in case
							// there are any problems
		}
		catch(boost::program_options::required_option& e)
	    {
	      std::cerr << "Required option missing: " << e.what() << std::endl;

	      return -2;
	    }
		catch (boost::program_options::error& e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl;

			return -2;
		}

		/// Application Code
		corsika_();

	}
	catch (std::exception& e)
	{
		std::cerr << "Unhandled Exception reached the top of main: " << e.what()
				<< ", application will now exit" << std::endl;
		return -1;

	}



	return 0;
}
