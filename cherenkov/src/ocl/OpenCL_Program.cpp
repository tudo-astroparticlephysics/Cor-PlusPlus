

#include "ocl/OpenCL_Program.h"



std::string OCL_Program::loadSource()
{
	std::ifstream file;

	// setException
	file.exceptions(std::ifstream::badbit);

	try
	{
		file.open(m_path, std::ios::in | std::ios::binary);

		std::stringstream sstr;
		sstr << file.rdbuf();

		file.close();

		return sstr.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "OpenCL Error: " << m_path << " could not be read!" << std::endl;
		std::cout << e.what() << std::endl;

		std::cin.get();
		exit(-1);
	}
	return std::string();
}


OCL_Program::OCL_Program(std::string path)
	:m_path(path) {}

cl::Program& OCL_Program::load(cl::Device& device, cl::Context& context)
{
	std::string source = loadSource();

	cl::Program::Sources sources;

	sources.push_back({ source.c_str(), source.length() });

	cl::Program program(context, sources);
	if (program.build({ device }, "-cl-mad-enable") != CL_SUCCESS) {
		std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
		std::cin.get();
		exit(1);
	}

	m_program = program;

	return program;
}



void OCL_Program::info()
{
	
}