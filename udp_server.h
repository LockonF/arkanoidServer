#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <set>
#include "Serializador.h"

using boost::asio::ip::udp;



class udp_server
{
public:
    udp_server(boost::asio::io_service& io_service,int port);
    void start_receive();
    void print();
    
private:
    
    void handle_receive(const boost::system::error_code& error,
                        std::size_t /*bytes_transferred*/);
    
    
    void handle_send(boost::shared_ptr<std::string> /*message*/,
                     const boost::system::error_code& /*error*/,
                     std::size_t /*bytes_transferred*/)
    {
    }
    
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 50000> recv_buffer_;
    //Funcion de aceptación
    bool is_player(const char * data);
    //Funcion para darle el número al jugador
    std::string assign_player();
    //Funcion para saber si está registrado
    bool is_registered(udp::endpoint player);
    
    
    //Timer
    boost::asio::deadline_timer timer_;
    //Remote endpoint
    std::set<udp::endpoint> participants_;
    //Tablero del arkanoid
    Tablero tablero;
    
};