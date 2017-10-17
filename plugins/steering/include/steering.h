/* export.h
* this file is part of CorPlusPlus
*
* Copyright (C) <2017> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/
#pragma once

#include <string>


class Steering
{
private:

public:

	virtual ~Steering() = 0;

	virtual std::string operator[](std::string) = 0;

};









