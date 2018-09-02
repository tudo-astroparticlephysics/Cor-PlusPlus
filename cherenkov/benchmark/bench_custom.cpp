#include <iostream>

#include <functional>
#include <memory>
#include <random>

#include "physic/constants.h"
#include "physic/particle.h"

#include "ocl/OpenCL_Hardware.h"
#include "ocl/OpenCL_Kernel.h"
#include "ocl/OpenCL_Program.h"
#include "ocl/OpenCL_Queue.h"

int main( int argc, char* argv[] )
{
    std::mt19937_64 generator( 1337 );
    std::uniform_real_distribution< double > start_dist( 10000, 15000 );
    std::uniform_real_distribution< double > end_dist( 4000, 10000 );

    auto start = std::bind( start_dist, generator );
    auto end   = std::bind( end_dist, generator );

    std::cout << "Start particle generation" << std::endl;

    std::vector< particle > par;
    par.resize( static_cast< long >( 32 ) );

    for( int i = 0; i < par.size(); i++ )
    {
        vec3 s( 0.0, 0.0, start() );
        vec3 e( 0.0, 0.0, end() );

        par[i] = particle( s, e, physics::mass( 14 ), 0.9999991, 1 );

    }

    std::cout << "-------> Start: " << par[0].m_s.x << " " << par[0].m_s.y << " " << par[0].m_s.z << std::endl;
    std::cout << "-------> End:   " << par[0].m_e.x << " " << par[0].m_e.y << " " << par[0].m_e.z << std::endl;
    std::cout << "------->Local Beta: " << par[0].m_beta << std::endl;

    unsigned long photonCount;
    std::vector< particle > photon;

    auto hardware = std::unique_ptr< OCL_Hardware >( new OCL_Hardware );

    std::cout << "Load Program" << std::endl;

    auto program = std::unique_ptr< OCL_Program >( new OCL_Program( "./create_photon.cl" ) );
    program->load( hardware->getDevice(), hardware->getContext() );
    program->info();

    std::cout << "Load Queue" << std::endl;

    auto queue = std::unique_ptr< OCL_Queue >( new OCL_Queue( hardware->getContext(), hardware->getDevice() ) );

    std::cout << "Load Kernel" << std::endl;

    auto kernel = std::unique_ptr< OCL_Kernel >( new OCL_Kernel( "photon_short" ) );
    kernel->createKernel( program->getProgram() );

    std::cout << "Info:" << std::endl;
    kernel->info();

    std::cout << "Add Buffer" << std::endl;

    auto numPhotons = std::unique_ptr<unsigned long>(new unsigned long);
    *numPhotons = 0;

    kernel->addBuffer< particle >( hardware->getContext(), CL_MEM_READ_ONLY, 0, par );
    kernel->addBuffer( hardware->getContext(), CL_MEM_READ_WRITE, 1, reinterpret_cast<void*>(numPhotons.get()), sizeof(unsigned long));
    
    kernel->writeData( queue->getQueue() );

    cl_int error = queue->getQueue().enqueueNDRangeKernel(kernel->getKernel(), 0, par.size(), 32);

    queue->getQueue().finish();
    
    kernel->readData(queue->getQueue(), 1);

    unsigned long tmp = kernel->readBuffer<unsigned long>(1);

    queue->getQueue().finish();

    std::cout << "Number of photons: " << *numPhotons << " ... " << tmp << std::endl;


    return 0;
}
