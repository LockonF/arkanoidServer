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

//Funcion que se ejecuta asíncronamente una vez cada x tiempo

void udp_server::print()
{
    int count = participants_.size();
    
    
    //Para cada participante le envía el string
    for(auto participant: participants_)
    {
        
        //Este es un string de ejemplo, pero podemos enviar lo que sea
        boost::shared_ptr<std::string> message(new std::string("hola"));
        
        
        socket_.async_send_to(boost::asio::buffer(*message), participant,
                              boost::bind(&udp_server::handle_send, this, message,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
        
    }
    

    
    
    //Timer, le agrega una cantidad de segundos segundos al temporizador
    timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
    timer_.async_wait(boost::bind(&udp_server::print, this));
}


//Incializamos el servidor con el puerto, el servicio io y el endpoint. Le decimos que asíncronamente puede ejecutar una función cada n tiempo sin dejar de lado la recepción.
udp_server::udp_server(boost::asio::io_service &io_service, int port): socket_(io_service, udp::endpoint(udp::v4(), port)), timer_(io_service, boost::posix_time::seconds(1))
{
    timer_.async_wait(boost::bind(&udp_server::print, this));

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
        //Guardamos el remote endpoint, insert se asegura de que no haya repetidos
        participants_.insert(remote_endpoint_);
        
        
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