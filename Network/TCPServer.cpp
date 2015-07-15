//
//  TCPServer.cpp
//  Poco_Server
//
//  Created by Daniel Franco on 7/13/15.
//  Copyright (c) 2015 Daniel. All rights reserved.
//

#include "TCPServer.h"

//Constructor de la clase TCPServer, inicializa el aceptor y con el io_service y el endpoint (versión de IP, puerto), además del socket que va a utilizar y manda a llamar a do_accept()
TCPServer::TCPServer(boost::asio::io_service& io_service,const tcp::endpoint& endpoint): acceptor_(io_service, endpoint),socket_(io_service)
{
    do_accept();
}

//Se hace la aceptación asíncrona y luego crea e inicializa una sesión de juego. Aquí se debe de limitar el número de jugadores para que solamente acepte tres

void TCPServer::do_accept()
{
    acceptor_.async_accept(socket_,
    [this](boost::system::error_code ec)
    {
        if (!ec)
        {
            std::make_shared<game_session>(std::move(socket_), room_)->start();
        }
        do_accept();
    });
}
