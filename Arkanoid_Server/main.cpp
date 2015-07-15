#include "TCPServer.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        //Creamos el io_service que va a hacer el handling e inicializamos el servidor
        boost::asio::io_service io_service;
        tcp::endpoint endpoint(tcp::v4(), 7000);
        TCPServer server(io_service,endpoint);
        std::cout<<"Started server"<<std::endl;
        
        //io_service crea un nuevo hilo, por lo que las operaciones se vuelven asíncronas.
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}