#pragma once

#include <cmath>

struct vec3
{
    double x;
    double y;
    double z;

    vec3(double px, double py, double pz)
        :x(px), y(py), z(pz)
    {
    }

    vec3()
        :x(0.0), y(0.0), z(0.0)
    {
    }

    inline vec3 operator+(const vec3& rhs) const
    {
        return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    inline vec3 operator-(const vec3& rhs) const
    {
        return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    inline vec3 operator*(const double& rhs) const
    {
        return vec3(x * rhs, y * rhs, z * rhs);
    }

    inline vec3 operator/(const double& rhs) const
    {
        return vec3(x / rhs, y / rhs, z / rhs);
    }

    inline vec3 operator-() const
    {
        return vec3(-x, -y, -z);
    }

    inline double sum() const
    {
        return x + y + z;
    }

    inline double length() const
    {
        return std::sqrt( x*x + y*y + z*z);
    }

};
