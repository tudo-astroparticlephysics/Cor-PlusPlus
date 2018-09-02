#pragma once


#include <iostream>
#include <sstream>



#include "ocl/OpenCL_Include.h"

#include "ocl/OpenCL_Error.h"


#ifdef _WIN32
#include <Windows.h>
#include <Wingdi.h>
#else
#include <GL/glx.h>
#endif

class OCL_Hardware
{
private:


	cl::Platform m_platform;
	cl::Device   m_device;
	cl::Context  m_context;

	cl::Platform loadPlatform();

	cl::Device loadDevice(bool gl_device_sharing = false);


	cl::Context loadContext();

public:

	OCL_Hardware();

	

	inline cl::Platform& getPlatform() { return m_platform; }
	inline cl::Device& getDevice() { return m_device; }
	inline cl::Context& getContext() { return m_context; }
	

	

};

