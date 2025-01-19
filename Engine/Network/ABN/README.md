## Network Exceptions Usage Guide for RTYPE

This guide explains how to use the exceptions defined in the `abn::client_exception` namespace to handle network-related errors in a customized way.

### Includes

Include the file containing the exceptions in your project

1. If you are using client/server of **abn** lib:
    ```cpp
    #include "UDPClient.hpp" / #include "UDPServer.hpp"
    
    OR
   
    #include "TCPClient.hpp" / #include "TCPServer.hpp"
    ```
1 bis. If you are not using client/server of **abn** lib:
   ```cpp
   #include "Abn.hpp"
   ```

### Using the Exceptions

Here’s an example of how to use the `ptr_exception`, `port_exception`, and `host_exception` exceptions.

```cpp
#include "UDPClient.hpp.hpp"
#include <iostream>

// This macro can be used for add specific information fitting your protocol
// Like client's id, time, etc...
// For example it's the basic std::cerr output
#define CLIENT_CERR std::cerr

int main(void)
{
    try {
        // If shared_pointer is invalid, ill-sized or null
        throw abn::client_exception::ptr_exception("Null pointer access.");
    } catch (const abn::client_exception::ptr_exception& e) {
        CLIENT_CERR << e.what() << std::endl;
    }

    try {
        // If port is outside the clustered range or non-numercial 
        throw abn::client_exception::port_exception("Invalid port number.");
    } catch (const abn::client_exception::port_exception& e) {
        CLIENT_CERR << e.what() << std::endl;
    }

    try {
        // If host is invalid
        throw abn::client_exception::host_exception("Unable to resolve host.");
    } catch (const abn::client_exception::host_exception& e) {
        CLIENT_CERR << e.what() << std::endl;
    }

    return (0);
}
