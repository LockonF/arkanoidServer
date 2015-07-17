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

//Funcion que manda la estructura, se inicia si el tamaño = 2 por el momento, falta cambiarlo a 3
void udp_server::print()
{
    int size = participants_.size();
    if(size==2)
    {
        
    
    for(auto participant : participants_)
    {
        tablero.set_num_jugador(0);
        boost::shared_ptr<std::string> message(new std::string(Serializador::serializarTablero(tablero)));
        socket_.async_send_to(boost::asio::buffer(*message), participant,
                              boost::bind(&udp_server::handle_send, this, message,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));

    }
    }
    timer_.expires_at(timer_.expires_at() + boost::posix_time::milliseconds(10));
    timer_.async_wait(boost::bind(&udp_server::print, this));

}


//Incializamos el servidor con el puerto, el servicio io y el endpoint
udp_server::udp_server(boost::asio::io_service &io_service, int port): socket_(io_service, udp::endpoint(udp::v4(), port)), timer_(io_service, boost::posix_time::seconds(1))
{
    if(participants_.size()==0)
    {
        tablero.inicializar_juego();
    }

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
        //Si el jugador está registrado
        if(is_registered(remote_endpoint_))
        {
            std::string json_struct(recv_buffer_.data());
            Tablero::tablero deserialized_struct(Serializador::deserializarTablero(json_struct));
            switch (deserialized_struct.num_jugador) {
                case 1:
                    tablero.set_barra(deserialized_struct.barraJ1, 1);
                    break;
                case 2:
                    tablero.set_barra(deserialized_struct.barraJ2, 2);
                    break;
                case 3:
                    tablero.set_barra(deserialized_struct.barraJ3, 3);
                    break;
                default:
                    break;
            }
        }
        else{
            
        //Si no esta registrado pero es jugador
        if(is_player(recv_buffer_.data()))
        {
                //Mandamos a llamar a la función que fabrica el string
                boost::shared_ptr<std::string> message(new std::string(assign_player()));
                recv_buffer_.assign(0);
                //Registramos el endpoint como participante
                participants_.insert(remote_endpoint_);

                socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                                  boost::bind(&udp_server::handle_send, this, message,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
            
            
            }
        }
        recv_buffer_.assign(0);
        //Regresamos al flujo original
        start_receive();
        

    }
}


//Funcion que revisa si se le envió una cadena de jugador

bool udp_server::is_player(const char * data)
{
    if(participants_.size()+1==3)
        return false;
    if(strcmp(data,"jugador")==0)
        return true;
    return false;
}

//Funcion para saber si está registrado
bool udp_server::is_registered(udp::endpoint player)
{
    std::set<udp::endpoint>::iterator it;
    it = participants_.find(player);
    if(it!=participants_.end())
        return true;
    return false;
}

//Funcion para asignar el número de jugador

std::string udp_server::assign_player()
{
    int num_participants = participants_.size();
    tablero.set_num_jugador(num_participants+1);
    std::string string(Serializador::serializarTablero(tablero));
    return string;
}

