//
//  TCPServer.h
//  Poco_Server
//
//  Created by Daniel Franco on 7/13/15.
//  Copyright (c) 2015 Daniel. All rights reserved.
//

#ifndef __Poco_Server__TCPServer__
#define __Poco_Server__TCPServer__

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Message.h"

#endif /* defined(__Poco_Server__TCPServer__) */

using boost::asio::ip::tcp;

//Definimos la cola de mensajes
typedef std::deque<Message> message_queue;


//Clase base para el jugador. La game_session hereda de esta clase
class game_player
{
public:
    virtual ~game_player() {}
    virtual void deliver(const Message & msg) = 0;
};

typedef std::shared_ptr<game_player> game_player_ptr;

//Clase para la sala de juegos. Tiene funciones para unirse, dejar y entregar mensajes. La clase game_room controla el flujo entre todos los jugadores
class game_room{
public:
    
    void join(game_player_ptr participant)
    {
        participants_.insert(participant);
        for (auto msg: recent_msgs_)
            participant->deliver(msg);
    }
    
    void leave(game_player_ptr participant)
    {
        participants_.erase(participant);
    }
    
    void deliver(const Message& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();
        for (auto participant: participants_)
            participant->deliver(msg);
    }
private:
    std::set<game_player_ptr> participants_;
    enum { max_recent_msgs = 100 };
    message_queue recent_msgs_;
};


//Clase para la sesión de juego, que hereda de game_player. Aquí implementamos las funciones start y deliver. Efectivamente, la sesión del juego representa al jugador dentro del servidor

class game_session: public game_player, public std::enable_shared_from_this<game_session>
{
public:
    game_session(tcp::socket socket, game_room & room):socket_(std::move(socket)),room_(room){}
    
    //Al iniciar una sesión de juego se una a una sala de juego, y genera un apuntador compartido a this
    void start(){
        room_.join(shared_from_this());
        do_read_header();
    }
    void deliver(const Message &msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }

    }
    
private:
    
    //Esta es la función lee el header del socket y crea un buffer dinámico que varía con el tamaño del mensaje, si puede decodificar el mensaje (es decir, obtener el tamaño del mensaje con el header) entonces lee el resto.
    void do_read_header()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), Message::header_length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec && read_msg_.decode_header())
            {
                do_read_body();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }
    
    void do_read_body()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                //Aqui podemos procesar el mensaje. Deserializarlo y entregar un mensaje diferente a todos. En esta parte podemos mover el mensaje a entregar.
                room_.deliver(read_msg_);
                do_read_header();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }
    
    
    //Esta función escribe al socket. Generamos un buffer dinámico y enviamos. Luego matamos el mensaje de la cola.
    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
        boost::asio::buffer(write_msgs_.front().data(),write_msgs_.front().length()),[this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                {
                    do_write();
                }
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    tcp::socket socket_;
    game_room room_;
    Message read_msg_;
    message_queue write_msgs_;
};

//Clase para el servidor

class TCPServer{
public:
    TCPServer(boost::asio::io_service& io_service,const tcp::endpoint& endpoint);

    
private:
    void do_accept();
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    game_room room_;
};

