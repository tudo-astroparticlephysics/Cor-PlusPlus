/* io_exception.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>

#include <sstream>

namespace lib
{
	namespace io
	{
		namespace network
		{
			/// Exception for not creatable sockets
			class socket_creation_failed: public std::runtime_error
			{
			public:
				socket_creation_failed()
						: std::runtime_error("Coult not create the socket!")
				{
				}
			};

			/// Exception if socket message could not send all data
			class socket_send_incomplete: public std::runtime_error
			{
			private:
				const size_t m_should;	/// Number of bytes that should be sended
				const size_t m_is;      /// Number of bytes that get sendet
			public:
				socket_send_incomplete(const size_t should, const size_t is)
						: std::runtime_error("Coult not send the complete message!"), m_should(should), m_is(is)
				{
				}

				virtual const char* what() const throw ()
				{
					std::stringstream sstr;
					sstr << runtime_error::what() << ": " << m_is << "/" << m_should << "Bytes sendet \n";
					return sstr.str().c_str();
				}
			};
		}

	}
}


