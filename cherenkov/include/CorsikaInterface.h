#ifndef _INCLUDE_CORSIKA_INTERFACE_
#define _INCLUDE_CORSIKA_INTERFACE_

#include <crs/CorsikaTypes.h>
#include <crs/CParticle.h>

namespace crs {
  class CInteraction;
}

/**
   \defgroup INTERFACE Interface to CORSIKA 
   \brief C/C++-Interface, to be called from CORSIKA
   
   The interconnection of CORSIKA with COAST is manged by 
   three wrapper functions. COAST offers two standart implementations 
   of this interface:
   - CorsikaMachineIndependent: 
   tries to write standard CORSIKA binary block files without the machine
   dependent fortran write command
   - CorsikaToROOTInterface:
   generates a CORSIKA ROOT output file
   - CorsikaROOTPlotterInterface:
   This is from the ./Examples subdirectory and shows how to make plots
   while running CORSIKA

   To use the interface compile CORSIKA with the ROOTOUT option, and link one
   of the shared object (.so) files to the corsika binary.
   
   \author Ralf Ulrich
   \date Thu Feb  3 13:51:28 CET 2005
   \version $Id: CorsikaInterface.h 6252 2017-09-01 07:39:34Z winchen $
*/


extern "C" {
    
  /**
     \fn void wrida_ (const CREAL *Data)
     \ingroup INTERFACE
     \brief Called from CORSIKA to write a data-subblock

     This is called from the CORSIKA TOBUF function instead from just 
     using a simple fortran WRITE command. 

     \author Ralf Ulrich
     \date Thu Feb  3 13:51:28 CET 2005
     \version $Id: CorsikaInterface.h 6252 2017-09-01 07:39:34Z winchen $
  */
  void wrida_ (const CREAL *DataSubBlock);

    
  /**
     \fn void inida_ (const char *filename, const bool &thinning)
     \ingroup INTERFACE
     \brief Called from CORSIKA to init the ROOT output

     This is called during the initialization of CORSIKA instead from just
     opening a binary output data file. This function also specify if the
     used data structure is thinned or non-thinned.

     \author Ralf Ulrich
     \date Thu Feb  3 13:51:28 CET 2005
     \version $Id: CorsikaInterface.h 6252 2017-09-01 07:39:34Z winchen $
  */
  void inida_ (const char* filename,
	       const int& thinning,
	       const int& curved,
	       const int& slant,
	       const int& stackinput,
	       const int& preshower,
	       int str_length);

  /**
     \fn void interaction_(const char *filename, const bool &thinning)
     \ingroup INTERFACE
     \brief Called from CORSIKA for each (hadronic) interaction

     \author Ralf Ulrich
     \date Tue Sep 22 22:44:53 CEST 2009
     \version $Id: CorsikaInterface.h 6252 2017-09-01 07:39:34Z winchen $
  */
  void interaction_ (const crs::CInteraction& interaction);
    
  /**
     \fn void cloda_ ()
     \ingroup INTERFACE
     \brief Called from CORSIKA to close the ROOT output

     This is called from CORSIKA at the end of a run, to close the ROOT file,
     and clean up memory.

     \author Ralf Ulrich
     \date Thu Feb  3 13:51:28 CET 2005
     \version $Id: CorsikaInterface.h 6252 2017-09-01 07:39:34Z winchen $
  */
  void cloda_ ();


  // COAST++
  void track_ (const crs::CParticle& pre, const crs::CParticle& post); 

  // For CoReas etc.
  int prminfo_(crs::CParticleFortranPtr& p);
  
  void mpinodedone_ ();


	/**
	 * use tabulated atmosphere from gdastool
	 * **/
void tabularizedatmosphere_(const int &nPoints, const double* height, const double* refractivity);

}

#endif
