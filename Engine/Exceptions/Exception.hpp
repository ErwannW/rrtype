//
// Created by nailperreau on 20/11/24.
//

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

namespace MGE
{
    /**
     * MGE's base exception class.
     */
    class Exception : public std::exception
    {
        public:
            /**
             * Creates a MGE exception with the given message.
             * @param message
             */
            explicit Exception(std::string message);

            const char *what() const noexcept override;

        private:
            std::string trace;
            std::string message;

            mutable std::string finalMessage;
    };
} // MGE

#endif //EXCEPTION_H
