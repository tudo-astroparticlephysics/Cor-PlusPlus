
//#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable
//#pragma OPENCL EXTENSION cl_khr_fp64 : enable

#include "random/general.cl"
#include "random/xorshift.cl"

#include "geometry.cl"

#define PI 3.1415926
#define ALPHA 0.007297352 // Fine-structure constant

typedef struct
{
	double4 start;
	double4 end;

	double m_mass;
	double m_beta;
	double m_charge;
	double buffer;

} particleStruct;

__kernel void photon_short(__global particleStruct *input_track, __global unsigned short *output_N) //, __global photon* output_photon)
{
	const int ID = get_global_id(0);
	const int group = get_group_id(0);
	const int localID = get_local_id(0);
	const int localSize = (0);

	if (ID > get_global_size(0))
	{
		return;
	}

	/*  Local Memory for this Kernel
	 *
	 */

	__local unsigned int photons;
	__local unsigned int photonCount[32];
	__local int i;
	__local int j;

	// Init local memory
	if (localID == 0)
	{
		photons = 0;
		i = 0;
	}
	barrier(CLK_LOCAL_MEM_FENCE);

	// Global constants
	__constant const double lambda1 = 200;																			  //nm
	__constant const double lambda2 = 900;																			  //nm
	__constant const double n = 1.000292;																			  // Brechungsindex
	__constant const double factor = (10.0 * 1000.0 * 1000.0) * (2.0 * PI * ALPHA * (1.0 / lambda1 - 1.0 / lambda2)); // 1/cm

	// Copy input stage
	particleStruct particle = input_track[ID];

	/**
	*  Get photon number -> Currently no statistic fluctation necessary
	*/
	const double theta = acos(1.0 / (n * particle.m_beta));
	const double dx = factor * particle.m_charge * particle.m_charge * sin(theta) * sin(theta);

	double length = distance(particle.start, particle.end);

	unsigned int tmpPhoton = dx * length;
	photonCount[localID] = tmpPhoton;
	atomic_add(&photons, tmpPhoton); // Slow but maybe ok --> Tree add is better

	// Debug Info
	if (ID == 0 && true)
	{
		printf("Job %i : %i Photons over %f units with (dx: %f, theta: %f) \n", ID, photonCount, length, dx, theta);
		printf("Factor: %f\n", factor);

		printf("Start: %f, %f, %f\n", particle.start.x, particle.start.y, particle.start.z);
		printf("End: %f, %f, %f\n", particle.end.x, particle.end.y, particle.end.z);
		printf("Mass:%f, Beta:%f, Charge:%f\n", particle.m_mass, particle.m_beta, particle.m_charge);

		printf(" --> Output: %i\n", *output_N);
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	while (j < 32)
	{
		double4 dir = particle[j].end - particle[j].start;
		double4 photon = particle[j].start + 0.5f * dir;

		while (true)
		{
			int workID = atomic_inc(i);
			if (workID < photonCount[j] )
			{
				float phi = 2.0f * PI;  // 0 to 2pi
								// theta calculated above
								// r = 1

				float4 p = (float4)( sin(theta)*cos(phi),
									 sin(theta)*sin(phi),
									 cos(theta),
									 0.0f);

														  // 10km
				if( intersectDisk((float3)(0.0f, 1.0f, 0.0f), 10000*100, photon, p.xyz) )
				{
					// store photon into global memory

				}
			}
			else if (workID == photonCount[j] )
			{
				atomic_inc(j);
				break;
			}
			else
			{
				break;
			}
		}
		atomic_inc(j);
		
	}

	return;
}