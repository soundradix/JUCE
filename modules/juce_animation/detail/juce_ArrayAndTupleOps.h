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

#ifndef DOXYGEN
//==============================================================================
/** The contents of this namespace are used to implement Animator and should not
    be used elsewhere. Their interfaces (and existence) are liable to change!
*/
namespace juce::detail::ArrayAndTupleOps
{
    template <typename, typename = void>
    constexpr auto hasTupleSize = false;

    template <typename T>
    constexpr auto hasTupleSize<T, std::void_t<decltype (std::tuple_size<T>::value)>> = true;

    static_assert (! hasTupleSize<float>);
    static_assert (hasTupleSize<std::tuple<float, float>>);
    static_assert (hasTupleSize<std::array<float, 5>>);

    template <typename A, typename B, typename Op, size_t... Ix, std::enable_if_t<hasTupleSize<B>, int> = 0>
    constexpr auto& assignOpImpl (A& a, const B& b, Op&& op, std::index_sequence<Ix...>)
    {
        (op (std::get<Ix> (a), std::get<Ix> (b)), ...);
        return a;
    }

    template <typename A, typename B, typename Op, size_t... Ix, std::enable_if_t<! hasTupleSize<B>, int> = 0>
    constexpr auto& assignOpImpl (A& a, const B& b, Op&& op, std::index_sequence<Ix...>)
    {
        (op (std::get<Ix> (a), b), ...);
        return a;
    }

    template <typename A, typename B, typename Op, std::enable_if_t<hasTupleSize<A>, int> = 0>
    constexpr auto& assignOpImpl (A& a, const B& b, Op&& op)
    {
        return assignOpImpl (a, b, std::forward<Op> (op), std::make_index_sequence<std::tuple_size_v<A>>());
    }

    template <typename A, typename B, std::enable_if_t<hasTupleSize<A>, int> = 0>
    constexpr auto& operator+= (A& a, const B& b)
    {
        return assignOpImpl (a, b, [] (auto& x, auto y)
                                   {
                                       using Tx = std::remove_reference_t<decltype (x)>;
                                       using Ty = std::remove_reference_t<decltype (y)>;

                                       if constexpr (std::is_integral_v<Tx> && std::is_floating_point_v<Ty>)
                                           x = (Tx) std::round ((Ty) x + y);
                                       else
                                           x += y;
                                   });
    }

    template <typename A, typename B, std::enable_if_t<hasTupleSize<A>, int> = 0>
    constexpr auto& operator-= (A& a, const B& b)
    {
        return assignOpImpl (a, b, [] (auto& x, auto y)
                                   {
                                       using Tx = std::remove_reference_t<decltype (x)>;
                                       using Ty = std::remove_reference_t<decltype (y)>;

                                       if constexpr (std::is_integral_v<Tx> && std::is_floating_point_v<Ty>)
                                           x = (Tx) std::round ((Ty) x - y);
                                       else
                                           x -= y;
                                   });
    }

    template <typename A, typename B, std::enable_if_t<hasTupleSize<A>, int> = 0>
    constexpr auto& operator*= (A& a, const B& b)
    {
        return assignOpImpl (a, b, [] (auto& x, auto y)
                                   {
                                       using Tx = std::remove_reference_t<decltype (x)>;
                                       using Ty = std::remove_reference_t<decltype (y)>;

                                       if constexpr (std::is_integral_v<Tx> && std::is_floating_point_v<Ty>)
                                           x = (Tx) std::round ((Ty) x * y);
                                       else
                                           x *= y;
                                   });
    }

    template <typename A, typename B, std::enable_if_t<hasTupleSize<A>, int> = 0>
    constexpr auto& operator/= (A& a, const B& b)
    {
        return assignOpImpl (a, b, [] (auto& x, auto y)
                                   {
                                       using Tx = std::remove_reference_t<decltype (x)>;
                                       using Ty = std::remove_reference_t<decltype (y)>;

                                       if constexpr (std::is_integral_v<Tx> && std::is_floating_point_v<Ty>)
                                           x = (Tx) std::round ((Ty) x / y);
                                       else
                                           x /= y;
                                   });
    }

    template <typename A, typename B, std::enable_if_t<hasTupleSize<A>, int> = 0> constexpr auto operator+ (const A& a, const B& b) { A copy { a }; return copy += b; }
    template <typename A, typename B, std::enable_if_t<hasTupleSize<A>, int> = 0> constexpr auto operator- (const A& a, const B& b) { A copy { a }; return copy -= b; }
    template <typename A, typename B, std::enable_if_t<hasTupleSize<A>, int> = 0> constexpr auto operator* (const A& a, const B& b) { A copy { a }; return copy *= b; }
    template <typename A, typename B, std::enable_if_t<hasTupleSize<A>, int> = 0> constexpr auto operator/ (const A& a, const B& b) { A copy { a }; return copy /= b; }

    static_assert (std::tuple (1.0f, 5.0) + 3.0f == std::tuple (4.0f, 8.0));
    static_assert (std::tuple (1.0f, 5.0) - 1.0f == std::tuple (0.0f, 4.0));
    static_assert (std::tuple (1, 2, 3) * std::tuple (4, 5, 6) == std::tuple (4, 10, 18));
} // namespace juce::detail::ArrayAndTupleOps
#endif
