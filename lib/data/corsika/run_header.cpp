/* run_header.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "lib/data/corsika/run_header.h"

namespace lib
{
	namespace data
	{

		RunHeader::RunHeader(float* p_evth)
				: m_fMemory(p_evth), m_bOwnsMemory(false)
		{
		}

		RunHeader::RunHeader(const RunHeader& p_rhs)
				: m_fMemory(new float[273]), m_bOwnsMemory(true)
		{
			for (int i = 0; i < 273; i++)
			{
				m_fMemory[i] = p_rhs.m_fMemory[i];
			}
		}

		RunHeader::RunHeader(RunHeader&& p_rhs)
				: m_fMemory(p_rhs.m_fMemory), m_bOwnsMemory(p_rhs.m_bOwnsMemory)
		{
		}

		RunHeader::~RunHeader()
		{
			if (m_bOwnsMemory)
			{
				if (m_fMemory != nullptr)
				{
					delete[] m_fMemory;
				}
			}
			return;
		}

	}
}
