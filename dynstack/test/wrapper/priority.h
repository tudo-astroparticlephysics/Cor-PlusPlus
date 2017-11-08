/* priority.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2017> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include "dynstack/stack/storage/fifo_stack.h"
#include "dynstack/stack/wrapper/priority_stack.h"

namespace test
{
    namespace dynstack
    {

        bool priority(const float* const data)
        {
            if(*data > 6)
            {
                return true;
            }
            return false;
        }

        TEST(PriorityStack, Ctr)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp1(12);
            tmp1.push_back(1.0f);
            tmp1.push_back(7.0f);

            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp2( tmp1 );

            ASSERT_EQ(tmp1.capacity(), 24);
            ASSERT_EQ(tmp1.size(), 2);
            ASSERT_EQ(tmp1.pop_back(), 7.0f);
            ASSERT_EQ(tmp1.size(), 1);
            ASSERT_EQ(tmp1.pop_back(), 1.0f);

            ASSERT_EQ(tmp2.capacity(), 24);
            ASSERT_EQ(tmp2.size(), 2);
            ASSERT_EQ(tmp2.pop_back(), 7.0f);
            ASSERT_EQ(tmp2.size(), 1);
            ASSERT_EQ(tmp2.pop_back(), 1.0f);

            tmp1.push_back(1.0f);
            tmp1.push_back(7.0f);

            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp3( std::move(tmp1) );

            ASSERT_EQ(tmp3.capacity(), 24);
            ASSERT_EQ(tmp3.size(), 2);
            ASSERT_EQ(tmp3.pop_back(), 7.0f);
            ASSERT_EQ(tmp3.size(), 1);
            ASSERT_EQ(tmp3.pop_back(), 1.0f);
        }

        TEST(PriorityStack, capacity)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp(12);

            ASSERT_EQ(tmp.capacity(), 24);  // Internal are two stacks
        }

        TEST(PriorityStack, size)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp(12);

            ASSERT_EQ(tmp.size(), 0);

            tmp.push_back(3.0f);
            ASSERT_EQ(tmp.size(), 1);

            for(int i=0; i<11; i++)
            {
                tmp.push_back(3.0f);
                ASSERT_EQ(2 + i, tmp.size());
            }

            tmp.push_back(3.0f);
            ASSERT_EQ(tmp.size(), 12);
            tmp.push_back(3.0f);
            ASSERT_EQ(tmp.size(), 12);

            tmp.push_back(8.0f);
            ASSERT_EQ(tmp.size(), 13);
        }

        TEST(PriorityStack, pushBack_reference)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp(12);

            float a1 = 1.0f;
            const float b1 = 2.0f;

            float a2 = 7.0f;
            const float b2 = 8.0f;

            tmp.push_back(a1);
            tmp.push_back(b1);
            tmp.push_back(3.0f);

            tmp.push_back(a2);
            tmp.push_back(b2);
            tmp.push_back(9.0f);

            ASSERT_EQ(tmp.size(), 6);
            ASSERT_EQ(tmp.pop_back(), 7.0f);
            ASSERT_EQ(tmp.pop_back(), 8.0f);
            ASSERT_EQ(tmp.pop_back(), 9.0f);

            ASSERT_EQ(tmp.pop_back(), 1.0f);
            ASSERT_EQ(tmp.pop_back(), 2.0f);
            ASSERT_EQ(tmp.pop_back(), 3.0f);
        }

        TEST(PriorityStack, pushBack_move)
        {
            /*::dynstack::storage::FIFO_Stack< std::unique_ptr<float> > tmp(12);

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

        TEST(PriorityStack, pushBack_uniquePtr)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp(12);

            std::unique_ptr<float[]> a(new float[7]);
            a[0] = 1.0f;
            a[1] = 2.0f;
            a[2] = 3.0f;
            a[3] = 4.0f;
            a[4] = 9.0f;
            a[5] = 4.0f;
            a[6] = 8.0f;

            tmp.push_back(std::move(a), 7);

            ASSERT_EQ(tmp.size(), 7);
            ASSERT_EQ(tmp.pop_back(), 9.0f);
            ASSERT_EQ(tmp.pop_back(), 8.0f);
            ASSERT_EQ(tmp.pop_back(), 1.0f);
            ASSERT_EQ(tmp.pop_back(), 2.0f);
            ASSERT_EQ(tmp.pop_back(), 3.0f);
            ASSERT_EQ(tmp.pop_back(), 4.0f);
            ASSERT_EQ(tmp.pop_back(), 4.0f);
        }

        TEST(PriorityStack, pushBack_ptr)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp(12);

            float a[] = {1.0f, 2.0f, 3.0f, 4.0f, 9.0f, 4.0f, 8.0f};

            tmp.push_back(a, 7);

            ASSERT_EQ(tmp.size(), 7);
            ASSERT_EQ(tmp.pop_back(), 9.0f);
            ASSERT_EQ(tmp.pop_back(), 8.0f);
            ASSERT_EQ(tmp.pop_back(), 1.0f);
            ASSERT_EQ(tmp.pop_back(), 2.0f);
            ASSERT_EQ(tmp.pop_back(), 3.0f);
            ASSERT_EQ(tmp.pop_back(), 4.0f);
            ASSERT_EQ(tmp.pop_back(), 4.0f);
        }

        TEST(PriorityStack, back)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp(12);

            // Test normal behaviour
            tmp.push_back(3.0f);
            ASSERT_EQ(tmp.back(), 3.0f);

            // Order correct?
            tmp.push_back(4.0f);
            ASSERT_EQ(tmp.back(), 3.0f);

            tmp.pop();
            ASSERT_EQ(tmp.back(), 4.0f);

            // Order for priority
            tmp.push_back(1.0f);
            tmp.push_back(7.0f);
            ASSERT_EQ(tmp.back(), 7.0f);
            tmp.pop();
            ASSERT_EQ(tmp.back(), 4.0f);
            tmp.pop();
            ASSERT_EQ(tmp.back(), 1.0f);


            // Test back on an empty Stack
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp2(12);
            ASSERT_EQ(tmp2.back(), float());   // Returnes default constructed value
        }

        TEST(PriorityStack, pop)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp(12);
            tmp.push_back(3.0f);

            // Test Normal behaviour
            ASSERT_EQ(tmp.size(), 1);
            tmp.pop();
            ASSERT_EQ(tmp.size(), 0);
            // Test already empty behaviour
            tmp.pop();
            ASSERT_EQ(tmp.size(), 0);

            // Test behaviour after overflow
            for(int i=0; i<12; i++)
            {
                tmp.push_back(3.0f);
            }
            ASSERT_EQ(tmp.size(), 12);

            for(int i=0; i<13; i++)
            {
                tmp.push_back(7.0f);
            }

            for(int i=0; i<=24; i++)
            {
                ASSERT_EQ(tmp.size(), 24 - i);
                tmp.pop();
            }

            // Test behaviour of a new initialized stack
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp2(12);
            tmp2.pop();

            ASSERT_EQ(tmp2.size(), 0);
        }

        TEST(PriorityStack, popBack)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp(12);

            // Empty init behaviour
            ASSERT_EQ(tmp.size(), 0);
            ASSERT_EQ(tmp.pop_back(), 0.0f);
            ASSERT_EQ(tmp.size(), 0);

            // Normal behaviour
            tmp.push_back(3.0f);
            tmp.push_back(4.0f);
            ASSERT_EQ(tmp.size(), 2);
            ASSERT_EQ(tmp.pop_back(), 3.0f);
            ASSERT_EQ(tmp.size(), 1);
            ASSERT_EQ(tmp.pop_back(), 4.0f);
            ASSERT_EQ(tmp.size(), 0);

            // Empty behaviour
            ASSERT_EQ(tmp.pop_back(), 0.0f);
            ASSERT_EQ(tmp.size(), 0);

            // Test behaviour after overflow
            for(int i=0; i<12; i++)
            {
                tmp.push_back(3.0f);
            }
            ASSERT_EQ(tmp.size(), 12);
            ASSERT_EQ(tmp.pop_back(), 3.0f);
            ASSERT_EQ(tmp.size(), 11);

        }

        TEST(PriorityStack, pusbBack_UpperLimit)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp(12);

            for(unsigned int i=0; i<12; i++)
            {
                tmp.push_back(1.0f);
                tmp.push_back(7.0f);
            }
            ASSERT_EQ(tmp.size(), tmp.capacity());

            // Default insert
            float a = 1.0f;
            const float b = 2.0f;

            tmp.push_back(a);
            tmp.push_back(b);
            tmp.push_back(8.0f);

            ASSERT_EQ(tmp.size(), tmp.capacity());

            // unique_ptr array insert
            std::unique_ptr<float[]> c(new float[4]);
            c[0] = 1.0f;
            c[1] = 2.0f;
            c[2] = 7.0f;
            c[3] = 8.0f;
            tmp.push_back(std::move(c), 4);

            ASSERT_EQ(tmp.size(), tmp.capacity());

            // Array insert
            float d[] = {1.0f, 2.0f, 7.0f, 8.0f};
            tmp.push_back(d, 4);

            ASSERT_EQ(tmp.size(), 24);
        }


        TEST(PriorityStack, clear)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp(12);

            // Empty test
            ASSERT_EQ(tmp.size(), 0);
            tmp.clear();
            ASSERT_EQ(tmp.size(), 0);

            // Fill Normal
            tmp.push_back( 1.0f );
            ASSERT_EQ(tmp.size(), 1);
            tmp.clear();
            ASSERT_EQ(tmp.size(), 0);

            for(unsigned int i=0; i<tmp.capacity(); i++)
            {
                tmp.push_back(i);
            }
            ASSERT_EQ(tmp.size(), 19);  // 0,1,2,3,4,5,6 in priority and 12 in rest -> 19 Elements
            tmp.clear();
            ASSERT_EQ(tmp.size(), 0);

        }

        TEST(PriorityStack, invalidRegion)
        {
            ::dynstack::wrapper::PriorityStack< ::dynstack::storage::FIFO_Stack<float>, priority> tmp(12);

            tmp.push_back(2);
            ASSERT_EQ(tmp.size(), 1);

            auto backElement = tmp.back();
            float old = backElement;
            const bool ret = tmp.pop();

            tmp.push_back(1);

            ASSERT_EQ(old, backElement);
            ASSERT_EQ(ret, true);
        }

    }
}
