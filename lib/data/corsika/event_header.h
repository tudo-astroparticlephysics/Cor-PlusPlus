/* event_header.h
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
		/// Storage for EVTH array
		/**
		*   Wrapper to store and get easy access to the event-header storage.
		*/
		class EventHeader
		{
		private:

			float* const m_fMemory;

			const bool m_bOwnsMemory;

		protected:

		public:
			enum class index;


			EventHeader(float* p_evth);

			EventHeader(const EventHeader& p_rhs);
			EventHeader(EventHeader&& p_rhs);


			~EventHeader();

			template<typename T>
			T read(const EventHeader::index p_index)
			{
				return *reinterpret_cast<T*>(&m_fMemory[static_cast<int>(p_index)]);
			}

			template<typename T>
			void write(const index p_index, const T p_data)
			{
				static_assert(sizeof(T) == 4, "Template size must be four bytes in size");

				m_fMemory[static_cast<int>(p_index)] = p_data;
			}



			enum class index : int
			{
				header_flag = 0,	///< Header of EVTH, must contain the 'EVTH' string
				event_number,		///< Current event number of this simulation
				particle_id,		///< Type of the initial particle
				total_energy,		///< Total energy of the initial particle
				start_alt,			///< Start altitude of particle tracking (in most cases top of the atmosphere)
				first_target,		///< Type of first target if a fixed target is chosen
				height,				///< Height of first interaction
				px,					///< Impulse in X Direction of initial particle
				py,					///< Impulse in Y Direction of initial particle
				pz,					///< Impulse in Z Direction of initial particle
				zenith,				///< Start zenith angle of initial particle (vertical up from experiment is 0)
				azimuth,			///< Start azimuth angle of initial particle
				nmbr_rng,			///< Number of random number generators used
				seed_1,				///< Seed of the first random number generator
				offset_low_1,		///< Number of times the executer got pre-executed (mod 10^6)
				offset_high_1,		///< Number of times the executer got pre-executed ( / 10^6 )
				seed_2,				///<
				offset_low_2,		///<
				offset_high_2,		///<
				seed_3,				///<
				offset_low_3,		///<
				offset_high_3,		///<
				seed_4,				///<
				offset_low_4,		///<
				offset_high_4,		///<
				seed_5,				///<
				offset_low_5,		///<
				offset_high_5,		///<
				seed_6,				///<
				offse_low_6,		///<
				offset_high_6,		///<
				seed_7,				///<
				offset_low_7,		///<
				offset_high_7,		///<
				seed_8,				///<
				offset_low_8,		///<
				offset_high_8,		///<
				seed_9,				///<
				offset_low_9,		///<
				offset_high_9,		///<
				seed_10,			///<
				offset_low_10,		///<
				offset_high_10,		///<
				run_number,			///< Number of run (set in steering card)
				begin_date,			///< Date from the startpoint of this event calculation
				prog_version,		///<
				nmbr_obsv_level,	///<
				obsv_level_height_1,///<
				obsv_level_height_2,///<
				obsv_level_height_3,///<
				obsv_level_height_4,///<
				obsv_level_height_5,///<
				obsv_level_height_6,///<
				obsv_level_height_7,///<
				obsv_level_height_8,///<
				obsv_level_height_9,///<
				obsv_level_height_10,///<
				spectrum_slope,		///<
				energy_lower_limit,	///<
				energy_upper_limit,	///<
				cutoff_hadron,		///<
				cutoff_muon,		///<
				cutoff_electron,	///<
				cutoff_photon,		///<
				NFLAIN,				///<
				NFLDIF,				///<
				NFLPI0,				///<
				NFLPIF,				///<
				NFLCHE,				///<
				NFRAGM,				///<
				earth_magnet_x,		///<
				earth_magnet_y,		///<
				flag_EGS4,			///<
				flag_NKG,			///<
				flag_low_energy_model,///<
				flag_high_energy_level,///<
				flag_cerenkov,		///<
				flag_neutrino,		///<
				flag_curved,		///<
				flag_computer		///<
			};









		};
	}


} /* namespace lib */

