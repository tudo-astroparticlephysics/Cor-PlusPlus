/* tuple_algorithm.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include<type_traits>
#include<tuple>

#include "lib/meta/index_sequence.h"





/**
*    \file tuple_algorithm.h
*    \brief Implementation of different tuple (compile time) algorithm
*
*    In C++11 not available or specific algorithm for the std::tuple class.
*
*/



namespace lib
{
	namespace meta
	{
		namespace tuple
		{
			/// Unpacks tuple to class c'tor
			/**
			*   This class is used to split a normal tuple into a parameter pack and forward it to the constructor of a arbitary class
			*
			*    \tparam TForward Class who gets the unpacked tuple as constructor input
			*/
			template<class TForward>
			class tupleUnpack : public TForward  
			{
			private:

				/// Unpack tuple expansion
				/**
				*    This function uses an indice list and the default tuple get function to expand the tuple contents into a comma seperated list
				*    and perfect forward it to the constructor of the user defined class
				*    \param args Tuple with content
				*    \tparam TArgs Variadic template describing the tuple content types
				*    \tparam Is Variadic template containing the indice list from index_sequence to access all tuple members
				*/
				template<class ... TArgs, std::size_t ... Is>
				tupleUnpack(std::tuple<TArgs...>&& args, lib::meta::integer_sequence<Is...>)
					: TForward( std::forward<TArgs>( std::get<Is>(args) )... )
				{
				}

			public:
		
				/// Single parameter direct forward
				/**
				*    perfect forwards a single parameter to the inherit class
				*/
				template<class TArg>
				tupleUnpack(TArg arg)
					: TForward( std::forward<TArg>(arg) )
				{}

				/// Single tuple element direct forward
				/**
				*    perfect forwards a single parameter to the inherit class
				*/
				template<class TArg>
				tupleUnpack(std::tuple<TArg> arg)
					: TForward( std::forward<TArg>(std::get<0>(arg)) )
				{}

				
				/// Unpack tuple interface
				/**
				*    Constructor that gets an tuple as parameter and creates an index sequence for it to forwad booth to the privat constructor
				*    \param args Tuple with content
				*    \tparam TArgs Variadic template describing the tuple content types
				*/
				template<class ... TArgs>
				tupleUnpack(std::tuple<TArgs...> args)
					: tupleUnpack( std::forward<std::tuple<TArgs...>&& >(args), lib::meta::index_sequence_from<sizeof...(TArgs)>())
				{}

				
				
				
			};

			
			/// Helper functions for an easy output
			/**
			* Helper functions for an easy output of default tuple
			* http://stackoverflow.com/questions/6245735/pretty-print-stdtuple
			* \todo Replace with own implementation (foldl maybe)
			*/
			namespace aux
			{
				template<class Ch, class Tr, class Tuple, std::size_t ... Is>
				void print_tuple(std::basic_ostream<Ch, Tr>& os, const Tuple& t, const lib::meta::integer_sequence<Is...>)
				{
					(void)(os);
					(void)(t);
					using swallow = int[];
					(void)swallow {
						0, (void(os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), 0)...
					};
				}
			} 

