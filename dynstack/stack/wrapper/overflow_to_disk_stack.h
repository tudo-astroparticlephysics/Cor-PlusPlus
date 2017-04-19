/* overflow_to_disk_stack.h
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
#include <assert.h>

#include <iostream>

#include "dynstack/stack/stack.h"

#include <fstream>

namespace dynstack
{
	namespace wrapper
	{

		
		template<class TStack, unsigned int TBufferSize>
		class OverflowToDiskStack: public TStack
		{

			static_assert(std::is_base_of<_Stack, TStack>::value, "TStackOrig must be a Stack!");
				
		private:

			typedef typename TStack::m_ReturnType TType;
			typedef typename TStack::m_StackType TTypeStack;




			TTypeStack m_oData[TBufferSize];	// Cache to write/read to/from HDD 
			unsigned int m_uiSize;				// Number of elements in buffer




			std::fstream m_stdFile;				// File access
			unsigned int m_uiBuffersInFile;			// Number of buffers that are stored in file



			void writeBuffer()
			{
				m_stdFile.write(dynamic_cast<char*>(m_oData), sizeof(TTypeStack) * TBufferSize);
				m_uiSize = 0;
				m_uiBuffersInFile++;
			}

			bool read()
			{
				if (m_uiBuffersInFile == 0)
					return false;

				m_stdFile.seekg((m_uiBuffersInFile - 1)*sizeof(TTypeStack)*TBufferSize);
				m_stdFile.seekp((m_uiBuffersInFile - 1)*sizeof(TTypeStack)*TBufferSize);

				m_stdFile.read(dynamic_cast<char*>(m_oData), sizeof(TTypeStack) * TBufferSize);

				m_uiBuffersInFile--;
				m_uiSize = TBufferSize;
			}


		protected:
			

		public:

			template<typename ... Args>
			OverflowToDiskStack(const char* const fileName, Args&&... args)
					: TStack( std::forward<Args>(args)... ), m_uiSize(0)
			{
				m_stdFile.open(fileName, std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);

				if (!m_stdFile.good())
				{
					std::cerr << "Overflow stack could not create caching file!" << std::endl;
				}

				memset(m_oData, 0, sizeof(TTypeStack) * TBufferSize);
			}
			
			OverflowToDiskStack(OverflowToDiskStack<TStack, TBufferSize> && rhs)
				: TStack((TStack&&)rhs), m_stdFile( std::move(rhs.m_stdFile) ), m_uiSize(rhs.m_uiSize)
			{		
				memcpy(m_oData, rhs.m_oData, sizeof(TTypeStack) * TBufferSize);

			}			

			~OverflowToDiskStack()
			{

			}

			inline int push_back(const TType& data)
			{
				const int canStore = TStack::push_back(data);
				
				if (canStore == 0)
				{
					return canStore;
				}
				
				if (m_uiSize >= TBufferSize)
				{
					writeBuffer();
				}

				m_oData[m_uiSize] = data;

				return 0;				
			}
			inline int push_back(TType&& data)
			{
				const bool canStore = TStack::push_back( std::forward<TType>(data) );

				if (canStore == 0)
				{
					return canStore;
				}

				if (m_uiSize >= TBufferSize)
				{
					writeBuffer();
				}

				m_oData[m_uiSize] = data;

				return 0;
			}

			//Copys element to next free space
			inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
			{
				
				for (int i = 0; i < elem; i++)
				{

					const bool canStore = TStack::push_back(data);

					if (canStore == 0)
					{
						return canStore;
					}

					if (m_uiSize >= TBufferSize)
					{
						writeBuffer();
					}

					m_oData[m_uiSize] = data;

					return 0;


				}
			}

			inline int push_back(const TType* const data, const unsigned int elem)
			{
				
				for (int i = 0; i < elem; i++)
				{
					const bool canStore = TStack::push_back(data);

					if (canStore == 0)
					{
						return canStore;
					}

					if (m_uiSize >= TBufferSize)
					{
						writeBuffer();
					}

					m_oData[m_uiSize] = data;

					return 0;
				}
			}		

			///TODO: pop,back,pop_back

			inline unsigned long size() const
			{
				return m_uiSize + TStack::size();
			}

			inline unsigned long capacity() const
			{
				return TBufferSize + TStack::capacity();
			}
		};

	}
}



