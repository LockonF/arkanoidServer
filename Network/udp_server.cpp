//
//  UDPServer.cpp
//  Arkanoid_Server
//
//  Created by Daniel Franco on 7/15/15.
//  Copyright (c) 2015 Daniel. All rights reserved.
//

#include "udp_server.h"

//Función de ejemplo para el control del string
std::string make_string(const char * data)
{
    if(strcmp(data,"jugador")==0)
        return std::string("accepted");
    else
        return std::string("not accepted");
}




//Incializamos el servidor con el puerto, el servicio io y el endpoint
udp_server::udp_server(boost::asio::io_service &io_service, int port): socket_(io_service, udp::endpoint(udp::v4(), port))
{
    start_receive();
}


//Función para recibir
void udp_server::start_receive(){
    socket_.async_receive_from(
                               boost::asio::buffer(recv_buffer_), remote_endpoint_,
                               boost::bind(&udp_server::handle_receive, this,
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
}


//Función que procesa lo recibido
void udp_server::handle_receive(const boost::system::error_code &error, std::size_t)
{
    if (!error || error == boost::asio::error::message_size)
    {
        
        //Mandamos a llamar a la función que fabrica el string
        boost::shared_ptr<std::string> message(new std::string(make_string(recv_buffer_.data())));
        recv_buffer_.assign(0);
        socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                              boost::bind(&udp_server::handle_send, this, message,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
        //Regresamos al flujo original
        start_receive();
    }
}