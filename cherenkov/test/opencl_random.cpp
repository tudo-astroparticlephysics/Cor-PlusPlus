
    /*
 * Display Device Information
 *
 * Script to print out some information about the OpenCL devices
 * and platforms available on your system
 *
 * History: C++ version written by Tom Deakin, 2012
 *          Updated by Tom Deakin, August 2013
*/


#include "ocl/OpenCL_Program.h"
#include "ocl/OpenCL_Hardware.h"
#include "ocl/OpenCL_Queue.h"


#include "ocl/cl2.cpp"

#include <memory>
#include <iomanip>


#define BLOCK_SIZE 64



int main(void)
{

    std::vector<float> random_data;
    random_data.resize(BLOCK_SIZE);


    auto hardware = std::unique_ptr< OCL_Hardware >( new OCL_Hardware );

    auto program = std::unique_ptr< OCL_Program >( new OCL_Program( "./random_test.cl" ) );
    program->load( hardware->getDevice(), hardware->getContext() );
    program->info();

    cl::Kernel random(program->getProgram(), "random");
    //photon_generation.setArg(1, 1337);
    
    cl::Buffer buffer(hardware->getContext(), CL_MEM_READ_WRITE, BLOCK_SIZE * sizeof(float));

    auto queue = std::unique_ptr< OCL_Queue >( new OCL_Queue( hardware->getContext(), hardware->getDevice() ) );
   
    std::cout << "Error setArg: " << random.setArg(0, buffer) << std::endl;
   
    
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Error queue: " << queue->getQueue().enqueueNDRangeKernel(random, 0, BLOCK_SIZE, 32) << std::endl;

    

    auto time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - start);
    std::cout << "Execution time " << " :  " << time.count() << "\n";
    
    
    queue->getQueue().enqueueReadBuffer(buffer, CL_TRUE, 0, BLOCK_SIZE * sizeof(float), random_data.data());
    queue->getQueue().finish();

    for(int i=0; i<BLOCK_SIZE; i++)
    {
        std::cout << "ID: " << i << "  -> " << std::setprecision(16) << random_data[i] << std::endl;
    }




  return 0;

}
    
