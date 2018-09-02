#pragma once

#include <iostream>


#include "math/expression_template.h"

namespace physics
{


    Constants<double> pi(3.1415926);

    Constants<double> alpha( 0.0072973525664 );     //Fine-structure constant

    Constants<double> c( 2.99792458e8 );

    Constants<double> e( 1.602e-19 );




    double mass(int type)
    {
        double mass;
        double charge;

        if ( type >= 100 )
        {
            charge = type%100;
            mass = (0.931494-0.000511)*(int)(type/100); /* nuclear mass */
            switch ( type ) /* more accurate masses for a few nuclei */
            {
                case 402: /* 4He */
                mass = 0.931845*4.;
                break;
            }
        }
        else if ( type > 1 )
        {
            switch ( type )
            {
                case 2: /* positron */
                charge = 1;
                mass = 0.000511;
                break;
                case 5: /* mu+ */
                charge = 1;
                mass = 0.105658;
                break;
                case 8: /* pi+ */
                charge = 1;
                mass = 0.13957;
                break;
                case 11: /* K+ */
                charge = 1;
                mass = 0.493677;
                break;
                case 14: /* proton */
                charge = 1;
                mass = 0.938272;
                break;
                case 3: /* electron */
                charge = -1;
                mass = 0.000511;
                break;
                case 6: /* mu- */
                charge = -1;
                mass = 0.105658;
                break;
                case 9: /* pi- */
                charge = -1;
                mass = 0.13957;
                break;
                case 12: /* K- */
                charge = -1;
                mass = 0.493677;
                break;
                case 15: /* antiproton */
                charge = -1;
                mass = 0.938272;
                break;
                case 13: /* neutron */
                charge = 0;
                mass = 0.939;
                break;
                case 66: /* neutrinos */
                case 67:
                case 68:
                case 69:
                charge = 0;
                mass = 1e-8;
                break;
                default:
                std::cerr << "Primary type %d not supported in get_impact_offset()" << std::endl;
                return -1.0;
            }
        }
        return mass;
    }



    bool gamma_check(double mass, double E, double gamma)
    {
        if ( mass > 0. && fabs(gamma/(E/mass)-1.0) > 1e-2 )
        {
           std::cerr << "Inconsistent gamma factor: " << gamma <<" vs. " << E/mass << ".\n Skipping get_impact_offset()" << std::endl;
           return false;
        }
        return true;
    }

}
