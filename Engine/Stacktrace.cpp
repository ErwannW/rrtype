//
// Created by nailperreau on 25/11/24.
//

#include "Stacktrace.hpp"

#if _GLIBCXX_HAVE_STACKTRACE || (defined _MSC_VER && _MSC_VER > 1934)
#define STACKTRACE_AVAILABLE
#endif

#ifdef STACKTRACE_AVAILABLE
#include <stacktrace>
#endif

namespace Stacktrace
{
    std::string GetStackTraceAsString()
    {
#ifdef STACKTRACE_AVAILABLE
        return std::to_string(std::stacktrace::current());
#else
      return std::string("Your platform doesn't support stack tracing.");
#endif
    }
}
