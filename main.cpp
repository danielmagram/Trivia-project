#include <iostream>
#include "Server.h"

int main()
{
    try
    {
        Server server;
        server.run();
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unknown error occurred." << std::endl;
    }

    return 0;
}