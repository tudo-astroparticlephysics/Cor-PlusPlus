/* export.h
 * this file is part of RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

 /// Function that handles steeringcard parameter
 /**
 *
 */
extern "C" void remotecontrol_init_(const char* line, const int* size);

/// Initializing function get called at the beginning of CORSIKA
/**
 *
 */
extern "C" void remotecontrol_start_();


//@{
/// Cleanup function get called at the termination of CORSIKA
/**
 *
 */
extern "C" void remotecontrol_end_();


//@{
/// Fixed structure callbacks to recieve information from CORSIKA
/**
 *  Get called from CORSIKA with the complete header information. The data stored should not be changed in any way!
 *  \param data Array of 273 floats containing the header information
 */
extern "C" void remotecontrol_push_runh_(const float* data);

extern "C" void remotecontrol_push_rune_(const float* data);

extern "C" void remotecontrol_push_evth_(const float* data);

extern "C" void remotecontrol_push_evte_(const float* data);
//@}


/// Callback to receive parameter of the initial particle from CORSIKA
/**
 *  \param data Particle information wrapped into the normal CORSIKA particle data format
 *  \param len Length of particle structure in byte for different possible settings of CORSIKA
 *  		   Checks if settings in setup matches the actually used data
 */
extern "C" void remotecontrol_push_initalparticle_(const float* data, unsigned int len);


/// Receive information for the initial particle from remote server
/**
 *  \param data Particle information wrapped into the normal CORSIKA particle data format
 *  \param len Length of particle structure in byte for different possible settings of CORSIKA
 *  \return Return blocks for specific time set in setup if no setting is stored in buffer otherwise it returns immediately
 */
extern "C" void remotecontrol_recv_initalparticle_(const float* data);


/// Function that exposes raw packet managing directly to CORSIKA
/**
 *  \param id Message ID of the packet to send
 *  \param data Array of data to send. Gets copied and added to the send buffer
 *  \param len Length in byte of the data array
 *  \return Function returns immediately
 */
extern "C" void remotecontrol_send_(const int ID, const void* data, int len);


/// Function that reads input card parameter from a webserver or fallback file
/**
 *  \param line Byte string to write a single setting of the steering file
 *  \param length Length of the line array in elements. It get changed to the number of bytes written to the array
 */
extern "C" void remotecontrol_steering_nextline_(char* line, int* lineLength, int* length);



