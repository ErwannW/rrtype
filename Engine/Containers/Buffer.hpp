//
// Created by np on 12/2/24.
//

#ifndef RTYPE_BUFFER_HPP
#define RTYPE_BUFFER_HPP

#include <cstddef>

namespace MGE
{
    /**
     * @struct Buffer
     * Represents a raw memory buffer that is automatically freed when it is destroyed.
     */
    class Buffer
    {
        public:
            Buffer() = default;

            /**
             * Creates of a buffer of the given size.
             * @param size
             */
            explicit Buffer(std::size_t size);

            /**
             * Moves a buffer.
             * @param other
             */
            Buffer(Buffer&& other) noexcept;
            Buffer(const Buffer& other) noexcept;

            Buffer& operator=(const Buffer& other) noexcept;

            operator std::byte*() noexcept;
            operator const std::byte*() const noexcept;

            std::size_t GetSize() const noexcept;

        private:
            std::byte* ptr = nullptr;
            std::size_t size = 0;
    };

} // MGE

#endif //RTYPE_BUFFER_HPP
