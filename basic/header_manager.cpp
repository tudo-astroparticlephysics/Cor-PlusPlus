/* header_manager.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "basic/header_manager.h"

HeaderManager::HeaderManager()
{
}

HeaderManager::HeaderManager(HeaderManager&& rhs)
{
	this->m_oCallbackEVTH = std::move(rhs.m_oCallbackEVTH);
	this->m_oCallbackEVTE = std::move(rhs.m_oCallbackEVTE);
	this->m_oCallbackRUNH = std::move(rhs.m_oCallbackRUNH);
	this->m_oCallbackRUNE = std::move(rhs.m_oCallbackRUNE);
}

HeaderManager& HeaderManager::getInstance()
{
	static HeaderManager singelton;
	return singelton;
}

void HeaderManager::register_evth_callback(std::function<void(EventHeader&)> p_function)
{
	m_oCallbackEVTH.push_back(p_function);
}

void HeaderManager::register_evte_callback(std::function<void(EventEnd&)> p_function)
{
	m_oCallbackEVTE.push_back(p_function);
}

void HeaderManager::register_runh_callback(std::function<void(RunHeader&)> p_function)
{
	m_oCallbackRUNH.push_back(p_function);
}

void HeaderManager::register_rune_callback(std::function<void(RunEnd&)> p_function)
{
	m_oCallbackRUNE.push_back(p_function);
}