			/// Enable an easy ostream output for tuple
			/**
			*
			*/
			template<class Ch, class Tr, class ... Args>
			auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
				-> std::basic_ostream<Ch, Tr>&
			{
				os << "(";
				::lib::meta::tuple::aux::print_tuple(os, t, lib::meta::index_sequence_from<sizeof...(Args)>());
				return os << ")";
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/* 		filterTuple
			 *		filter specific type (TFilter) from tuple (TTuple)
			 **/

			namespace filterNamespace
			{
				template<class TTuple, class TFilter, size_t TNum, bool TMatch>
				struct _filter;

				template<class TTuple, class TFilter, size_t TNum>
				struct _filter<TTuple, TFilter, TNum, true>
				{
					constexpr static auto filter_same(const TTuple tup)
					{
						return std::tuple_cat(_filter<TTuple, TFilter, TNum - 1, std::is_same<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_same(tup),
							std::make_tuple(std::get<TNum>(tup)));
					}

					constexpr static auto filter_not_same(const TTuple tup)
					{
						return std::tuple_cat(_filter<TTuple, TFilter, TNum - 1, !std::is_same<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_not_same(tup),
							std::make_tuple(std::get<TNum>(tup)));
					}

					constexpr static auto filter_convertible(const TTuple tup)
					{
						return std::tuple_cat(_filter<TTuple, TFilter, TNum - 1, std::is_convertible<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_convertible(tup),
							std::make_tuple(std::get<TNum>(tup)));
					}

					constexpr static auto filter_not_convertible(const TTuple tup)
					{
						return std::tuple_cat(_filter<TTuple, TFilter, TNum - 1, !std::is_convertible<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_not_convertible(tup),
							std::make_tuple(std::get<TNum>(tup)));
					}
				};

				template<class TTuple, class TFilter, size_t TNum>
				struct _filter<TTuple, TFilter, TNum, false>
				{
					constexpr static auto filter_same(const TTuple tup)
					{
						return _filter<TTuple, TFilter, TNum - 1, std::is_same<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_same(tup);
					}

					constexpr static auto filter_not_same(const TTuple tup)
					{
						return _filter<TTuple, TFilter, TNum - 1, !std::is_same<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_not_same(tup);
					}

					constexpr static auto filter_convertible(const TTuple tup)
					{
						return _filter<TTuple, TFilter, TNum - 1, std::is_convertible<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_convertible(tup);
					}

					constexpr static auto filter_not_convertible(const TTuple tup)
					{
						return _filter<TTuple, TFilter, TNum - 1, !std::is_convertible<typename std::tuple_element<TNum - 1, TTuple>::type, TFilter>::value>::filter_not_convertible(tup);
					}
				};

				template<class TTuple, class TFilter>
				struct _filter<TTuple, TFilter, 0, false>
				{
					constexpr static auto filter_same(const TTuple tup)
					{
						(void)(tup);
						return std::make_tuple();
					}

					constexpr static auto filter_not_same(const TTuple tup)
					{
						(void)(tup);
						return std::make_tuple();
					}

					constexpr static auto filter_convertible(const TTuple tup)
					{
						(void)(tup);
						return std::make_tuple();
					}

					constexpr static auto filter_not_convertible(const TTuple tup)
					{
						(void)(tup);
						return std::make_tuple();
					}
				};

				template<class TTuple, class TFilter>
				struct _filter<TTuple, TFilter, 0, true>
				{
					constexpr static auto filter_same(const TTuple tup)
					{
						return std::make_tuple(std::get<0>(tup));
					}

					constexpr static auto filter_not_same(const TTuple tup)
					{
						return std::make_tuple(std::get<0>(tup));
					}

					constexpr static auto filter_convertible(const TTuple tup)
					{
						return std::make_tuple(std::get<0>(tup));
					}

					constexpr static auto filter_not_convertible(const TTuple tup)
					{
						return std::make_tuple(std::get<0>(tup));
					}
				};
			}


			/// Tuple type filter
			/**
			*   Removes all parts of a tuple that does not match the given type
			*
			*   \tparam TFilter Filter the tuple with this specific type
			*	\tparam TTuple Type of tuple that should be filtered
			*/
			template<class TFilter, class TTuple>
			class filterTuple
			{
			private:

			public:

				/// Keep all elements of exactly the type TFilter
				constexpr static auto filter_same(const TTuple tup)
				{
					return filterNamespace::_filter<TTuple, TFilter, std::tuple_size<TTuple>::value, false>::filter_same(tup);
				}

				/// Keep all elements that are not exactly TFilter
				constexpr static auto filter_not_same(const TTuple tup)
				{
					return filterNamespace::_filter<TTuple, TFilter, std::tuple_size<TTuple>::value, false>::filter_not_same(tup);
				}

				/// Keep all elements that are castable to TFilter
				constexpr static auto filter_convertible(const TTuple tup)
				{
					return filterNamespace::_filter<TTuple, TFilter, std::tuple_size<TTuple>::value, false>::filter_convertible(tup);
				}

				/// Keep all elements that are not convertible to TFilter
				constexpr static auto filter_not_convertible(const TTuple tup)
				{
					return filterNamespace::_filter<TTuple, TFilter, std::tuple_size<TTuple>::value, false>::filter_not_convertible(tup);
				}

			};




			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			////	fold for tuples

			/// Tuple fold methode
			/**
			*   Fold right and left for tuples. The user must guarantee that the types inside the tuple are castable to the needed types
			*
			*   Foldl: call function init and first element -> call function with return and second element -> ...
			*   Foldr: call function init and last element -> call function with return and second to last -> ...
			*/
			class foldTuple
			{
			private:

				template<size_t TNum, class TFunc, class TReturn>
				struct _foldl
				{
					template<class ... TArgs>
					constexpr static TReturn fold(TFunc f, const std::tuple<TArgs...> tup, TReturn init)
					{
						return f(std::get<TNum - 1>(tup), _foldl<TNum - 1, TFunc, TReturn>::fold(f, tup, init));
					}
				};

				template<class TFunc, class TReturn>
				struct _foldl<0, TFunc, TReturn>
				{
					template<class ... TArgs>
					constexpr static TReturn fold(TFunc f, const std::tuple<TArgs...> tup, TReturn init)
					{
						return init;
					}
				};

				/*template<class TFunc, class TReturn>
				struct _foldl<-1, TFunc, TReturn>
				{
					template<class ... TArgs>
					constexpr static TReturn fold(TFunc f, const std::tuple<TArgs...> tup, TReturn init)
					{
						(void)(f);
						(void)(tup);
						(void)(init);
						return TReturn();
					}
				};*/

				template<size_t TNum, class TFunc, class TReturn>
				struct _foldr
				{
					template<class ... TArgs>
					constexpr static TReturn fold(TFunc f, const std::tuple<TArgs...> tup, TReturn init)
					{
						return f(std::get<std::tuple_size<decltype(tup)>::value - TNum>(tup), _foldr<TNum - 1, TFunc, TReturn>::fold(f, tup, init));
					}
				};

				template<class TFunc, class TReturn>
				struct _foldr<0, TFunc, TReturn>
				{
					template<class ... TArgs>
					constexpr static TReturn fold(TFunc f, const std::tuple<TArgs...> tup, TReturn init)
					{
						(void)(f);
						(void)(tup);						
						return init;
					}
				};





				template<size_t TNum, template <typename> class TFunc, class TReturn>
				struct _foldr_class
				{
					template<class ... TArgs>
					constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
					{
						return static_cast<TReturn>(TFunc<decltype(std::get<std::tuple_size<decltype(tup)>::value - TNum>(tup))>(std::get<std::tuple_size<decltype(tup)>::value - TNum>(tup), _foldr_class<TNum - 1, TFunc, TReturn>::fold(tup, init)));
					}
				};

				template<template <typename> class TFunc, class TReturn>
				struct _foldr_class<0, TFunc, TReturn>
				{
					template<class ... TArgs>
					constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
					{			
						(void)(tup);
						return init;
					}
				};

				template<size_t TNum, template <typename> class TFunc, class TReturn>
				struct _foldl_class
				{
					template<class ... TArgs>
					constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
					{			
						return static_cast<TReturn>(TFunc<decltype(std::get<TNum>(tup))>(std::get<TNum>(tup), _foldl_class<TNum - 1, TFunc, TReturn>::fold(tup, init)));
					}
				};

				template<template <typename> class TFunc, class TReturn>
				struct _foldl_class<0, TFunc, TReturn>
				{
					template<class ... TArgs>
					constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
					{
						return static_cast<TReturn>(TFunc<decltype(std::get<0>(tup))>(std::get<0>(tup), init));
					}
				};






				/*template<size_t TNum, class TType, template <TType> class TFunc, class TReturn, TType ... TX>
				struct _foldl_t;

				template<size_t TNum, class TType, template <TType> class TFunc, class TReturn, TType T1, TType ... TX>
				struct _foldl_t<TNum, TType, TFunc, TReturn, T1, TX...>
				{
					template<class ... TArgs>
					constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
					{
					return static_cast<TReturn>(
						TFunc< T1 >(
							std::get<TNum>(tup),
							_foldl_t<TNum - 1, TType, TFunc, TReturn, TX...>::fold(tup, init))
						);
					}
				};

				template<class TType, template <TType> class TFunc, class TReturn, TType T1>
				struct _foldl_t<0, TType, TFunc, TReturn, T1>
				{
					template<class ... TArgs>
					constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
					{
						(void)(tup);
						return static_cast<TReturn>(
							TFunc<T1>(
							std::get<0>(tup), init)
						);
					}
				};*/
				

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				template<size_t TNum, class TType, template <TType> class TFunc, class TReturn, TType ... TX>
				struct _foldr_t;

				template<size_t TNum, class TType, template <TType> class TFunc, class TReturn, TType T1, TType ... TX>
				struct _foldr_t<TNum, TType, TFunc, TReturn, T1, TX...>
				{
					template<class ... TArgs>
					constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
					{
						return static_cast<TReturn>( 
							TFunc< T1 >(
								std::get<std::tuple_size<decltype(tup)>::value - TNum>(tup),
								 _foldr_t<TNum - 1, TType, TFunc, TReturn, TX...>::fold(tup, init))
						);
					}
				};

				template<class TType, template <TType> class TFunc, class TReturn>
				struct _foldr_t<0, TType, TFunc, TReturn>
				{
					template<class ... TArgs>
					constexpr static TReturn fold(const std::tuple<TArgs...> tup, TReturn init)
					{
						(void)(tup);
						return init;
					}
				};	


			public:

				template<class TFunc, class TReturn, class TTuple>
				constexpr static TReturn foldl(TFunc f, const TTuple tup, const TReturn init)
				{
					return _foldl<std::tuple_size<TTuple>::value, TFunc, TReturn>::fold(f, tup, init);
				}

				template<class TFunc, class TReturn, class TTuple>
				constexpr static TReturn foldr(TFunc f, const TTuple tup, const TReturn init)
				{
					return _foldr<std::tuple_size<TTuple>::value, TFunc, TReturn>::fold(f, tup, init);
				}

				
				template<template <typename> class TFunc, class TReturn, class TTuple>
				constexpr static TReturn foldl_class(const TTuple tup, const TReturn init)
				{
					return _foldl_class<std::tuple_size<TTuple>::value - 1, TFunc, TReturn>::fold(tup, init);
				}

				template<template <typename> class TFunc, class TReturn, class TTuple>
				constexpr static TReturn foldr_class(const TTuple tup, const TReturn init)
				{
					return _foldr_class<std::tuple_size<TTuple>::value, TFunc, TReturn>::fold(tup, init);
				}
				
					
				// To make this work the template pack TX must be reversed
				/*template<class TType, template <TType> class TFunc, class TReturn, class TTuple, TType ... TX>
				constexpr static TReturn foldl_t(const TTuple tup, const TReturn init)
				{
					return _foldl_t<std::tuple_size<TTuple>::value - 1, TType, TFunc, TReturn, TX...>::fold(tup, init);
				}*/

				template<class TType, template <TType> class TFunc, class TReturn, class TTuple, TType ... TX>
				constexpr static TReturn foldr_t(const TTuple tup, const TReturn init)
				{
					return _foldr_t<std::tuple_size<TTuple>::value, TType, TFunc, TReturn, TX...>::fold(tup, init);
				}
			


				


			};

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			////	map for tuples

			/// Map for tuples
			/**
			*   Call a function on every tuple element. The user must guarantee that the types inside the tuple are castable to the needed types
			*/
			class mapTuple
			{
			private:

				template<class TFunc, class TTuple, std::size_t ... Is>
				constexpr static auto _map(TFunc f, TTuple tup, lib::meta::integer_sequence<Is...>)
				{
					return std::make_tuple(f(std::get<Is>(tup))...);
				}


				template<class TFunc, class TTuple, std::size_t ... Is>
				constexpr static auto _map_with_index(TFunc f, TTuple tup, lib::meta::integer_sequence<Is...>)
				{
					return std::make_tuple(f(std::get<Is>(tup), Is)...);
				}

			public:

				template<class TFunc, class TTuple>
				constexpr static auto map(TFunc f, TTuple tup)
				{
					return _map<TFunc, TTuple>(f, tup, lib::meta::index_sequence_from<std::tuple_size<TTuple>::value> { });
				}

				template<class TFunc, class TTuple>
				constexpr static auto map_with_index(TFunc f, TTuple tup)
				{
					return _map_with_index<TFunc, TTuple>(f, tup, lib::meta::index_sequence_from<std::tuple_size<TTuple>::value> { });
				}
			};

			////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////
			


		}

	}
}
