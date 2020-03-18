/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{
namespace dsp
{

//==============================================================================
#ifndef DOXYGEN
/** The contents of this namespace are used to implement ProcessorChain and should
    not be used elsewhere. Their interfaces (and existence) are liable to change!
*/
namespace detail
{
    template <typename Fn, typename Tuple, size_t... Ix>
    constexpr void forEachInTuple (Fn&& fn, Tuple&& tuple, std::index_sequence<Ix...>)
        noexcept (noexcept (std::initializer_list<int> { (fn (std::get<Ix> (tuple), Ix), 0)... }))
    {
        (void) std::initializer_list<int> { ((void) fn (std::get<Ix> (tuple), Ix), 0)... };
    }

    template <typename T>
    using TupleIndexSequence = std::index_sequence_for<std::remove_cv_t<std::remove_reference_t<T>>>;

    template <typename Fn, typename Tuple>
    constexpr void forEachInTuple (Fn&& fn, Tuple&& tuple)
        noexcept (noexcept (forEachInTuple (std::forward<Fn> (fn), std::forward<Tuple> (tuple), TupleIndexSequence<Tuple>{})))
    {
        forEachInTuple (std::forward<Fn> (fn), std::forward<Tuple> (tuple), TupleIndexSequence<Tuple>{});
    }
}
#endif

/** This variadically-templated class lets you join together any number of processor
    classes into a single processor which will call process() on them all in sequence.
*/
template <typename... Processors>
class ProcessorChain
{
public:
    /** Get a reference to the processor at index `Index`. */
    template <int Index>       auto& get()       noexcept { return std::get<Index> (processors).processor; }

    /** Get a reference to the processor at index `Index`. */
    template <int Index> const auto& get() const noexcept { return std::get<Index> (processors).processor; }

    /** Set the processor at index `Index` to be bypassed or enabled. */
    template <int Index>
    void setBypassed (bool b) noexcept  { std::get<Index> (processors).isBypassed = b; }

    /** Query whether the processor at index `Index` is bypassed. */
    template <int Index>
    bool isBypassed() const noexcept    { return std::get<Index> (processors).isBypassed; }

    /** Prepare all inner processors with the provided `ProcessSpec`. */
    void prepare (const ProcessSpec& spec)
    {
        detail::forEachInTuple ([&] (auto& item, size_t) { item.processor.prepare (spec); }, processors);
    }

    /** Reset all inner processors. */
    void reset()
    {
        detail::forEachInTuple ([] (auto& item, size_t) { item.processor.reset(); }, processors);
    }

    /** Process `context` through all inner processors in sequence. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        detail::forEachInTuple ([&] (auto& item, size_t index) noexcept
        {
            if (context.usesSeparateInputAndOutputBlocks() && index != 0)
            {
                jassert (context.getOutputBlock().getNumChannels() == context.getInputBlock().getNumChannels());
                ProcessContextReplacing<typename ProcessContext::SampleType> replacingContext (context.getOutputBlock());
                replacingContext.isBypassed = (item.isBypassed || context.isBypassed);

                item.processor.process (replacingContext);
            }
            else
            {
                ProcessContext contextCopy (context);
                contextCopy.isBypassed = (item.isBypassed || context.isBypassed);

                item.processor.process (contextCopy);
            }
        }, processors);
    }

private:
    template <typename Processor>
    struct ProcessorWithBypass
    {
        Processor processor;
        bool isBypassed = false;
    };

    std::tuple<ProcessorWithBypass<Processors>...> processors;
};

/** Non-member equivalent of ProcessorChain::get which avoids awkward
    member template syntax.
*/
template <int Index, typename... Processors>
inline auto& get (ProcessorChain<Processors...>& chain) noexcept
{
    return chain.template get<Index>();
}

/** Non-member equivalent of ProcessorChain::get which avoids awkward
    member template syntax.
*/
template <int Index, typename... Processors>
inline auto& get (const ProcessorChain<Processors...>& chain) noexcept
{
    return chain.template get<Index>();
}

/** Non-member equivalent of ProcessorChain::setBypassed which avoids awkward
    member template syntax.
*/
template <int Index, typename... Processors>
inline void setBypassed (ProcessorChain<Processors...>& chain, bool bypassed) noexcept
{
    chain.template setBypassed<Index> (bypassed);
}

/** Non-member equivalent of ProcessorChain::isBypassed which avoids awkward
    member template syntax.
*/
template <int Index, typename... Processors>
inline bool isBypassed (const ProcessorChain<Processors...>& chain) noexcept
{
    return chain.template isBypassed<Index>();
}

} // namespace dsp
} // namespace juce
