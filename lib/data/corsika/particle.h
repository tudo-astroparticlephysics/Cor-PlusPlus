/* particle.h
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include <cstddef>
#include <memory>
#include <array>

#include <iostream>
#include <sstream>

#include "lib/meta/index_sequence.h"

namespace lib
{

	namespace data
	{

		/// Possible storage container for different CORSIKA modules
		/**
		 * 	This enum contains all currently possible variations for the particle storage internally used in CORSIKA.
		 * 	In the default version of CORSIKA its not possible to use the basic and, for example, the curved content together. All intermediate ones must be included
		 * 	too without regards if it used or not.
		 */
		enum ParticleDataType
		{
			basic = 0, 	///< Fundamental particle informations
			thin,		///< Additional information for the thining option
			curved,		///< Additional information for the curved atmosphere model
			inttest,	///< Additional information for the inttest
			ehistory,	///< Additional information for the storage of individual particle history
			parallel,	///< Additional information for the mpi parallel execution (does not work with ehistory)
			unused,		///< Unknown information on position 40
			multithin,	///< Additional information for the multithin option
			padding,	///< 4 Byte without enum for padding purpose
			custom		///< Custom sized storage with start and stop enum
		};

		/// Wrapper class for different particle storage types
		/**
		 *  Manages an array for the intermediate ParticleStorage for different CORSIKA modules. Additionally access to every field is possible with its name.
		 *
		 *  \tparam TSize Size (size_t) in Byte of the of the preceding classes
		 *  \tparam ...TArgs (size_t) List of storage modules added to the particle. Possible modules can be fount in \ref ParticleDataType. For restrictions with
		 *  						 with the current CORSIKA see the documentation of ParticleDataType.
		 */
		template<size_t TSize, size_t ... TArgs>
		struct ParticleData;

		/// Base case for the particle storage class, containing most of its functionality
		/**
		 *   Base case to end the unpacking of the variadic template. This class contains most of the basic functionality to copy print and compare the stored data.
		 *
		 *   \tparam size_t $TSize Final size of the particle storage
		 */
		template<size_t TSize>
		struct ParticleData<TSize>
		{
			std::array<double, TSize> m_oData;

			static const int dataSize = 0;

			ParticleData()	{}

			~ParticleData()	{}

			/// Trivial copy constructor
			/**
			*   \param[in] rhs Object to copy 
			*/
			ParticleData(const ParticleData<TSize>& p_rhs)
			{
				m_oData = p_rhs.m_oData;
			}

			/// Trivial move constructor
			/**
			*   Deletes parameter object and moves the storage over to this
			*   \param rhs Object to move 
			*/
			ParticleData(ParticleData<TSize> && p_rhs)
			{
				m_oData = p_rhs.m_oData;
			}

			/// Trivial asignment (copy) operator
			/**
			*   \param[in] rhs Copys the content into the particle storage 
			*/
			ParticleData<TSize>& operator=(const ParticleData<TSize>& p_rhs)
			{
				m_oData = p_rhs.m_oData;
				return *this;
			}

			/// Raw read/write access to the data
			/**
			 *  \return returns the raw pointer to the data
			 */
			double* data()
			{
				return m_oData.data();
			}

			/// Raw read/write access to the data
			/**
			*  \return returns the data at a specific position
			*/
			double& operator[](const size_t p_id)
			{
				if (p_id < 0 || p_id >= TSize)
				{
					std::stringstream sstr;
					sstr << p_id << " from " << TSize;
					throw(std::out_of_range("p_id is out of range! Is " + sstr.str()));
				}

				return m_oData[p_id];

			}

			/// Raw read access to the data
			/**
			*  \return returns the data at a specific position
			*/
			const double& operator[](const size_t p_id) const
			{
				if (p_id < 0 || p_id >= TSize)
				{
					std::stringstream sstr;
					sstr << p_id << " from " << TSize;
					throw(std::out_of_range("p_id is out of range! Is " + sstr.str()));
				}

				return m_oData[p_id];
			}
				

			/// Raw read access to the data
			/**
			 *  \return returns a constant raw pointer to the data
			 */
			const double* data() const
			{
				return m_oData.data();
			}



			/// Appends the data to a basic output stream
			/**
			 *  Used for a trivial output for stream functions like cout or fstream
			 *  \return returns a ostream with appended particle data
			 */
			friend std::ostream& operator<<(std::ostream& p_stream, const ParticleData<TSize>& p_particle)
			{
				p_stream << " (";
				for (int i = 0; i < TSize; i++)
					p_stream << p_particle.m_oData[i] << ", ";
				p_stream << ")\n";
				
				return p_stream;
			}

			constexpr unsigned int size() const
			{
				return TSize;
			}
		};

		/// Specialisation of the ParticleData class for the base storage
		/**
		 *  \tparam size_t $Size in Byte of the of the preceding classes
		 *  \tparam size_t $...TArgs List of storage modules that will be added after this.
		 */
		template<size_t TSize, size_t ... TArgs>
		struct ParticleData<TSize, ParticleDataType::basic, TArgs...> : public ParticleData<TSize + 13, TArgs...>
		{
			/// Shorten the name of the other storage parts
			typedef ParticleData<TSize + 13, TArgs...> Parent;

			/// Appends its own size to the storage container
			static const int dataSize = Parent::dataSize + 13;

			/// Content of the basic particle storage
			/**
			 *  Describes the content of the basic particle storage and allows access to the indivitual elements by name
			 */
			enum basic
			{
				PARTICLE_TYPE = TSize,				///< 0  Particle type
				GAMMA,								///< 1  Lorentz factor
				COSTHE,								///< 2  Direction cos Z-Direction
				PHIX,								///< 3  Direction cos X-Direction
				PHIY,								///< 4  Direction cos Y-Direction
				H,									///< 5  (true) height
				T,									///< 6  Accumulated time in seconds
				X,									///< 7  X-Position
				Y,									///< 8  Y-Position
				GENERATION,							///< 9  particle generation
				LEVEL_OF_LAST_INTERACTION,			///< 10 level of last interaction
				POLARIZATION_DIR1,					///< 11 Polarization direction, cos(theta) for muons
				POLARIZATION_DIR2					///< 12 Polarization direction, phi for muons
			};

			ParticleData()
					: Parent()
			{
			}

			/// Trivial copy constructor
			/**
			*   \param[in] rhs Object to copy
			*/
			ParticleData(const ParticleData<TSize, ParticleDataType::basic, TArgs...>& p_rhs)
				: Parent(p_rhs)
			{
			}

			ParticleData(ParticleData<TSize, ParticleDataType::basic, TArgs...> && p_rhs)
					: Parent(std::forward<Parent>(p_rhs))
			{
			}

			ParticleData<TSize, ParticleDataType::basic, TArgs...>& operator=(const ParticleData<TSize, 0, TArgs...>& p_rhs)
			{
				static_cast<Parent*>(this)->operator=(p_rhs);
				return *this;
			}

		};

		/// Specialisation of the ParticleData class for the thining storage
		/**
		 *  \tparam size_t $Size in Byte of the of the preceding classes
		 *  \tparam size_t $...TArgs List of storage modules that will be added after this.
		 */
		template<size_t TSize, size_t ... TArgs>
		struct ParticleData<TSize, ParticleDataType::thin, TArgs...> : public ParticleData<TSize + 1, TArgs...>
		{
			/// Shorten the name of the other storage parts
			typedef ParticleData<TSize + 1, TArgs...> Parent;

			/// Appends its own size to the storage container
			static const int dataSize = Parent::dataSize + 1;

			/// Content of the thin particle storage
			/**
			*  Describes the content of the thin particle storage and allows access to the indivitual elements by name
			*/
			enum thin
			{
				weight = TSize			///< 13 Thinning weight
			};

			ParticleData()
					: Parent()
			{
			}

			/// Trivial copy constructor
			/**
			*   \param[in] rhs Object to copy
			*/
			ParticleData(const ParticleData<TSize, ParticleDataType::thin, TArgs...>& p_rhs)
				: Parent(p_rhs)
			{
			}

			ParticleData(ParticleData<TSize, ParticleDataType::thin, TArgs...> && p_rhs)
					: Parent(std::forward<Parent>(p_rhs))
			{
			}

			ParticleData<TSize, ParticleDataType::thin, TArgs...>& operator=(const ParticleData<TSize, ParticleDataType::thin, TArgs...>& p_rhs)
			{
				static_cast<Parent*>(this)->operator=(p_rhs);
				return *this;
			}

		};

		/// Specialisation of the ParticleData class for the curved storage
		/**
		 *  \tparam size_t $Size in Byte of the of the preceding classes
		 *  \tparam size_t $...TArgs List of storage modules that will be added after this.
		 */
		template<size_t TSize, size_t ... TArgs>
		struct ParticleData<TSize, ParticleDataType::curved, TArgs...> : public ParticleData<TSize + 3, TArgs...>
		{
			/// Shorten the name of the other storage parts
			typedef ParticleData<TSize + 3, TArgs...> Parent;

			/// Appends its own size to the storage container
			static const int dataSize = Parent::dataSize + 3;

			/// Content of the curved particle storage
			/**
			*  Describes the content of the curved particle storage and allows access to the indivitual elements by name
			*/
			enum curved
			{				
				APPARENT_HEIGHT = TSize,	///< 14 apparent height
				APPARENT_ZENITH,			///< 15 apparent zenith angle cosine
				ANGLE						///< 16 angle at earth center
			};

			ParticleData()
					: Parent()
			{
			}

			/// Trivial copy constructor
			/**
			*   \param[in] rhs Object to copy
			*/
			ParticleData(const ParticleData<TSize, ParticleDataType::curved, TArgs...>& p_rhs)
				: Parent(p_rhs)
			{
			}

			ParticleData(ParticleData<TSize, ParticleDataType::curved, TArgs...> && p_rhs)
					: Parent(std::forward<Parent>(p_rhs))
			{
			}

			ParticleData<TSize, ParticleDataType::curved, TArgs...>& operator=(const ParticleData<TSize, ParticleDataType::curved, TArgs...>& p_rhs)
			{
				static_cast<Parent*>(this)->operator=(p_rhs);
				return *this;
			}
		};

		/// Specialisation of the ParticleData class for the inttest storage
		/**
		 *  \tparam size_t $Size in Byte of the of the preceding classes
		 *  \tparam size_t $...TArgs List of storage modules that will be added after this.
		 */
		template<size_t TSize, size_t ... TArgs>
		struct ParticleData<TSize, ParticleDataType::inttest, TArgs...> : public ParticleData<TSize + 1, TArgs...>
		{
			/// Shorten the name of the other storage parts
			typedef ParticleData<TSize + 1, TArgs...> Parent;

			/// Appends its own size to the storage container
			static const int dataSize = Parent::dataSize + 1;

			/// Content of the inttest particle storage
			/**
			*  Describes the content of the inttest particle storage and allows access to the indivitual elements by name
			*/
			enum inttest
			{
				transverse_momentum = TSize    ///< 17
			};

			ParticleData()
					: Parent()
			{
			}

			/// Trivial copy constructor
			/**
			*   \param[in] rhs Object to copy
			*/
			ParticleData(const ParticleData<TSize, ParticleDataType::inttest, TArgs...>& p_rhs)
				: Parent(p_rhs)
			{
			}

			ParticleData(ParticleData<TSize, ParticleDataType::inttest, TArgs...> && p_rhs)
					: Parent(std::forward<Parent>(p_rhs))
			{
			}

			ParticleData<TSize, ParticleDataType::inttest, TArgs...>& operator=(const ParticleData<TSize, ParticleDataType::inttest, TArgs...>& p_rhs)
			{
				static_cast<Parent*>(this)->operator=(p_rhs);
				return *this;
			}
		};

		/// Specialisation of the ParticleData class for the ehistory storage
		/**
		 *  \tparam size_t $Size in Byte of the of the preceding classes
		 *  \tparam size_t $...TArgs List of storage modules that will be added after this.
		 */
		template<size_t TSize, size_t ... TArgs>
		struct ParticleData<TSize, ParticleDataType::ehistory, TArgs...> : public ParticleData<TSize + 22, TArgs...>
		{
			/// Shorten the name of the other storage parts
			typedef ParticleData<TSize + 22, TArgs...> Parent;

			/// Appends its own size to the storage container
			static const int dataSize = Parent::dataSize + 22;
			
			/// Content of the basic ehistory storage
			/**
			*  Describes the content of the ehistory particle storage and allows access to the indivitual elements by name
			*/
			enum ehistory
			{
				MOTHER_PARTICLE_TYPE = TSize,///< 17  Particle type of mother
				MOTHER_GAMMA,                ///< 18  Lorentz factor
				MOTHER_COSTHE,               ///< 19  Direction cos Z-Direction
				MOTHER_PHIX,                 ///< 20  Direction cos X-Direction
				MOTHER_PHIY,                 ///< 21  Direction cos Y-Direction
				MOTHER_H,                    ///< 22  (true) height
				MOTHER_T,                    ///< 23  Accumulated time in seconds
				MOTHER_X,                    ///< 24  X-Position
				MOTHER_Y,                    ///< 25  Y-Position
				MOTHER_WEIGHT,               ///< 26  Thinning weight
				MOTHER_PADDING,              ///< 27  unused
				
				GRANDMOTHER_PARTICLE_TYPE,   ///< 28  Particle type of grandmother
				GRANDMOTHER_GAMMA,           ///< 29  Lorentz factor
				GRANDMOTHER_COSTHE,          ///< 30  Direction cos Z-Direction
				GRANDMOTHER_PHIX,            ///< 31  Direction cos X-Direction
				GRANDMOTHER_PHIY,            ///< 32  Direction cos Y-Direction
				GRANDMOTHER_H,               ///< 33  (true) height
				GRANDMOTHER_T,               ///< 34  Accumulated time in seconds
				GRANDMOTHER_GENERATION,      ///< 35  particle generation
				GRANDMOTHER_CHI,             ///< 36  chi penetration before last interaction (= 0 for EM-particle)
				GRANDMOTHER_WEIGHT,          ///< 37  Thinning weight
				GRANDMOTHER_PADDING          ///< 38  unused
			};

			ParticleData()
					: Parent()
			{
			}

			/// Trivial copy constructor
			/**
			*   \param[in] rhs Object to copy
			*/
			ParticleData(const ParticleData<TSize, ParticleDataType::ehistory, TArgs...>& p_rhs)
				: Parent(p_rhs)
			{
			}

			ParticleData(ParticleData<TSize, ParticleDataType::ehistory, TArgs...> && p_rhs)
					: Parent(std::forward<Parent>(p_rhs))
			{
			}

			ParticleData<TSize, ParticleDataType::ehistory, TArgs...>& operator=(const ParticleData<TSize, ParticleDataType::ehistory, TArgs...>& p_rhs)
			{
				static_cast<Parent*>(this)->operator=(p_rhs);
				return *this;
			}
		};

		/// Specialisation of the ParticleData class for the parallel storage
		/**
		 *  \tparam size_t $Size in Byte of the of the preceding classes
		 *  \tparam size_t $...TArgs List of storage modules that will be added after this.
		 */
		template<size_t TSize, size_t ... TArgs>
		struct ParticleData<TSize, ParticleDataType::parallel, TArgs...> : public ParticleData<TSize + 1, TArgs...>
		{
			/// Shorten the name of the other storage parts
			typedef ParticleData<TSize + 1, TArgs...> Parent;

			/// Appends its own size to the storage container
			static const int dataSize = Parent::dataSize + 1;

			/// Content of the parallel particle storage
			/**
			*  Describes the content of the parallel particle storage and allows access to the indivitual elements by name
			*/
			enum parallel
			{
				///< 18
				///< 39
			};

			ParticleData()
					: Parent()
			{
			}

			/// Trivial copy constructor
			/**
			*   \param[in] rhs Object to copy
			*/
			ParticleData(const ParticleData<TSize, ParticleDataType::parallel, TArgs...>& p_rhs)
				: Parent(p_rhs)
			{
			}

			ParticleData(ParticleData<TSize, ParticleDataType::parallel, TArgs...> && p_rhs)
					: Parent(std::forward<Parent>(p_rhs))
			{
			}

			ParticleData<TSize, ParticleDataType::parallel, TArgs...>& operator=(const ParticleData<TSize, ParticleDataType::parallel, TArgs...>& p_rhs)
			{
				static_cast<Parent*>(this)->operator=(p_rhs);
				return *this;
			}
		};

		/// Specialisation of the ParticleData class for the multithin storage
		/**
		 *  \tparam size_t $Size in Byte of the of the preceding classes
		 *  \tparam size_t $...TArgs List of storage modules that will be added after this.
		 */
		template<size_t TSize, size_t ... TArgs>
		struct ParticleData<TSize, ParticleDataType::multithin, TArgs...> : public ParticleData<TSize + 8, TArgs...>
		{
			/// Shorten the name of the other storage parts
			typedef ParticleData<TSize + 8, TArgs...> Parent;

			/// Appends its own size to the storage container
			static const int dataSize = Parent::dataSize + 8;

			/// Content of the multithin particle storage
			/**
			*  Describes the content of the multithin particle storage and allows access to the indivitual elements by name
			*/
			enum multithin
			{
				///< 41
				///< 42
				///< 43
				///< 44
				///< 45
				///< 46
			};

			ParticleData()
					: Parent()
			{
			}

			/// Trivial copy constructor
			/**
			*   \param[in] rhs Object to copy
			*/
			ParticleData(const ParticleData<TSize, ParticleDataType::multithin, TArgs...>& p_rhs)
				: Parent(p_rhs)
			{
			}

			ParticleData(ParticleData<TSize, ParticleDataType::multithin, TArgs...> && p_rhs)
					: Parent(std::forward<Parent>(p_rhs))
			{
			}

			ParticleData<TSize, ParticleDataType::multithin, TArgs...>& operator=(const ParticleData<TSize, ParticleDataType::multithin, TArgs...>& p_rhs)
			{
				static_cast<Parent*>(this)->operator=(p_rhs);
				return *this;
			}
		};




		/// Specialisation of the ParticleData class for the padding storage
		/**
		*  \tparam size_t $Size in Byte of the of the preceding classes
		*  \tparam size_t $...TArgs List of storage modules that will be added after this.
		*/
		template<size_t TSize, size_t ... TArgs>
		struct ParticleData<TSize, ParticleDataType::padding, TArgs...> : public ParticleData<TSize + 1, TArgs...>
		{
			/// Shorten the name of the other storage parts
			typedef ParticleData<TSize + 1, TArgs...> Parent;

			/// Appends its own size to the storage container
			static const int dataSize = Parent::dataSize + 1;

			/// No enum access to padding bytes		
			enum padding
			{				
			};

			ParticleData()
				: Parent()
			{
			}

			/// Trivial copy constructor
			/**
			*   \param[in] rhs Object to copy
			*/
			ParticleData(const ParticleData<TSize, ParticleDataType::padding, TArgs...>& p_rhs)
				: Parent(p_rhs)
			{
			}

			ParticleData(ParticleData<TSize, ParticleDataType::padding, TArgs...> && p_rhs)
				: Parent(std::forward<Parent>(p_rhs))
			{
			}

			ParticleData<TSize, ParticleDataType::padding, TArgs...>& operator=(const ParticleData<TSize, ParticleDataType::padding, TArgs...>& p_rhs)
			{
				static_cast<Parent*>(this)->operator=(p_rhs);
				return *this;
			}
		};



		/// Specialisation of the ParticleData class for the custom sized storage
		/**
		*  IMPORTANT: To use the custom storage you must define its size directly after type::custom
		*
		*  \tparam size_t $Size in Byte of the of the preceding classes
		*  \tparam size_t $...TArgs List of storage modules that will be added after this.
		*/
		template<size_t TSize, size_t TCustomSize, size_t ... TArgs>
		struct ParticleData<TSize, ParticleDataType::custom, TCustomSize, TArgs...> : public ParticleData<TSize + TCustomSize, TArgs...>
		{
			/// Shorten the name of the other storage parts
			typedef ParticleData<TSize + TCustomSize, TArgs...> Parent;

			/// Appends its own size to the storage container
			static const int dataSize = Parent::dataSize + TCustomSize;

			/// No enum access to padding bytes		
			enum custom
			{
				CUSTOM_START = TSize,
				CUSTOM_END = TSize + TCustomSize - 1
			};

			ParticleData()
				: Parent()
			{
			}

			/// Trivial copy constructor
			/**
			*   \param[in] rhs Object to copy
			*/
			ParticleData(const ParticleData<TSize, ParticleDataType::custom, TArgs...>& p_rhs)
				: Parent(p_rhs)
			{
			}

			ParticleData(ParticleData<TSize, ParticleDataType::custom, TArgs...> && p_rhs)
				: Parent(std::forward<Parent>(p_rhs))
			{
			}

			ParticleData<TSize, ParticleDataType::custom, TArgs...>& operator=(const ParticleData<TSize, ParticleDataType::custom, TArgs...>& p_rhs)
			{
				static_cast<Parent*>(this)->operator=(p_rhs);
				return *this;
			}
		};




















		/// Wrapper to create a complete custom particle
		/**
		 *  Advanced users only
		 *  Creates a particle storage with user defined order and storage modules
		 *  \tparam ...TArgs List of module id's in arbitary order
		 *  \return returns a particle storage
		 */
		template<size_t ... TArgs>
		auto make_particle()
		{
			return ParticleData<0, TArgs...>();
		}


	}
}

