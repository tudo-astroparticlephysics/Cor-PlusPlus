#pragma once

#include <cmath>



template<class T>
class Constants
{
private:
    const T data;

public:
    Constants(T p) : data(p){}

    T operator()()
    {
        return static_cast<T>(data);
    }

    T& operator=(const T& rhs) = delete;

    inline bool operator==(const T& rhs){ return data == rhs; }
    inline bool operator!=(const T& rhs){ return !(data == rhs); }

    inline bool operator< (const T& rhs){ return data < rhs; }
    inline bool operator> (const T& rhs){ return data > rhs; }
    inline bool operator<=(const T& rhs){ return data <= rhs; }
    inline bool operator>=(const T& rhs){ return data >= rhs; }

};


template<class T>
class Variable
{
private:
    T data;

public:
    Variable(T p) : data(p){}

    T& operator()()
    {
        return static_cast<T>(data);
    }

    T& operator=(const T& rhs) { return data = rhs;};

    inline bool operator==(const T& rhs){ return data == rhs; }
    inline bool operator!=(const T& rhs){ return !(data == rhs); }

    inline bool operator< (const T& rhs){ return data < rhs; }
    inline bool operator> (const T& rhs){ return data > rhs; }
    inline bool operator<=(const T& rhs){ return data <= rhs; }
    inline bool operator>=(const T& rhs){ return data >= rhs; }


    T& operator^(int power){return std::pow(data, power);}
    T& operator^(double power){return std::pow(data, power);}

};
