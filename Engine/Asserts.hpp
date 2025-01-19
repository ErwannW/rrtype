//
// Created by Nail on 20/11/2024.
//

#ifndef ASSERTS_HPP
#define ASSERTS_HPP

#include <string>
#include "Stacktrace.hpp"
#include "Engine.hpp"
#include "Exceptions/Exception.hpp"

#define stringize(x) #x

/**
 * Severity of a failed assert.
 */
enum class Severity
{
    /**
     * This error is recoverable, and is not fatal.
     */
    NonFatal,
    /**
     * This error is not recoverable from the current function.
     */
    Exception,
    /**
     * This error cannot be recovered from and will crash the running process.
     */
    Fatal,
};

/**
 * Checks the result of an assertion.
 * @tparam Severity The severity level of this check.
 * @param ok Did the assertion passed the test?
 * @param msg The message to display if the assertion failed.
 * @return The value passed to ok
 */
template<Severity Severity>
static bool Check(const bool ok, const std::string& msg)
{
    if (ok)
        return true;

    if constexpr (Severity == Severity::Exception)
        throw MGE::Exception(msg);

    MGE::Engine::Current().GetLogger()->Error(msg + "\n" + Stacktrace::GetStackTraceAsString());

    if constexpr (Severity == Severity::Fatal)
        std::terminate();
    return false;
}

/**
 * Checks the result of an assertion.
 * @tparam Severity The severity level of this check.
 * @param ok Did the assertion passed the test?
 * @param msg The message to display if the assertion failed.
 * @return The value passed to ok
 */
template<Severity Severity>
static bool Check(const bool ok, const char* msg)
{
    if (ok)
      return true;

    if constexpr (Severity == Severity::Exception)
      throw MGE::Exception(msg);

    MGE::Engine::Current().GetLogger()->Error(msg + ("\n" + Stacktrace::GetStackTraceAsString()));

    if constexpr (Severity == Severity::Fatal)
      std::terminate();
    return false;
}

/**
 * Different types of debug macros:
 * |-----------------------------|--EXPECT--|---VERIFY---|-----CHECK-----|--UNIMPLEMENTED--|
 * | Behavior in DEBUG build*:   |  Fatal   | Non-Fatal  | Non-Fatal     | Non-Fatal       |
 * |-----------------------------|----------|------------|---------------|-----------------|
 * | Behavior in RELEASE build*: |  Fatal   | Non-Fatal  | Not evaluated | Fatal           |
 * |-----------------------------|----------|------------|---------------|-----------------|
 * Fatal:         will crash the process.
 * Non-fatal:     will log an error and continue the execution.
 * Not evaluated: the expression will not be evaluated, and will be optimized out.
 *
 * *: When the given expression evaluates to false.
 */

#ifdef DEBUG_BUILD
    /**
     * A macro that expand to expr in debug builds, and is optimized out in release builds.
     */
    #define DEBUG(expr) (expr)

    #define EXPECT(expr, msg) Check<Severity::Fatal>((expr), "Fatal assertion failed: \"" stringize(expr) "\" did not evaluate to true (" + std::string(msg) + ").")
    #define EXPECTF(expr) Check<Severity::Fatal>((expr), "Fatal assertion failed: \"" stringize(expr) "\" did not evaluate to true.")
    #define VERIFY(expr, msg) Check<Severity::NonFatal>((expr), "Non-fatal assertion failed: \"" stringize(expr) "\" did not evaluate to true (" + std::string(msg) + ").")
    #define CHECK(expr, msg) Check<Severity::NonFatal>((expr), "Non-fatal assertion failed: \"" stringize(expr) "\" did not evaluate to true (" + std::string(msg) + ").")

    #define UNIMPLEMENTED(msg) do { constexpr bool IsImplemented = false; VERIFY(IsImplemented, msg); } while (false)
#else
    /**
     * A macro that expand to expr in debug builds, and is optimized out in release builds.
     */
    #define DEBUG(expr)

    #define EXPECT(expr, msg) Check<Severity::Fatal>((expr), "Fatal assertion failed: \"" stringize(expr) "\" did not evaluate to true (" + std::string(msg) + ").")
    #define VERIFY(expr, msg) Check<Severity::NonFatal>((expr), "Non-fatal assertion failed: \"" stringize(expr) "\" did not evaluate to true (" + std::string(msg) + ").")
    #define CHECK(expr, msg) (expr)

    #define UNIMPLEMENTED(msg) do { constexpr bool IsImplemented = false; EXPECT(IsImplemented, msg); } while (false)
#endif

#define DBG_TEST(expr, msg) DEBUG(VERIFY(expr, msg))

#endif //ASSERTS_HPP
