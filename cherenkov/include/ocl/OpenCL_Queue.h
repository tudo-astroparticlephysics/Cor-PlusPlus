#pragma once


#include <iostream>


#include "ocl/OpenCL_Include.h"

#include "ocl/OpenCL_Error.h"


class OCL_Queue
{
private:

	cl::CommandQueue m_queue;


public:

	OCL_Queue(cl::Context con, cl::Device dev);

	inline cl::CommandQueue& getQueue() { return m_queue; }
	

	/*cl::CommandQueue* operator->()
	{
		return &m_queue;
	}*/

};

