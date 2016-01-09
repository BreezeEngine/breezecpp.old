/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef RANGES_V3_ALGORITHM_GENERATE_N_HPP
#define RANGES_V3_ALGORITHM_GENERATE_N_HPP

#include <tuple>
#include <utility>
#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/static_const.hpp>
#include <range/v3/utility/tagged_pair.hpp>
#include <range/v3/algorithm/tagspec.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-algorithms
        /// @{
        struct generate_n_fn
        {
            template<typename O, typename F,
                CONCEPT_REQUIRES_(Function<F>() &&
                    WeakOutputIterator<O, concepts::Function::result_t<F>>())>
            tagged_pair<tag::out(O), tag::fun(F)>
            operator()(O begin, iterator_difference_t<O> n, F fun) const
            {
                RANGES_ASSERT(n >= 0);
                auto norig = n;
                auto b = uncounted(begin);
                for(; 0 != n; ++b, --n)
                    *b = fun();
                return {recounted(begin, b, norig), fun};
            }
        };

        /// \sa `generate_n_fn`
        /// \ingroup group-algorithms
        namespace
        {
            constexpr auto&& generate_n = static_const<generate_n_fn>::value;
        }

        // @}
    } // namespace v3
} // namespace ranges

#endif // include guard
