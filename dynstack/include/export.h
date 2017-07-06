/* export.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once


/// Initialize the Stack
/**
*   Calls the setup routine with the steeringcard parameter. The following arguments get forwarded to the stack:
*   \li <b>DYNSTACK X1 ... Xn</b> With \b X the sizes of the stack
*   \li <b>DYNSTACKP X</b>		With \b X as parameter forwarded to setup. The forwarded parameter or sorted in the order of appearance in the steering card
*   \param[in] line Pointer to concatinate arguments given from the steering card
*   \param[in] lengthDim2 Size of every dynstack steering card command in byte
*   \param[in] lengthDim1 Number of dynstack steering card commands
*/
extern "C" void dynstack_init_(const char *line, const int* size);	


/// Dynstack startup
/**
*   Calls the startup routine and sends cummulated parameter to dynstack main routine
*/
extern "C" void dynstack_start_();


/// Terminate Stack
/**
*   Closes the stack and frees the memory
*/
extern "C" void dynstack_end_();



/// To Stack
/**
*   Writes one particle consists out of size doubles to the stack
*   \param[in] par Pointer to particle data
*   \param[in] size Size of the particle array. Only used for error checking, the particle size is already defined in the setup routine.
*/
extern "C" void dynstack_tstout_(const void* par, const int* size, int* ret);


/// From Stack
/**
*   Reads and remove one particle from stack
*   \param[out] par Pointer to particle data thats written to
*   \param[in] size Size of the particle array. Only used for error checking, the particle size is already defined in the setup routine.
*/
extern "C" void dynstack_fstack_(void* par, int* size);


/// Check if the stack is empty
/**
*   Reads the current size of the stack and check if it is zero and writes it to the parameter.
*   \param[out] par Parameter that gets set to true if the stack is empty and false if it still contains data
*/
extern "C" void dynstack_empty_(bool* par);


/// Reset Stack
/**
*   Emptys the stack and reset variables if needed
*/
extern "C" void dynstack_reset_();

					

