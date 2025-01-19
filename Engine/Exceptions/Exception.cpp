//
// Created by nailperreau on 20/11/24.
//

#include <utility>
#include "Stacktrace.hpp"
#include "Exception.hpp"

namespace MGE
{
    Exception::Exception(std::string message) : trace(Stacktrace::GetStackTraceAsString()), message(std::move(message))
    {

    }

    const char* Exception::what() const noexcept
    {
        finalMessage = message + "\n" + trace;
        return finalMessage.c_str();
    }
} // MGE