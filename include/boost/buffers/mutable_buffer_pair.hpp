//
// Copyright (c) 2023 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/cppalliance/buffers
//

#ifndef BOOST_BUFFERS_MUTABLE_BUFFER_PAIR_HPP
#define BOOST_BUFFERS_MUTABLE_BUFFER_PAIR_HPP

#include <boost/buffers/detail/config.hpp>
#include <boost/buffers/mutable_buffer.hpp>
#include <boost/assert.hpp>

namespace boost {
namespace buffers {

/** A mutable buffer pair
*/
class mutable_buffer_pair
{
public:
    using value_type = mutable_buffer;

    using const_iterator = value_type const*;

    /** Constructor.
    */
    mutable_buffer_pair() = default;

    /** Constructor.
    */
    mutable_buffer_pair(
        mutable_buffer_pair const&) = default;

    /** Constructor.
    */
    mutable_buffer_pair(
        mutable_buffer const& b0,
        mutable_buffer const& b1) noexcept
        : b_{ b0, b1 }
    {
    }

    /** Assignment.
    */
    mutable_buffer_pair& operator=(
        mutable_buffer_pair const&) = default;

    mutable_buffer const&
    operator[](unsigned i) const noexcept
    {
        BOOST_ASSERT(i < 2);
        return b_[i];
    }

    mutable_buffer&
    operator[](unsigned i) noexcept
    {
        BOOST_ASSERT(i < 2);
        return b_[i];
    }

    const_iterator
    begin() const noexcept
    {
        return b_;
    }

    const_iterator
    end() const noexcept
    {
        return b_ + 2;
    }

    friend
    mutable_buffer_pair
    tag_invoke(
        prefix_tag const&,
        mutable_buffer_pair const& b,
        std::size_t n) noexcept
    {
        return b.prefix_impl(n);
    }

    friend
    mutable_buffer_pair
    tag_invoke(
        suffix_tag const&,
        mutable_buffer_pair const& b,
        std::size_t n) noexcept
    {
        return b.suffix_impl(n);
    }

private:
    BOOST_BUFFERS_DECL
    mutable_buffer_pair
    prefix_impl(
        std::size_t n) const noexcept;

    BOOST_BUFFERS_DECL
    mutable_buffer_pair
    suffix_impl(
        std::size_t n) const noexcept;

    mutable_buffer b_[2];
};

} // buffers
} // boost

#endif
