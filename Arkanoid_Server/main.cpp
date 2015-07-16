#include "udp_server.h"
#include <cstdlib>
#include <iostream>





int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_service io_service;
        udp_server server(io_service,7200);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
        
        return 0;
}