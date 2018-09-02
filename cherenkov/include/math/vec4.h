#pragma once

#include <cmath>

#include "vec3.h"

struct vec4
{
    double x;
    double y;
    double z;
    double w;

    vec4(double px, double py, double pz, double pw)
        :x(px), y(py), z(pz), w(pw)
    {
    }

    vec4()
        :x(0.0), y(0.0), z(0.0), w(0.0)
    {
    }

    vec4( const vec3& rhs)
        :x(rhs.x), y(rhs.y), z(rhs.z), w(0.0)
    {

    }

    inline vec4 operator+(const vec4& rhs) const
    {
        return vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    inline vec4 operator-(const vec4& rhs) const
    {
        return vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    inline vec4 operator*(const double& rhs) const
    {
        return vec4(x * rhs, y * rhs, z * rhs, w * rhs);
    }

    inline vec4 operator/(const double& rhs) const
    {
        return vec4(x / rhs, y / rhs, z / rhs, w/rhs);
    }

    inline vec4 operator-() const
    {
        return vec4(-x, -y, -z, -w);
    }

    inline double sum() const
    {
        return x + y + z + w;
    }

    inline double length() const
    {
        return std::sqrt( x*x + y*y + z*z + w*w);
    }

};
