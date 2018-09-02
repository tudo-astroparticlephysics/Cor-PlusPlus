


#include "random/xorshift.cl"
#include "random/pcg.cl"

#include "random/general.cl"

typedef union{
	ulong ul_type;
	double df_type;
	unsigned int ui_type;
	float f_type;
} type_conversion;




__kernel void random( __global float* output)
{
	const int ID = get_global_id(0);
	const int group = get_group_id(0);
	const int localID = get_local_id(0);
	const int localSize = 	(0);
	
	if( ID > get_global_size(0) )
	{
		return;
	}

	/*  Local Memory for this Kernel
	 *
	 */
	
	// Init local memory
	if(localID == 0)
	{
		
	}
	barrier(CLK_LOCAL_MEM_FENCE);

    xorwow_state rng_seed;
	rng_seed.s0 = 123456789;
    rng_seed.s1 = 362436069;
    rng_seed.s2 = 521288629;
	rng_seed.s3 = 88675123;
	rng_seed.s4 = 314159265;
	
	for(int i=0; i<500; i++)
	{
		xorwow(&rng_seed);
	}

    type_conversion tmp;
	tmp.ui_type = xorwow(&rng_seed);
	output[ID] = tmp.f_type/FLT_MAX;
	

	printf("For: %i  -> %i, %i | %i ( %f | %f)\n", ID, rng_seed.s1, rng_seed.s2, tmp.ui_type, tmp.f_type, uint_to_small_float(tmp.ui_type) );

    return;
}