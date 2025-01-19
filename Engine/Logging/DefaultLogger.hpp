//
// Created by Nail on 20/11/2024.
//

#ifndef DEFAULTLOGGER_HPP
#define DEFAULTLOGGER_HPP
#include <iostream>
#include <string_view>

#include "ILogger.hpp"

namespace MGE
{
    class DefaultLogger : public ILogger
    {
        public:
            explicit DefaultLogger(std::ostream &infoStream = std::cout, std::ostream &warningStream = std::cout, std::ostream &errorStream = std::cerr);

            void Info(const std::string_view &message) override;
            void Warning(const std::string_view &message) override;
            void Error(const std::string_view &message) override;

            std::ostream& infoStream = std::cout;
            std::ostream& warningStream = std::cout;
            std::ostream& errorStream = std::cerr;
    };
} // MGE

#endif //DEFAULTLOGGER_HPP
