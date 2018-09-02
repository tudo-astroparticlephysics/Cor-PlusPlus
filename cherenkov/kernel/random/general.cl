float uint_to_small_float(uint rng)
{
    return convert_float( ldexp( convert_double(rng), -32) );
}