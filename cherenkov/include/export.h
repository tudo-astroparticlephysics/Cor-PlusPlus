/* export.h
* this file is part of Cherenkov for CORSIKA
*
* Copyright (C) <2017> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include "ocl/OpenCL_Hardware.h"


/// Initialize the Stack
/**
*   Calls the setup routine with the steeringcard parameter. The following arguments get forwarded to the stack:
*/
extern "C" void cherenkov_init_(const char *line, const int* size);


/// Cherenkov routine startup
/**
*   Calls the startup routine and sends cummulated parameter to cherenkov main routine
*/
extern "C" void cherenkov_start_();


/// Terminate cherenkov routines
/**
*   Closes the stack and frees the memory
*/
extern "C" void cherenkov_end_();


/*
C  C(H)ERENK(OV RADIATION FROM ALL KINDS OF CHARGED PARTICLES)
C
C  CREATION OF CHERENKOV PHOTONS ALONG THE TRACKS OF CHARGED PARTICLES.
C  CHERENKOV RADIATION IS ONLY CALCULATED FOR THE LOWEST OBSERVATION
C  LEVEL. ALL PARAMETERS OF THE PARTICLE TRACK STEP ARE PASSED AS
C  ARGUMENTS.
C  THIS SUBROUTINE IS CALLED FROM ELECTR AND UPDATE.
C  ARGUMENTS (ALL DOUBLE PRECISION):
C   STEPCR = STEP LENGTH FOR THE PARTICLE [CM]
C   UMEAN  = DIRECTION COSINE TO X AXIS (STEP AVERAGE)
C   VMEAN  = DIRECTION COSINE TO Y AXIS (STEP AVERAGE)
C   WMEAN  = DIRECTION COSINE TO -Z AXIS (STEP AVERAGE)
C   EBEG   = ENERGY [GEV] AT BEGINNING OF STEP
C   EEND   = ENERGY [GEV] AT END OF STEP
C   XBEG   = X POSITION [CM] AT BEGINNING OF STEP
C   YBEG   = Y POSITION [CM] AT BEGINNING OF STEP
C   ZBEG   = Z POSITION [CM] AT BEGINNING OF STEP
C   XEND   = X POSITION [CM] AT END OF STEP
C   YEND   = Y POSITION [CM] AT END OF STEP
C   ZEND   = Z POSITION [CM] AT END OF STEP
C   TBEG   = TIME [NSEC] AT BEGIN OF STEP
C   TEND   = TIME [NSEC] AT END OF STEP
C   AMASS  = PARTICLE MASS [GEV/C**2]
C   CHARGE = CHARGE NUMBER (OR NEGATIVE - WE NEED ONLY THE SQUARE OF IT)
C   WTTHIN = PARTICLE WEIGHT FOR THINNING VERSION, ELSE 1.
C   CTEA   = COSINE OF EARTH ANGLE IN CURVED VERSION, ELSE 1.
*/
extern "C" void parallel_cerenk(double STEPCR, double UMEAN, double VMEAN, double WMEAN, double EBEG, double EEND, double XBEG, double YBEG,
		          double ZBEG, double XEND, double YEND, double ZEND, double TBEG, double TEND, double AMASS, double CHARGE, double WTTHIN, double CTEA);
