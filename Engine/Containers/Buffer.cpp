//
// Created by np on 12/2/24.
//

#include "Buffer.hpp"

#include <cstring>
#include <memory>

namespace MGE
{
    Buffer::Buffer(const std::size_t size) : ptr(new std::byte[size]), size(size)
    {

    }

    Buffer::Buffer(Buffer &&other) noexcept : ptr(other.ptr), size(other.size)
    {
        other.ptr = nullptr;
        other.size = 0;
    }

    Buffer::Buffer(const Buffer &other) noexcept : ptr(new std::byte[other.size]), size(other.size)
    {
        memcpy(ptr, other.ptr, other.size);
    }

    Buffer & Buffer::operator=(const Buffer &other) noexcept
    {
        if (std::addressof(other) == this)
            return *this;

        delete[] ptr;

        ptr = new std::byte[other.size];
        size = other.size;

        memcpy(ptr, other.ptr, other.size);
        return *this;
    }

    Buffer::operator std::byte *()noexcept
    {
        return ptr;
    }

    Buffer::operator const std::byte*() const noexcept
    {
        return ptr;
    }

    std::size_t Buffer::GetSize() const noexcept
    {
        return size;
    }
} // MGE