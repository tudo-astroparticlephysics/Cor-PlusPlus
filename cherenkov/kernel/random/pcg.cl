// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)


// http://www.pcg-random.org/download.html


//Floats are hard!
//http://mumble.net/~campbell/tmp/random_real.c


typedef struct 
{
    ulong state;
    ulong inc; 
} pcg32_random_t;



unsigned int pcg32_random(pcg32_random_t* rng)
{
    ulong oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

void pcg32_seed_random(pcg32_random_t* rng, ulong initstate, ulong initseq)
{
    rng->state = 0U;
    rng->inc = (initseq << 1u) | 1u;
    pcg32_random(rng);
    rng->state += initstate;
    pcg32_random(rng);
}

