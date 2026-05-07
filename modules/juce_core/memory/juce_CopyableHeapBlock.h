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

/** Wraps a HeapBlock, but additionally provides a copy constructor and remembers its size.

    This class is not intended for use as a general-purpose container - in most cases, a std::vector
    or juce::Array is a better choice. However, CopyableHeapBlock may be handy for optimisation in
    situations where the container elements don't need to be value-initialised and the container
    doesn't need to be resized.

    @see Array, OwnedArray, MemoryBlock, HeapBlock

    @tags{Core}
*/
template <typename ElementType, bool throwOnFailure = false>
class CopyableHeapBlock
{
public:
    static_assert (std::is_trivially_copyable_v<ElementType>);

    /** Creates an empty block. */
    CopyableHeapBlock() = default;

    /** Creates a block large enough to hold the specified number of elements.

        The contents of the block are unspecified, and *must* be written before they are first read.
    */
    explicit CopyableHeapBlock (size_t numElementsIn)
        : block (numElementsIn),
          numElements (numElementsIn)
    {
    }

    /** Moves the contents of another block into this one. */
    CopyableHeapBlock (CopyableHeapBlock&& other) noexcept
        : block (std::exchange (other.block, {})),
          numElements (std::exchange (other.numElements, {}))
    {
    }

    /** Copies the contents of another block into this one. */
    CopyableHeapBlock (const CopyableHeapBlock& other)
        : block (other.numElements),
          numElements (other.numElements)
    {
        std::copy (other.block.getData(), other.block.getData() + other.numElements, block.getData());
    }

    /** Moves the contents of another block into this one. */
    CopyableHeapBlock& operator= (CopyableHeapBlock&& other) noexcept
    {
        CopyableHeapBlock { std::move (other) }.swap (*this);
        return *this;
    }

    /** Copies the contents of another block into this one. */
    CopyableHeapBlock& operator= (const CopyableHeapBlock& other) noexcept
    {
        CopyableHeapBlock { other }.swap (*this);
        return *this;
    }

    /** Returns a pointer to the first element in the block. */
    ElementType* data()
    {
        return block.getData();
    }

    /** Returns a pointer to the first element in the block. */
    const ElementType* data() const
    {
        return block.getData();
    }

    /** Returns the number of elements in the block. */
    size_t size() const
    {
        return numElements;
    }

    /** Returns true if the size of the block is zero. */
    bool isEmpty() const
    {
        return size() == 0;
    }

    /** Returns a pointer to the first element in the block. */
    ElementType* begin()
    {
        return data();
    }

    /** Returns a pointer to the first element in the block. */
    const ElementType* begin() const
    {
        return data();
    }

    /** Returns a pointer to the first element in the block. */
    const ElementType* cbegin() const
    {
        return data();
    }

    /** Returns a pointer immediately after the last element in the block. */
    ElementType* end()
    {
        return data() + size();
    }

    /** Returns a pointer immediately after the last element in the block. */
    const ElementType* end() const
    {
        return data() + size();
    }

    /** Returns a pointer immediately after the last element in the block. */
    const ElementType* cend() const
    {
        return data() + size();
    }

    /** Returns a reference to the element at the provided index. */
    ElementType& operator[] (size_t index)
    {
        return data()[index];
    }

    /** Returns a reference to the element at the provided index. */
    const ElementType& operator[] (size_t index) const
    {
        return data()[index];
    }

private:
    void swap (CopyableHeapBlock& other) noexcept
    {
        std::swap (other.block, block);
        std::swap (other.numElements, numElements);
    }

    HeapBlock<ElementType, throwOnFailure> block;
    size_t numElements{};
};

} // namespace juce
