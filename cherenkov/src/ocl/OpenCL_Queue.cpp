
#include "ocl/OpenCL_Queue.h"


OCL_Queue::OCL_Queue(cl::Context con, cl::Device dev)
{
	cl_int error;

	m_queue = cl::CommandQueue(con, dev, 0, &error);
	errorCheck("CommandQueue creation", error);
}