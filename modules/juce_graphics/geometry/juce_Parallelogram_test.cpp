/*
  ==============================================================================

   This file is part of the JUCE 9 preview.
   Copyright (c) Raw Material Software Limited

   You may use this code under the terms of the AGPLv3
   (see www.gnu.org/licenses).

   For the JUCE 9 preview this file cannot be licensed commercially.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

class ParallelogramTest : public UnitTest
{
public:
    ParallelogramTest() : UnitTest ("Parallelogram", UnitTestCategories::graphics) {}

    void runTest() override
    {
        beginTest ("isEmpty");
        {
            expect (! Parallelogram (Rectangle<int> (10, 10, 20, 20)).isEmpty());
            expect (Parallelogram (Rectangle<int> (10, 10, 0, 20)).isEmpty());
            expect (Parallelogram (Rectangle<int> (10, 10, 20, 0)).isEmpty());

            expect (! Parallelogram (Point<int> (0, 0), Point<int> (10, 10), Point<int> (20, 0)).isEmpty());
            expect (Parallelogram (Point<int> (0, 0), Point<int> (0, 0), Point<int> (20, 0)).isEmpty());
            expect (Parallelogram (Point<int> (0, 0), Point<int> (10, 10), Point<int> (10, 10)).isEmpty());
            expect (Parallelogram (Point<int> (20, 0), Point<int> (10, 10), Point<int> (20, 0)).isEmpty());
        }

        beginTest ("operators");
        {
            Parallelogram p (Rectangle<int> (10, 10, 20, 20));
            p += Point<int> (5, 10);
            expect (p.topLeft == Point<int> (15, 20));
            expect (p.topRight == Point<int> (35, 20));
            expect (p.bottomLeft == Point<int> (15, 40));

            p -= Point<int> (10, 5);
            expect (p.topLeft == Point<int> (5, 15));
            expect (p.topRight == Point<int> (25, 15));
            expect (p.bottomLeft == Point<int> (5, 35));
        }
    }
};

static ParallelogramTest parallelogramTest;

} // namespace juce
