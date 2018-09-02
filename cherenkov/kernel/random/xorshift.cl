; // nicht komplett mit 0 initialisieren





typedef struct
{
	ulong s[16];
	int p;
   
} xorshift1024star_state;

ulong xorshift1024star(xorshift1024star_state* state) 
{
	const ulong s0 = state->s[state->p++];
	ulong s1 = state->s[state->p &= 15];
	s1 ^= s1 << 31; // a
	s1 ^= s1 >> 11; // b
	s1 ^= s0 ^ (s0 >> 30); // c
	state->s[state->p] = s1;
	return s1 * (unsigned long)1181783497276652981;
}



typedef struct
{
    ulong s1;
    ulong s2;
} xorshift128plus_state;

ulong xorshift128plus(xorshift128plus_state* state) {
	ulong x = state->s1;
	ulong const y = state->s2;
	state->s1 = y;
	x ^= x << 23; // a
	state->s2 = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
	return state->s2 + y;
}


typedef struct
{
    unsigned int s0;
    unsigned int s1;
	unsigned int s2;
	unsigned int s3;
	unsigned int s4;
} xorwow_state;

unsigned int xorwow(xorwow_state* state)
{
	/* Algorithmus "xorwow" von S. 5 in Marsaglia, "Xorshift RNGs" */
	unsigned int s, t = state->s3;
	t ^= t >> 2;
	t ^= t << 1;
	state->s3 = state->s2;
	state->s2 = state->s1;
	state->s1 = s = state->s0;
	t ^= s;
	t ^= s << 4;
	state->s0 = t;
	return t + (state->s4 += 362437);
}