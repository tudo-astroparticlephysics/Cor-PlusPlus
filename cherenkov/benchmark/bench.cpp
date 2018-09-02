#include <iostream>

#include <functional>
#include <memory>
#include <random>

#include "physic/constants.h"
#include "physic/particle.h"

#include "ocl/OpenCL_Program.h"
#include "ocl/OpenCL_Hardware.h"
#include "ocl/OpenCL_Queue.h"

#include <chrono>

#define BLOCK_SIZE 10000000

int main( int argc, char* argv[] )
{
    std::mt19937_64 generator( 1337 );
    std::uniform_real_distribution< double > start_dist( 10000, 15000 );
    std::uniform_real_distribution< double > end_dist( 4000, 10000 );

    auto start = std::bind( start_dist, generator );
    auto end   = std::bind( end_dist, generator );

    std::cout << "Start particle generation" << std::endl;

    std::vector< particle > par;
    par.resize( BLOCK_SIZE );

    for( int i = 0; i < BLOCK_SIZE; i++ )
    {
        vec3 s( 0.0, 0.0,  start() );
        vec3 e( 0.0, 0.0,  end() );
                                     // 1TeV Proton:  0.99999956
        par[i] = particle( s, e, physics::mass( 14 ), 0.9999996, 1 );

    }

    std::cout << "-------> Start: " << par[0].m_s.x << " " << par[0].m_s.y << " " << par[0].m_s.z << std::endl;
    std::cout << "-------> End:   " << par[0].m_e.x << " " << par[0].m_e.y << " " << par[0].m_e.z << std::endl;
    std::cout << "------->Local Beta: " << par[0].m_beta << std::endl;

  
    std::vector< particle > photon;

    auto hardware = std::unique_ptr< OCL_Hardware >( new OCL_Hardware );

    auto program = std::unique_ptr< OCL_Program >( new OCL_Program( "./create_photon.cl" ) );
    program->load( hardware->getDevice(), hardware->getContext() );
    program->info();

    cl::Kernel photon_generation(program->getProgram(), "photon_short");
    //photon_generation.setArg(1, 1337);

    cl::Buffer buffer_A(hardware->getContext(), CL_MEM_WRITE_ONLY, BLOCK_SIZE * sizeof(particle));
    cl::Buffer buffer_B(hardware->getContext(), CL_MEM_READ_WRITE, BLOCK_SIZE * sizeof(unsigned short));

    auto queue = std::unique_ptr< OCL_Queue >( new OCL_Queue( hardware->getContext(), hardware->getDevice() ) );
    
    queue->getQueue().enqueueWriteBuffer(buffer_A, CL_TRUE, 0, BLOCK_SIZE * sizeof(particle), par.data());
    photon_generation.setArg(0, buffer_A);
    photon_generation.setArg(1, buffer_B);
   
    
    auto start_time = std::chrono::high_resolution_clock::now();

    queue->getQueue().enqueueNDRangeKernel(photon_generation, 0, BLOCK_SIZE, 256);

    auto timer = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - start_time);
    std::cout << "For " << " :  " << timer.count() << "\n";
    
    
    auto photonArr = std::unique_ptr<unsigned short[]>(new unsigned short [BLOCK_SIZE]);
    unsigned long numPhotons = 0;
    queue->getQueue().enqueueReadBuffer(buffer_B, CL_TRUE, 0, BLOCK_SIZE * sizeof(unsigned short), photonArr.get());
    queue->getQueue().finish();

    for(int i=0; i<BLOCK_SIZE; i++)
    {       
        numPhotons += photonArr[i];
    }

    std::cout << "Number of photons: " << numPhotons << std::endl;


    return 0;
}
