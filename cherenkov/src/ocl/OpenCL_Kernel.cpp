
#include "ocl/OpenCL_Kernel.h"




OCL_Kernel::OCL_Kernel(std::string functionName)
	:m_functionName(functionName)
{}


void OCL_Kernel::createKernel(cl::Program& prog)
{
	cl_int error;

	m_kernel = cl::Kernel(prog, m_functionName.c_str(), &error);
	errorCheck("Kernel creation", error);
}



void OCL_Kernel::addMemoryGL(cl::Context& context, const int parameterNumber, const cl_GLint textureFlag, const cl_GLuint textureID)
{
	cl_int error;

	cl::Memory texture = cl::ImageGL(context, CL_MEM_READ_WRITE, textureFlag, 0, textureID, &error);
	errorCheck("shared texture creation", error);
	m_kernel.setArg(parameterNumber, sizeof(cl::Memory), &texture);

	m_memory.push_back(texture);
}




void OCL_Kernel::writeData(cl::CommandQueue& queue)
{
	cl_int error;

	for (auto itr : m_buffer)
	{
		if (std::get<1>(itr) == nullptr)
			continue;

		error = queue.enqueueWriteBuffer(std::get<0>(itr), CL_FALSE, 0, std::get<2>(itr), std::get<1>(itr));
		errorCheck("Buffer Transmit", error);
	}

	queue.finish();
}

void OCL_Kernel::readData(cl::CommandQueue& queue, unsigned int i)
{
	cl_int error;
	error = queue.enqueueReadBuffer(std::get<0>(m_buffer[i]) , CL_TRUE, 0, std::get<2>(m_buffer[i]), std::get<1>(m_buffer[i]));
	errorCheck("Buffer Read", error);
}



void OCL_Kernel::acquireOGL(cl::CommandQueue& queue)
{
	cl_int error;

	if (m_memory.size() == 0)
	{
		std::cout << "No OpenGL texture defined!!!" << std::endl;
		std::cin.get();
		exit(-1);
	}

	error = queue.enqueueAcquireGLObjects(&m_memory);
	errorCheck("Acquire Texture", error);
}

void OCL_Kernel::releaseOGL(cl::CommandQueue& queue)
{
	cl_int error;

	error = queue.enqueueReleaseGLObjects(&m_memory);
	errorCheck("Release Texture", error);
}



void OCL_Kernel::info()
{
	cl_uint tmp;
	m_kernel.getInfo(CL_KERNEL_NUM_ARGS, &tmp);
	std::cout << "Number of Arguments: " << tmp << std::endl;
}