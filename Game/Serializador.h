//
//  Serializador.h
//  Poco_Server
//
//  Created by Daniel Franco on 7/12/15.
//  Copyright (c) 2015 Daniel. All rights reserved.
//

#ifndef __Poco_Server__Serializador__
#define __Poco_Server__Serializador__

#include <stdio.h>
#include <Poco/JSON/JSON.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <vector>
#include "tablero.h"


#endif /* defined(__Poco_Server__Serializador__) */


class Serializador{
public:
    static std::string serializarTablero(Tablero tab);
    static Tablero::tablero deserializarTablero(std::string);
};