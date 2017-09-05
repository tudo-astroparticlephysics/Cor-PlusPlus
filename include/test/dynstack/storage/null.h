/* null.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2017> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include "dynstack/stack/storage/null_stack.h"

#include <limits>

namespace test
{
    namespace dynstack
    {

        TEST(null, capacity)
        {
            ::dynstack::storage::Null_Stack<float> tmp;

            ASSERT_EQ(tmp.capacity(), std::numeric_limits<unsigned long>::max());
        }

        TEST(null, size)
        {
            ::dynstack::storage::Null_Stack<float> tmp;

            ASSERT_EQ(tmp.size(), 0);

            tmp.push_back(3);
            ASSERT_EQ(tmp.size(), 0);

            for(int i=0; i<11; i++)
            {
                tmp.push_back(3);
                ASSERT_EQ(tmp.size(), 0);
            }

            tmp.push_back(3);
            ASSERT_EQ(tmp.size(), 0);
            tmp.push_back(3);
            ASSERT_EQ(tmp.size(), 0);
        }

        TEST(null, pushBack_reference)
        {
            ::dynstack::storage::Null_Stack<float> tmp;

            float a = 1;
            const float b = 2;

            tmp.push_back(a);
            tmp.push_back(b);
            tmp.push_back(3);

            ASSERT_EQ(tmp.size(), 0);
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.pop_back(), float());
        }

        TEST(null, pushBack_move)
        {
            /*::dynstack::storage::Null_Stack< std::unique_ptr<float> > tmp(12);

            std::unique_ptr<float> a(new float(1.0f));
            std::unique_ptr<float> b(new float(2.0f));
            std::unique_ptr<float> c(new float(3.0f));

            tmp.push_back( std::move(a) );
            tmp.push_back( std::move(b) );
            tmp.push_back( std::move(c) );


            ASSERT_EQ(tmp.size(), 3);
            ASSERT_EQ(*tmp.pop_back(), 1.0f);
            ASSERT_EQ(*tmp.pop_back(), 2.0f);
            ASSERT_EQ(*tmp.pop_back(), 3.0f);*/
        }

        TEST(null, pushBack_uniquePtr)
        {
            ::dynstack::storage::Null_Stack< float > tmp;

            std::unique_ptr<float[]> a(new float[4]);
            a[0] = 1.0f;
            a[1] = 2.0f;
            a[2] = 3.0f;
            a[3] = 4.0f;

            tmp.push_back(std::move(a), 4);

            ASSERT_EQ(tmp.size(), 0);
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.pop_back(), float());
        }

        TEST(null, pushBack_ptr)
        {
            ::dynstack::storage::Null_Stack< float > tmp;

            float a[] = {1.0f, 2.0f, 3.0f, 4.0f};

            tmp.push_back(a, 4);

            ASSERT_EQ(tmp.size(), 0);
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.pop_back(), float());
        }

        TEST(null, back)
        {
            ::dynstack::storage::Null_Stack< float > tmp;

            // Test normal behaviour
            tmp.push_back(3.0f);
            ASSERT_EQ( float(), tmp.back());

            // Order correct?
            tmp.push_back(4.0f);
            ASSERT_EQ( float(), tmp.back());

            tmp.pop();
            ASSERT_EQ( float(), tmp.back());

            // Test back on an empty Stack
            ::dynstack::storage::Null_Stack< float > tmp2;
            ASSERT_EQ(tmp2.back(), float());   // Returnes default constructed value
        }

        TEST(null, pop)
        {
            ::dynstack::storage::Null_Stack< float > tmp;
            tmp.push_back(3.0f);

            // Test Normal behaviour
            ASSERT_EQ(tmp.size(), 0);
            tmp.pop();
            ASSERT_EQ(tmp.size(), 0);
            // Test already empty behaviour
            tmp.pop();
            ASSERT_EQ(tmp.size(), 0);

            // Test behaviour after overflow
            for(int i=0; i<12; i++)
            {
                tmp.push_back(3);
            }
            ASSERT_EQ(tmp.size(), 0);

            tmp.pop();
            ASSERT_EQ(tmp.size(), 0);

            // Test behaviour of a new initialized stack
            ::dynstack::storage::Null_Stack< float > tmp2;
            tmp2.pop();

            ASSERT_EQ(tmp2.size(), 0);
        }

        TEST(null, popBack)
        {
            ::dynstack::storage::Null_Stack< float > tmp;

            // Empty init behaviour
            ASSERT_EQ(tmp.size(), 0);
            ASSERT_EQ(tmp.pop_back(), float() );
            ASSERT_EQ(tmp.size(), 0);

            // Normal behaviour
            tmp.push_back(3.0f);
            tmp.push_back(4.0f);
            ASSERT_EQ(tmp.size(), 0);
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.size(), 0);
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.size(), 0);

            // Empty behaviour
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.size(), 0);

            // Test behaviour after overflow
            for(int i=0; i<12; i++)
            {
                tmp.push_back(3.0f);
            }
            ASSERT_EQ(tmp.size(), 0);
            ASSERT_EQ(tmp.pop_back(), float());
            ASSERT_EQ(tmp.size(), 0);

        }

        TEST(null, pusbBack_UpperLimit)
        {
            ::dynstack::storage::Null_Stack< float > tmp;

            for(unsigned int i=0; i<12; i++)
            {
                tmp.push_back(i);
            }
            ASSERT_EQ(tmp.size(), 0);

            // Default insert
            float a = 1;
            const float b = 2;

            tmp.push_back(a);
            tmp.push_back(b);
            tmp.push_back(3);

            ASSERT_EQ(tmp.size(), 0);

            // unique_ptr array insert
            std::unique_ptr<float[]> c(new float[4]);
            c[0] = 1;
            c[1] = 2;
            c[2] = 3;
            c[3] = 4;
            tmp.push_back(std::move(c), 4);

            ASSERT_EQ(tmp.size(), 0);

            // Array insert
            float d[] = {1.0f, 2.0f, 3.0f, 4.0f};
            tmp.push_back(d, 4);

            ASSERT_EQ(tmp.size(), 0);
        }


        TEST(null, clear)
        {
            ::dynstack::storage::Null_Stack< float > tmp;

            // Empty test
            ASSERT_EQ(tmp.size(), 0);
            tmp.clear();
            ASSERT_EQ(tmp.size(), 0);

            // Fill Normal
            tmp.push_back( 1.0f );
            ASSERT_EQ(tmp.size(), 0);
            tmp.clear();
            ASSERT_EQ(tmp.size(), 0);

            for(unsigned int i=0; i<12; i++)
            {
                tmp.push_back(i);
            }
            ASSERT_EQ(tmp.size(), 0);
            tmp.clear();
            ASSERT_EQ(tmp.size(), 0);

        }

        TEST(null, invalidRegion)
        {
            ::dynstack::storage::Null_Stack< float > tmp;

            tmp.push_back(2);
            ASSERT_EQ(tmp.size(), 0);

            auto backElement = tmp.back();
            float old = backElement;
            const bool ret = tmp.pop();

            tmp.push_back(1);

            ASSERT_EQ(old, backElement);
            ASSERT_EQ(ret, true);
        }

    }
}
