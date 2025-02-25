//
// Copyright (c) 2023 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/cppalliance/buffers
//

#ifndef BOOST_BUFFERS_MUTABLE_BUFFER_HPP
#define BOOST_BUFFERS_MUTABLE_BUFFER_HPP

#include <boost/buffers/detail/config.hpp>
#include <boost/buffers/tag_invoke.hpp>
#include <cstddef>
#include <type_traits>

namespace boost {
namespace buffers {

/** Holds a buffer that can be modified.
*/
class mutable_buffer
{
    unsigned char* p_ = nullptr;
    std::size_t n_ = 0;

public:
    using value_type = mutable_buffer;
    using const_iterator =
        value_type const*;

    /** Constructor.
    */
    mutable_buffer() = default;

    /** Constructor.
    */
    mutable_buffer(
        mutable_buffer const&) = default;

    /** Constructor.
    */
    mutable_buffer(
        void* data,
        std::size_t size) noexcept
        : p_(static_cast<
            unsigned char*>(data))
        , n_(size)
    {
    }

    /** Assignment.
    */
    mutable_buffer& operator=(
        mutable_buffer const&) = default;

    // conversion to boost::asio::mutable_buffer
    template<
        class T
        , class = typename std::enable_if<
            std::is_constructible<T,
                void*, std::size_t>::value
            && ! std::is_same<T, mutable_buffer>::value
            //&& ! std::is_same<T, const_buffer>::value
        >::type
    >
    operator T() const noexcept
    {
        return T{ data(), size() };
    }

    void*
    data() const noexcept
    {
        return p_;
    }

    std::size_t
    size() const noexcept
    {
        return n_;
    }

    const_iterator
    begin() const noexcept
    {
        return this;
    }

    const_iterator
    end() const noexcept
    {
        return this + 1;
    }

    /** Remove a prefix from the buffer.
    */
    mutable_buffer&
    operator+=(std::size_t n) noexcept
    {
        if(n >= n_)
        {
            p_ = p_ + n_;
            n_ = 0;
            return *this;
        }
        p_ = p_ + n;
        n_ -= n;
        return *this;
    }

    /** Return the buffer with a prefix removed.
    */
    friend
    mutable_buffer
    operator+(
        mutable_buffer b,
        std::size_t n) noexcept
    {
        return b += n;
    }

    /** Return the buffer with a prefix removed.
    */
    friend
    mutable_buffer
    operator+(
        std::size_t n,
        mutable_buffer b) noexcept
    {
        return b += n;
    }

    friend
    mutable_buffer
    tag_invoke(
        prefix_tag const&,
        mutable_buffer const& b,
        std::size_t n) noexcept
    {
        if(n < b.size())
            return { b.p_, n };
        return b;
    }

    friend
    mutable_buffer
    tag_invoke(
        suffix_tag const&,
        mutable_buffer const& b,
        std::size_t n) noexcept
    {
        if(n < b.size())
            return { b.p_ + b.n_ - n, n };
        return b;
    }
};

} // buffers
} // boost

#endif
