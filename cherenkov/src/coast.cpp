


#include "interface/CorsikaInterface.h"


#include "crs/CParticle.h"
#include "crs/CInteraction.h"

#include <sstream>
#include <iostream>


/*
  Data is one CORSIKA data-block constining of 21 SubBlocks.
  A SubBlock can be:
  - thinned mode:     39 (Particles) * 8 (ENTRIES) * 4 (BYTES) 
  - not-thinned mode: 39 (Particles) * 7 (ENTRIES) * 4 (BYTES) 
*/
void
wrida_ (const CREAL *Data) 
{    

}




void
inida_ (const char* filename,
        const int& thinning,
        const int& /*curved*/,
        const int& /*slant*/,
        const int& /*stackinput*/,
        const int& /*preshower*/,
        int str_length) 
{
	// Init
}

void
cloda_ () 
{
  // Close
}


void 
interaction_ (const crs::CInteraction& interaction)
{
  /*
    all interactions in the shower are available in this function ! 
    
    the information availabel in the CInteraction class are:
    
    double x;
    double y;
    double z;      
    double etot;      // lab energy
    double sigma;     // cross-section of process
    double kela;      // elasticity
    int    projId;    // projectile
    int    targetId;  // target
    double time;
  */
}


void
track_ (const crs::CParticle& pre, const crs::CParticle& post)
{
  /*
    all particles in the shower are available in this function !
    
    The pre and post objecte are the two endpoints for one single track 
    in the shower, where the information available in CParticle is:

    double x;
    double y;
    double z;
    double depth;
    double time;
    double energy;
    double weight;
    int    particleId;
    int    hadronicGeneration;
  */




} 


