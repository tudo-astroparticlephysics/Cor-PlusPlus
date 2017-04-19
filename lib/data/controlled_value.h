/* controlled_value.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once


#include <exception>
#include <type_traits>
#include <memory>


namespace lib
{
	namespace meta
	{
		/// Wrapper with build in error check
		/**
		*   Wrapper class that includes a build in boolean for error checking
		*   If the check variable is false the included data is compromised and
		*   should not be used anymore
		*   Most things should get optimiced away from the compiler
		*
		*	\tparam TType Type of storage that should get wrapped
		*/
		template<class TType>
		class ControlledValue
		{
		private:
			/// Storage
			TType data;

			/// Check variable
			bool check;

		public:

			/// Default constructor sets the internal storage to default construct value and the ckeck to invalide
			constexpr ControlledValue()
				: data(TType()), check(false)
			{}

			/// Copys the value into the storage
			constexpr ControlledValue(const TType& d)
				: data(d), check(true)
			{}

			/// Moves the value into the storage
			constexpr ControlledValue(TType&& d)
				: data(std::forward<TType>(d)), check(true)
			{}

			/// Copy constructor
			constexpr ControlledValue(const ControlledValue& d)
				: data(d.data), check(d.check)
			{
			}

			/// Move constructor
			ControlledValue(ControlledValue&& d)
				: data(std::move(d.data)), check(d.check)
			{
				d.check = false;
			}

			/// Internal status check
			/**
			*   \return Retunes the internal status that gets stored inside the wrapper
			*/
			inline constexpr bool status() const  { return check; }

			/// Cast opreator returns the wrapped value
			inline operator TType() const
			{
				return data;
			}
			/// Cast opreator returns the wrapped value
			inline operator TType&&()
			{
				check = false;
				return std::move(data);
			}
			/// Cast opreator returns the wrapped value
			inline operator TType* ()
			{				
				return &data;
			}
			/// Cast opreator returns the wrapped value
			inline TType* operator&()
			{
				return &data;
			}
			/// assignment opreator assigns to the wrapped value
			inline TType operator=(const TType d)
			{
				data = d;
				return data;
			}
			/// comparison opreator compares the wrapped value
			inline bool operator==(const TType& d) const
			{				
				return d == data;
			}


		};

	}

}
