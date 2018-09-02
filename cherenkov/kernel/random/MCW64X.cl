


//! Represents the state of a particular generator
typedef struct{ uint x; uint c; } mwc64x_state_t;

enum{ MWC64X_A = 4294883355U };
enum{ MWC64X_M = 18446383549859758079UL };
enum{ MWC64X_B = 4077358422479273989UL };



// Pre: a < M, b < M
// Post: r = (a + b) mod M
ulong AddMod64(ulong a, ulong b, ulong M)
{
	ulong v = a + b;
	if (v >= M || v < a) v -= M;
	return v;
}


// Pre: a < M, b < M
// Post: r = (a * b) mod M
ulong MulMod64(ulong a, ulong b, ulong M)
{
	ulong r = 0;
	while (a)
	{
		if (a & 1) r = AddMod64(r, b, M);
		b = AddMod64(b, b, M);
		a = a >> 1;
	}
	return r;
}

// Pre: a < M, e >= 0
// Post: r = (a ^ b) mod M
// This takes at most ~64^2 modular additions, so probably about 2^15 or so instructions on
// most architectures
ulong PowMod64(ulong a, ulong e, ulong M)
{
	ulong sqr = a, acc = 1;
	while (e)
	{
		if (e & 1) acc = MulMod64(acc, sqr, M);
		sqr = MulMod64(sqr, sqr, M);
		e = e >> 1;
	}
	return acc;
}



float MWC64X_Step(mwc64x_state_t *s)
{
	uint X=s->x, C=s->c;
	uint r = X^C;
	
	uint Xn=MWC64X_A*X+C;
	uint carry=(uint)(Xn<C);				// The (Xn<C) will be zero or one for scalar
	uint Cn=mad_hi(MWC64X_A,X,carry);  
	
	s->x=Xn;
	s->c=Cn;

	return r;
}


void  MWC64X_seed(mwc64x_state_t *s, ulong baseOffset, ulong perStreamOffset)
{
	ulong d = baseOffset + get_global_id(0) * perStreamOffset;
	ulong m = PowMod64(MWC64X_A, d, MWC64X_M);
	ulong x = MulMod64(MWC64X_B, m, MWC64X_M);
	s->x = x / MWC64X_A;
	s->c = x % MWC64X_A;
}