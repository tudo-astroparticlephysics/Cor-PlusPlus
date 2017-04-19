/* header_set.h
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

#include "lib/data/corsika/event_header.h"
#include "lib/data/corsika/event_end.h"
#include "lib/data/corsika/run_header.h"
#include "lib/data/corsika/run_end.h"



using namespace lib::data;

class HeaderSet
{
private:
	std::shared_ptr<EventHeader> evth;
	std::shared_ptr<EventEnd> evte;
	std::shared_ptr<RunHeader> runh;
	std::shared_ptr<RunEnd> rune;

protected:

public:


	HeaderSet();
	HeaderSet(HeaderSet&& rhs);

	void setEVTH(EventHeader evth);
	void moveEVTH(std::shared_ptr<EventHeader>&& evth);
	std::weak_ptr<EventHeader> getEVTH();	/// get data via weak_ptr.lock()

	void setEVTE(const float* const evte);
	void moveEVTE(EventEnd evte);
	std::weak_ptr<EventEnd> getEVTE();

	void setRUNH(const float* const runh);
	void moveRUNH(RunHeader runh);
	std::weak_ptr<RunHeader> getRUNH();

	void setRUNE(const float* const rune);
	void moveRUNE(RunEnd rune);
	std::weak_ptr<RunEnd> getRUNE();

};
