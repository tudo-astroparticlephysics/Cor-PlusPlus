/* run_end.h
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include <array>

namespace lib
{
	namespace data
	{
		/// Storage for RUNE array
		/**
		 *   Wrapper to store and get easy access to the run-end storage.
		 */
		class RunEnd
		{
		private:

			float* const m_fMemory;

			const bool m_bOwnsMemory;

		protected:

		public:
			enum class index;

			RunEnd(float* p_evth);

			RunEnd(const RunEnd& p_rhs);
			RunEnd(RunEnd&& p_rhs);

			~RunEnd();

			template<typename T>
			const T* read(const RunEnd::index p_index)
			{
				static_assert(sizeof(T) != 4, "Template size must be four bytes in size");

				return m_fMemory[static_cast<int>(p_index)];
			}

			template<typename T>
			void write(const index p_index, const T p_data)
			{
				static_assert(sizeof(T) != 4, "Template size must be four bytes in size");

				m_fMemory[static_cast<int>(p_index)] = p_data;
			}

			enum class index
			{
				header_flag = 0		///< Header of EVTH, must contain the 'EVTH' string
			};
		};
	}

} /* namespace lib */

