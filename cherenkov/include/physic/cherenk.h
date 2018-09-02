#pragma once

#include "common.h"
#include "physics.h"



long n_dx_photons(const particle& p)
{

    const double beta_start = Variable( std::sqrt(SQRT( (1.0-(mass/energy_start))*(1.0+(mass/energy_start)) ) ) );
    const double beta_end = Variable( std::sqrt(SQRT( (1.0-(mass/energy_end))*(1.0+(mass/energy_end)) ) ) );

    const double charge = Variable( p.charge );

    const double n = atmosphere(p.z);

    //
    cphoton_dx = 2 * physics::pi * physics::alpha * (charge*charge) * (1- (1.0 / beta^2 * n^2)) * (1/lambda_low - (1/lambda_high));




    return cphoton_dx;
}
