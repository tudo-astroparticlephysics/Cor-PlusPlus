/* is_callable.h
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

/**
 *    \file is_callable.h
 *    \brief
 *
 *
 */

namespace lib
{
	namespace meta
	{
		/// Auxilary namespace, includes code that the user shouldn't use directly
		namespace aux
		{

			/*template<typename, typename TRet, class... TArgs>
			 struct is_callable_class
			 {
			 static_assert(std::integral_constant<TRet, false>::value, "Second template parameter needs to be of function type.");
			 };*/

			// specialization that does the checking
			


			///@{
			/**   SFINAE check
			 If the class has an operator() with defined arguments the return value is checked. If its not available
			 an error is createt -> this leads to the second check()
			 */
			template<class T, class TReturn, class... TPara>
			static auto test_class(T*)
			-> typename std::is_same<decltype(std::declval<T>().operator()(std::declval<TPara>()...)), TReturn >::type;

			template<class, class, class...>
			static auto test_class(...) -> std::false_type;
			/// ... is elipsis and has the lowest rank for overloading

			///@}

			
			template<class T, class TSig>
			static auto test_func(T*) -> typename std::is_same<T, TSig>::type;

			template<class, class>
			static auto test_func(...) -> std::false_type;


			// C is a class, delegate to is_functor_with_signature
			template<bool TIsClass, class TCal, class TReturn, class ... TPara>
			struct _is_callable;

			template<class TClass, class TReturn, class ... TPara>
			struct _is_callable<true, TClass, TReturn, TPara...> : decltype( test_class<TClass, TReturn, TPara...>(0) )
			{
			};

			/// Case for function
			template<class TFunc, class TReturn, class ... TPara>
			struct _is_callable<false, TFunc, TReturn, TPara...> : decltype( test_func<TFunc, TReturn (*)(TPara...)>(0) )
			{
			};
		}
		
		template<class TCal, class TReturn, class ... TPara>
		struct is_callable : aux::_is_callable<std::is_class<TCal>::value, TCal, TReturn, TPara...>
		{
		};
	
	}

}
