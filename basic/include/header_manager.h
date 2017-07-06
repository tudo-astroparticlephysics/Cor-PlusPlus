/* header_manger.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/


#pragma once

#include <memory>
#include <functional>
#include <vector>


#include "lib/data/corsika/event_header.h"
#include "lib/data/corsika/event_end.h"
#include "lib/data/corsika/run_header.h"
#include "lib/data/corsika/run_end.h"

using namespace lib::data;


class HeaderManager
{
private:

	friend void baack_modify_evth_(float* p_evth, const int* p_size);
	friend void baack_modify_evte_(float* p_evth, const int* p_size);
	friend void baack_modify_runh_(float* p_evth, const int* p_size);
	friend void baack_modify_rune_(float* p_evth, const int* p_size);

protected:


	std::vector< std::function<void(EventHeader&)>> m_oCallbackEVTH;
	std::vector< std::function<void(EventEnd&)>> m_oCallbackEVTE;
	std::vector< std::function<void(RunHeader&)>> m_oCallbackRUNH;
	std::vector< std::function<void(RunEnd&)>> m_oCallbackRUNE;

	HeaderManager(const HeaderManager& rhs) = delete;
	HeaderManager();

public:
	HeaderManager(HeaderManager&& rhs);

	static HeaderManager& getInstance();


	void register_evth_callback( std::function<void(EventHeader&)> p_function );
	void register_evte_callback( std::function<void(EventEnd&)> p_function );
	void register_runh_callback( std::function<void(RunHeader&)> p_function );
	void register_rune_callback( std::function<void(RunEnd&)> p_function );
};


inline HeaderManager& SHeaderManager()
{
	return HeaderManager::getInstance();
}


