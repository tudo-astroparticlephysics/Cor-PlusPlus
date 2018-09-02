

#include <memory>

#include "export.h"

#include "physic/constants.h"
#include "physic/particle.h"



/*-----------------------------------------------------------------------
*  C(H)ERENK(OV RADIATION FROM ALL KINDS OF CHARGED PARTICLES)
*
*  CREATION OF CHERENKOV PHOTONS ALONG THE TRACKS OF CHARGED PARTICLES.
*  CHERENKOV RADIATION IS ONLY CALCULATED FOR THE LOWEST OBSERVATION
*  LEVEL. ALL PARAMETERS OF THE PARTICLE TRACK STEP ARE PASSED AS
*  ARGUMENTS.
*  THIS SUBROUTINE IS CALLED FROM ELECTR AND UPDATE.
*  ARGUMENTS (ALL DOUBLE PRECISION):
*   STEPCR = STEP LENGTH FOR THE PARTICLE [CM]
*   UMEAN  = DIRECTION COSINE TO X AXIS (STEP AVERAGE)
*   VMEAN  = DIRECTION COSINE TO Y AXIS (STEP AVERAGE)
*   WMEAN  = DIRECTION COSINE TO -Z AXIS (STEP AVERAGE)
*   EBEG   = ENERGY [GEV] AT BEGINNING OF STEP
*   EEND   = ENERGY [GEV] AT END OF STEP
*   XBEG   = X POSITION [CM] AT BEGINNING OF STEP
*   YBEG   = Y POSITION [CM] AT BEGINNING OF STEP
*   ZBEG   = Z POSITION [CM] AT BEGINNING OF STEP
*   XEND   = X POSITION [CM] AT END OF STEP
*   YEND   = Y POSITION [CM] AT END OF STEP
*   ZEND   = Z POSITION [CM] AT END OF STEP
*   TBEG   = TIME [NSEC] AT BEGIN OF STEP
*   TEND   = TIME [NSEC] AT END OF STEP
*   AMASS  = PARTICLE MASS [GEV/C**2]
*   CHARGE = CHARGE NUMBER (OR NEGATIVE - WE NEED ONLY THE SQUARE OF IT)
*   WTTHIN = PARTICLE WEIGHT FOR THINNING VERSION, ELSE 1.
*   CTEA   = COSINE OF EARTH ANGLE IN CURVED VERSION, ELSE 1.
----------------------------------------------------------------------- */

void CERENK( double step_length, vec3 dir, double EBEG, double EEND, vec3 start ,vec3 end, double TBEG, double TEND, double AMASS, double CHARGE, double WTTHIN, double CTEA)
{
    vec3 mov =  (end - start);
    float length = mov.length();


    float fPartitions = length / step_length;
    int iPartitions = static_cast<int>(fPartitions);

    mov = mov / fPartitions;
    vec3 cur = start;

    std::unique_ptr< particle[] > buffer( new particle[iPartitions+1] );

    for(int i=0; i < iPartitions; i++)
    {
        buffer[i] = particle(cur, cur + mov, AMASS, 0, CHARGE);

        cur = cur + mov;
    }

    std::cout << "Add particle: " << AMASS << " with " << CHARGE << "Charge and beta: " << CTEA << std::endl;


}
