//
// Created by Nail on 20/11/2024.
//

#ifndef ILOGGER_HPP
#define ILOGGER_HPP

/**
 * Interface for logging abstractions.
 */
class ILogger
{
    public:
        virtual ~ILogger() = default;

        /**
         * Logs an info.
         * @param message
         */
        virtual void Info(const std::string_view& message) = 0;
        /**
         * Logs a warning.
         * @param message
         */
        virtual void Warning(const std::string_view& message) = 0;
        /**
         * Logs an error.
         * @param message
         */
        virtual void Error(const std::string_view& message) = 0;
};

#endif //ILOGGER_HPP
