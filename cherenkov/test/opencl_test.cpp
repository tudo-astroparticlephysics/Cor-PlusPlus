
    /*
 * Display Device Information
 *
 * Script to print out some information about the OpenCL devices
 * and platforms available on your system
 *
 * History: C++ version written by Tom Deakin, 2012
 *          Updated by Tom Deakin, August 2013
*/

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include "ocl/cl2.cpp"
#include <iostream>
#include <vector>

const char *err_code (cl_int err_in)
{
    switch (err_in) {
        case CL_SUCCESS:
            return (char*)"CL_SUCCESS";
        case CL_DEVICE_NOT_FOUND:
            return (char*)"CL_DEVICE_NOT_FOUND";
        case CL_DEVICE_NOT_AVAILABLE:
            return (char*)"CL_DEVICE_NOT_AVAILABLE";
        case CL_COMPILER_NOT_AVAILABLE:
            return (char*)"CL_COMPILER_NOT_AVAILABLE";
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            return (char*)"CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case CL_OUT_OF_RESOURCES:
            return (char*)"CL_OUT_OF_RESOURCES";
        case CL_OUT_OF_HOST_MEMORY:
            return (char*)"CL_OUT_OF_HOST_MEMORY";
        case CL_PROFILING_INFO_NOT_AVAILABLE:
            return (char*)"CL_PROFILING_INFO_NOT_AVAILABLE";
        case CL_MEM_COPY_OVERLAP:
            return (char*)"CL_MEM_COPY_OVERLAP";
        case CL_IMAGE_FORMAT_MISMATCH:
            return (char*)"CL_IMAGE_FORMAT_MISMATCH";
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:
            return (char*)"CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case CL_BUILD_PROGRAM_FAILURE:
            return (char*)"CL_BUILD_PROGRAM_FAILURE";
        case CL_MAP_FAILURE:
            return (char*)"CL_MAP_FAILURE";
        case CL_MISALIGNED_SUB_BUFFER_OFFSET:
            return (char*)"CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
            return (char*)"CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case CL_INVALID_VALUE:
            return (char*)"CL_INVALID_VALUE";
        case CL_INVALID_DEVICE_TYPE:
            return (char*)"CL_INVALID_DEVICE_TYPE";
        case CL_INVALID_PLATFORM:
            return (char*)"CL_INVALID_PLATFORM";
        case CL_INVALID_DEVICE:
            return (char*)"CL_INVALID_DEVICE";
        case CL_INVALID_CONTEXT:
            return (char*)"CL_INVALID_CONTEXT";
        case CL_INVALID_QUEUE_PROPERTIES:
            return (char*)"CL_INVALID_QUEUE_PROPERTIES";
        case CL_INVALID_COMMAND_QUEUE:
            return (char*)"CL_INVALID_COMMAND_QUEUE";
        case CL_INVALID_HOST_PTR:
            return (char*)"CL_INVALID_HOST_PTR";
        case CL_INVALID_MEM_OBJECT:
            return (char*)"CL_INVALID_MEM_OBJECT";
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
            return (char*)"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case CL_INVALID_IMAGE_SIZE:
            return (char*)"CL_INVALID_IMAGE_SIZE";
        case CL_INVALID_SAMPLER:
            return (char*)"CL_INVALID_SAMPLER";
        case CL_INVALID_BINARY:
            return (char*)"CL_INVALID_BINARY";
        case CL_INVALID_BUILD_OPTIONS:
            return (char*)"CL_INVALID_BUILD_OPTIONS";
        case CL_INVALID_PROGRAM:
            return (char*)"CL_INVALID_PROGRAM";
        case CL_INVALID_PROGRAM_EXECUTABLE:
            return (char*)"CL_INVALID_PROGRAM_EXECUTABLE";
        case CL_INVALID_KERNEL_NAME:
            return (char*)"CL_INVALID_KERNEL_NAME";
        case CL_INVALID_KERNEL_DEFINITION:
            return (char*)"CL_INVALID_KERNEL_DEFINITION";
        case CL_INVALID_KERNEL:
            return (char*)"CL_INVALID_KERNEL";
        case CL_INVALID_ARG_INDEX:
            return (char*)"CL_INVALID_ARG_INDEX";
        case CL_INVALID_ARG_VALUE:
            return (char*)"CL_INVALID_ARG_VALUE";
        case CL_INVALID_ARG_SIZE:
            return (char*)"CL_INVALID_ARG_SIZE";
        case CL_INVALID_KERNEL_ARGS:
            return (char*)"CL_INVALID_KERNEL_ARGS";
        case CL_INVALID_WORK_DIMENSION:
            return (char*)"CL_INVALID_WORK_DIMENSION";
        case CL_INVALID_WORK_GROUP_SIZE:
            return (char*)"CL_INVALID_WORK_GROUP_SIZE";
        case CL_INVALID_WORK_ITEM_SIZE:
            return (char*)"CL_INVALID_WORK_ITEM_SIZE";
        case CL_INVALID_GLOBAL_OFFSET:
            return (char*)"CL_INVALID_GLOBAL_OFFSET";
        case CL_INVALID_EVENT_WAIT_LIST:
            return (char*)"CL_INVALID_EVENT_WAIT_LIST";
        case CL_INVALID_EVENT:
            return (char*)"CL_INVALID_EVENT";
        case CL_INVALID_OPERATION:
            return (char*)"CL_INVALID_OPERATION";
        case CL_INVALID_GL_OBJECT:
            return (char*)"CL_INVALID_GL_OBJECT";
        case CL_INVALID_BUFFER_SIZE:
            return (char*)"CL_INVALID_BUFFER_SIZE";
        case CL_INVALID_MIP_LEVEL:
            return (char*)"CL_INVALID_MIP_LEVEL";
        case CL_INVALID_GLOBAL_WORK_SIZE:
            return (char*)"CL_INVALID_GLOBAL_WORK_SIZE";
        case CL_INVALID_PROPERTY:
            return (char*)"CL_INVALID_PROPERTY";

        default:
            return (char*)"UNKNOWN ERROR";
    }
}


