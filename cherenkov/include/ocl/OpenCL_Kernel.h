#pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <tuple>


#include "ocl/OpenCL_Include.h"
#include "ocl/OpenCL_Error.h"


class OCL_Kernel
{
private:

	cl::Kernel m_kernel;

	std::string m_functionName;


	std::vector< cl::Memory > m_memory;
	std::vector<std::tuple<cl::Buffer, void*, size_t>> m_buffer;


public:

	OCL_Kernel(std::string functionName);


	void createKernel(cl::Program& prog);	


	void addMemoryGL(cl::Context& context, const int parameterNumber, const cl_GLint textureFlag, const cl_GLuint textureID);

	
	template<class T>
	inline void addBuffer(cl::Context& context, cl_mem_flags memoryFlag, cl_uint parameterNumber, std::vector<T>& data)
	{
		cl_int error;

		size_t size = sizeof(T) * data.size();

		cl::Buffer buffer = cl::Buffer(context, memoryFlag, size, 0, &error);
		errorCheck("Add Buffer (vector) Create", error);

		error = m_kernel.setArg(parameterNumber, buffer);
		errorCheck("Add Buffer (vector) Set", error);

		m_buffer.push_back(std::make_tuple(buffer, data.data(), size ));
	}

	// Size in bytes
	inline void addBuffer(cl::Context& context, cl_mem_flags memoryFlag, const cl_uint parameterNumber, size_t size)
	{
		cl_int error;

		cl::Buffer buffer = cl::Buffer(context, memoryFlag, size, 0, &error);
		errorCheck("Add Buffer (size) Create", error);

		error = m_kernel.setArg(parameterNumber, buffer);
		errorCheck("Add Buffer (size) Set", error);		

		m_buffer.push_back(std::make_tuple(buffer, nullptr, 0));
	}

	// Size in bytes
	inline void addBuffer(cl::Context& context, cl_mem_flags memoryFlag, const cl_uint parameterNumber, void* data, size_t size)
	{
		cl_int error;

		cl::Buffer buffer = cl::Buffer(context, memoryFlag, size, 0, &error);
		errorCheck("Add Buffer (size) Create", error);

		error = m_kernel.setArg(parameterNumber, buffer);
		errorCheck("Add Buffer (size) Set", error);		

		m_buffer.push_back(std::make_tuple(buffer, data, 0));
	}

	template<class T>
	inline void replaceBuffer(const int bufferID, cl::Context& context, cl_mem_flags memoryFlag, cl_uint parameterNumber, std::vector<T>& data)
	{
		cl_int error;

		size_t size = sizeof(T) * data.size();

		cl::Buffer buffer = cl::Buffer(context, memoryFlag, size, 0, &error);
		errorCheck("Add Buffer (vector) Create", error);

		error = m_kernel.setArg(parameterNumber, buffer);
		errorCheck("Add Buffer (vector) Set", error);

		//std::get<0>(m_buffer[bufferID]).release();
		m_buffer[bufferID] = std::make_tuple(buffer, data.data(), size);
		

	}
	
	void writeData(cl::CommandQueue& queue);

	void readData(cl::CommandQueue& queue, unsigned int i);

	template<class T>
	inline void setArg(const cl_uint parameterNumber, const T& data)
	{
		cl_int error = m_kernel.setArg(parameterNumber, data);

		errorCheck("Kernel Argument", error);
	}


	void acquireOGL(cl::CommandQueue& queue);

	void releaseOGL(cl::CommandQueue& queue);



	void info();


	inline cl::Kernel& getKernel() { return m_kernel; }

	inline cl::Buffer& getBuffer(unsigned int i)
	{
		if(i < m_buffer.size() )
			return std::get<0>(m_buffer[i]);
	
		std::cout << "Buffer dont exist!" << std::endl;
		std::cin.get();
		exit(-1);
	}

	template<class T>
	inline T readBuffer(unsigned int i)
	{		
		if(i < m_buffer.size() )
		{
			
			return *static_cast<T*>( std::get<1>(m_buffer[i]) );
		}
	
		std::cout << "Buffer dont exist!" << std::endl;
		std::cin.get();
		exit(-1);
	}

};

