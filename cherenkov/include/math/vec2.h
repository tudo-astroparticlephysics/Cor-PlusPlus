#pragma once



struct vec2
{
    double x;
    double y;

    vec2(double px, double py)
        :x(px), y(py)
    {
    }

    vec2()
        :x(0.0), y(0.0)
    {
    }

    inline vec2 operator+(const vec2& rhs) const
    {
        return vec2(x + rhs.x, y + rhs.y);
    }

    inline vec2 operator-(const vec2& rhs) const
    {
        return vec2(x - rhs.x, y - rhs.y);
    }

    inline vec2 operator*(const double& rhs) const
    {
        return vec2(x * rhs, y * rhs);
    }

    inline vec2 operator/(const double& rhs) const
    {
        return vec2(x / rhs, y / rhs);
    }

    inline vec2 operator-() const
    {
        return vec2(-x, -y);
    }

    inline double sum() const
    {
        return x + y;
    }

};
