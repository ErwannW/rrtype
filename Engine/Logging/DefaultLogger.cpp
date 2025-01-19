//
// Created by Nail on 20/11/2024.
//

#include "DefaultLogger.hpp"

#include <iostream>

namespace MGE
{
    DefaultLogger::DefaultLogger(std::ostream& infoStream, std::ostream& warningStream, std::ostream& errorStream)
        : infoStream(infoStream), warningStream(warningStream), errorStream(errorStream)
    {
    }

    void DefaultLogger::Info(const std::string_view &message)
    {
        std::cout << "[INFO] " << message << std::endl;
    }

    void DefaultLogger::Warning(const std::string_view &message)
    {
        std::cout << "[WARN] "  << message << std::endl;
    }

    void DefaultLogger::Error(const std::string_view &message)
    {
        std::cerr << "[ERRO] "  << message << std::endl;
    }
} // MGE