void check_error(cl_int err, const char *operation, char *filename, int line)
{
    if (err != CL_SUCCESS)
    {
        fprintf(stderr, "Error during operation '%s', ", operation);
        fprintf(stderr, "in '%s' on line %d\n", filename, line);
        fprintf(stderr, "Error code was \"%s\" (%d)\n", err_code(err), err);
        exit(EXIT_FAILURE);
    }
}

int main(void)
{

  try
  {
    // Discover number of platforms
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    std::cout << "\nNumber of OpenCL plaforms: " << platforms.size() << std::endl;

    // Investigate each platform
    std::cout << "\n-------------------------" << std::endl;
    for (std::vector<cl::Platform>::iterator plat = platforms.begin(); plat != platforms.end(); plat++)
    {
      std::string s;
      plat->getInfo(CL_PLATFORM_NAME, &s);
      std::cout << "Platform: " << s << std::endl;

      plat->getInfo(CL_PLATFORM_VENDOR, &s);
      std::cout << "\tVendor:  " << s << std::endl;

      plat->getInfo(CL_PLATFORM_VERSION, &s);
      std::cout << "\tVersion: " << s << std::endl;

      // Discover number of devices
      std::vector<cl::Device> devices;
      plat->getDevices(CL_DEVICE_TYPE_ALL, &devices);
      std::cout << "\n\tNumber of devices: " << devices.size() << std::endl;

      // Investigate each device
      for (std::vector<cl::Device>::iterator dev = devices.begin(); dev != devices.end(); dev++ )
      {
        std::cout << "\t-------------------------" << std::endl;

        dev->getInfo(CL_DEVICE_NAME, &s);
        std::cout << "\t\tName: " << s << std::endl;

        dev->getInfo(CL_DEVICE_OPENCL_C_VERSION, &s);
        std::cout << "\t\tVersion: " << s << std::endl;

        int i;
        dev->getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &i);
        std::cout << "\t\tMax. Compute Units: " << i << std::endl;

        size_t size;
        dev->getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &size);
        std::cout << "\t\tLocal Memory Size: " << size/1024 << " KB" << std::endl;

        dev->getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &size);
        std::cout << "\t\tGlobal Memory Size: " << size/(1024*1024) << " MB" << std::endl;

        dev->getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &size);
        std::cout << "\t\tMax Alloc Size: " << size/(1024*1024) << " MB" << std::endl;

        dev->getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &size);
        std::cout << "\t\tMax Work-group Total Size: " << size << std::endl;

        std::vector<size_t> d;
        dev->getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &d);
        std::cout << "\t\tMax Work-group Dims: (";
        for (std::vector<size_t>::iterator st = d.begin(); st != d.end(); st++)
          std::cout << *st << " ";
        std::cout << "\x08)" << std::endl;

        std::string ext;
        dev->getInfo(CL_DEVICE_EXTENSIONS, &ext);
        std::cout << "\t\tExtensions: (" << ext << ")" << std::endl;     
        

        std::cout << "\t-------------------------" << std::endl;

      }

      std::cout << "\n-------------------------\n";
    }

  }
  catch (cl::Error err)
  {
    std::cout << "OpenCL Error: " << err.what() << " returned " << err_code(err.err()) << std::endl;
    std::cout << "Check cl.h for error codes." << std::endl;
    exit(-1);
  }

  return 0;

}
    
