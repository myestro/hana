/*!
@file
Forward declares `boost::hana::Group`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_GROUP_HPP
#define BOOST_HANA_GROUP_GROUP_HPP

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/monoid/monoid.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Group` type class is used for `Monoid`s where all objects have
    //! an inverse w.r.t. the `Monoid`'s binary operation.
    //!
    //! The method names refer to the group of numbers under addition.
    //!
    //! ### Requires
    //! `Monoid`
    //!
    //! ### Laws
    //! For all objects `x` of a `Group` `G`, the following laws must be
    //! satisfied:
    //! @code
    //!     plus(x, negate(x)) == zero<G> // right inverse
    //!     plus(negate(x), x) == zero<G> // left inverse
    //! @endcode
    struct Group {
        BOOST_HANA_BINARY_TYPECLASS(Group);
        template <typename G1, typename G2>
        struct negate_mcd;
        template <typename G1, typename  G2>
        struct minus_mcd;
        struct laws;
    };

    //! Subtract two elements of a group.
    //! @relates Group
    //!
    //! Specifically, this performs the `Monoid` operation on the first
    //! argument and on the inverse of the second argument, thus being
    //! equivalent to:
    //! @code
    //!     minus(x, y) == plus(x, negate(y))
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/group/minus.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto minus = [](auto x, auto y) {
        return Group::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::minus_impl(x, y);
    };

    //! Return the inverse of an element of a group.
    //! @relates Group
    //!
    //! Since `Group` is a binary type class and `negate` is a unary method,
    //! `negate(x)` is dispatched to the type class instance for `G` and `G`,
    //! i.e. `Group::instance<G, G>`, where `G` is the data type of `x`.
    //!
    //! ### Example
    //! @snippet example/group/negate.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto negate = [](auto x) {
        return Group::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(x)>
        >::negate_impl(x);
    };

    //! Minimal complete definition: `minus`
    template <typename G1, typename G2>
    struct Group::minus_mcd {
        template <typename X>
        static constexpr auto negate_impl(X x) {
            return minus(zero<G1>, x);
        }
    };

    namespace operators {
        //! Equivalent to `minus`.
        //! @relates boost::hana::Group
        template <typename C1, typename C2>
        constexpr auto operator-(C1 c1, C2 c2)
        { return minus(c1, c2); }

        //! Equivalent to `negate`.
        //! @relates boost::hana::Group
        template <typename C1>
        constexpr auto operator-(C1 c1)
        { return negate(c1); }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_GROUP_GROUP_HPP

#ifndef BOOST_HANA_GROUP_GROUP_MORE_HPP
#define BOOST_HANA_GROUP_GROUP_MORE_HPP

#include <boost/hana/core/is_a.hpp>


namespace boost { namespace hana {
    //! Instance of `Group` for objects with numeric data types.
    //!
    //! Any two objects whose data types form a `Monoid` and can be subtracted
    //! with the usual `operator-` naturally form an additive group, with
    //! the group subtraction being that usual `operator-`.
    template <typename X, typename Y>
    struct Group::instance<X, Y, when<
        are<Monoid, X, Y>()
        &&
        (
            decltype((void)(*(X*)0 - *(Y*)0), 1){}, true

        )
    >>
        : Group::minus_mcd<X, Y>
    {
        static constexpr auto minus_impl(X x, Y y) {
            return x - y;
        }
    };
}}

#endif
