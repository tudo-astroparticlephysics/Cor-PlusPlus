


bool intersectPlane(const float3& n, const float3& l0, const float3& l, float& t)
{
    float denom = dot(l, n);
	if( denom > 1e-6 )
	{
		float3 p0l0 = -l0;
		t = dot(p0l0, n) / denom;

        return (t >= 0);
	}
    return false;
}



bool intersectDisk(const float3& n, const float& radius2, const float3& l0, const float3& l)
{
    float t = 0;
    if (intersectPlane(n, l0, l, t)) 
    {
        Vec3f p = l0 + l * t;
        Vec3f v = p;
        float d2 = dot(v, v);
        return (d2 <= radius2);
        // or you can use the following optimisation (and precompute radius^2)
        // return d2 <= radius2; // where radius2 = radius * radius
    }

    return false;
} 