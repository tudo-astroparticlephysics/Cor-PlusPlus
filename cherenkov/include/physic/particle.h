#pragma once

#include "math/vec3.h"
#include "math/vec4.h"

struct particle
{
    vec4 m_s;
    vec4 m_e;

    double m_mass;
    double m_beta;
    double m_charge;

    double buffer;

    particle()
    {
        
    }

    particle(vec3 s, vec3 e, double mass, double beta, double charge)
    {
        m_s = s;
        m_beta = beta;
        m_e = e;
        m_mass = mass;
        m_charge = charge;
    }
};
