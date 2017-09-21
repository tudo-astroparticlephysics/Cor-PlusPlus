
/* fifo.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include "dynstack/stack/storage/fifo_stack.h"

namespace test
{
    namespace dynstack
    {

        TEST(FIFO, capacity)
        {
            ::dynstack::storage::FIFO_Stack<float> tmp(12);

            ASSERT_EQUAL(tmp.capacity(), 12);
        }

        TEST(FIFO, size)
        {
            ::dynstack::storage::FIFO_Stack<float> tmp(12);

            ASSERT_EQUAL(tmp.size(), 0);

        }

        TEST(FIFO, pushBack_reference)
        {
            ::dynstack::storage::FIFO_Stack<float> tmp(12);

            float a = 1;
            const float b = 2;

            tmp.push_back(a);
            tmp.push_back(b);
            tmp.push_back(3);

            ASSERT_EQUAL(tmp.size(), 3);
            ASSERT_EQUAL(tmp.pop_back(), 1);
            ASSERT_EQUAL(tmp.pop_back(), 2);
            ASSERT_EQUAL(tmp.pop_back(), 3);
        }

        TEST(FIFO, pushBack_move)
        {
            ::dynstack::storage::FIFO_Stack< std::unique_ptr<float> > tmp(12);

            std::unique_ptr<float> a(new float(1.0f));
            std::unique_ptr<float> b(new float(2.0f));
            std::unique_ptr<float> c(new float(3.0f));

            tmp.push_back( std::move(a));
            tmp.push_back( std::move(b));
            tmp.push_back( std::move(c));


            ASSERT_EQUAL(tmp.size(), 3);
            ASSERT_EQUAL(tmp.pop_back(), 1.0f);
            ASSERT_EQUAL(tmp.pop_back(), 2.0f);
            ASSERT_EQUAL(tmp.pop_back(), 3.0f);
        }

        TEST(FIFO, pushBack_uniquePtr)
        {
            ::dynstack::storage::FIFO_Stack< std::unique_ptr<float> > tmp(12);

            std::unique_ptr<float[]> a(new float[4]);
            a[0] = 1;
            a[1] = 2;
            a[2] = 3;
            a[3] = 4;

            tmp.push_back(std::move(a));

            ASSERT_EQUAL(tmp.size(), 4);
            ASSERT_EQUAL(tmp.pop_back(), 1);
            ASSERT_EQUAL(tmp.pop_back(), 2);
            ASSERT_EQUAL(tmp.pop_back(), 3);
            ASSERT_EQUAL(tmp.pop_back(), 4);
        }

        TEST(FIFO, pushBack_ptr)
        {
            ::dynstack::storage::FIFO_Stack< float > tmp(12);

            float[] a = {1.0f, 2.0f, 3.0f, 4.0f};

            tmp.push_back(a, 4);

            tmp.push_back(std::move(a));

            ASSERT_EQUAL(tmp.size(), 4);
            ASSERT_EQUAL(tmp.pop_back(), 1.0f);
            ASSERT_EQUAL(tmp.pop_back(), 2.0f);
            ASSERT_EQUAL(tmp.pop_back(), 3.0f);
            ASSERT_EQUAL(tmp.pop_back(), 4.0f);
        }

        TEST(FIFO, back)
        {
            ::dynstack::storage::FIFO_Stack< float > tmp(12);

        }

        TEST(FIFO, pop)
        {

        }

        TEST(FIFO, popBack)
        {

        }

        TEST(FIFO, pusbBack_UpperLimit)
        {
            ::dynstack::storage::FIFO_Stack< float > tmp(12);

            for(int i=0; i<tmp.capacity(); i++)
            {
                tmp.push_back(i);
            }
            ASSERT_EQUAL(tmp.size(), tmp.capacity());

            // Default insert
            float a = 1;
            const float b = 2;

            tmp.push_back(a);
            tmp.push_back(b);
            tmp.push_back(3);

            ASSERT_EQUAL(tmp.size(), tmp.capacity());

            // Move insert
            std::unique_ptr<float> a(new float(3.4f));
            tmp.push_back( std::move(a));

            ASSERT_EQUAL(tmp.size(), tmp.capacity());

            // unique_ptr array insert
            std::unique_ptr<float[]> a(new float[4]);
            a[0] = 1;
            a[1] = 2;
            a[2] = 3;
            a[3] = 4;
            tmp.push_back(std::move(a));

            ASSERT_EQUAL(tmp.size(), tmp.capacity());

            // Array insert
            float[] a = {1.0f, 2.0f, 3.0f, 4.0f};
            tmp.push_back(a, 4);

            ASSERT_EQUAL(tmp.size(), tmp.capacity());
        }


        TEST(FIFO, clear)
        {

        }

        TEST(FIFO, invalidRegion)
        {
            testObject.push_back(data[1]);
            auto backElement = testObject.back();
            TType old = backElement;
            const bool ret = testObject.pop();

            testObject.push_back(data[0]);

            if (old != backElement && ret != false)
            {
                std::cerr << "pop_back returns: " << ret << std::endl;

                std::cerr << "Element vor Push: " << old << std::endl;

                std::cerr << "Element nach Push: " << (TType)backElement << std::endl;
                return false;
            }

        }

    }
}
