//
//  Serializador.cpp
//  Poco_Server
//
//  Created by Daniel Franco on 7/12/15.
//  Copyright (c) 2015 Daniel. All rights reserved.
//

#include "Serializador.h"

std::string Serializador::serializarTablero(Tablero tab)
{
    Poco::JSON::Object::Ptr objetoJSON = new Poco::JSON::Object;
    Poco::JSON::Object interno;
    Tablero::tablero estructura = tab.obtenerTablero();
    
    //Iteradores
    std::vector<SDL_Point>::iterator principio;
    std::vector<SDL_Point>::iterator fin;
    
    
    //Cuenta
    int cuentaBarra = 0;
    int cuentaCoord = 0;
    
    //Cadenas
    std::string stringBarra = "barra";
    
    //Convertimos el número de jugador, así podemos convertir cada cosa que no sea un vector
    objetoJSON->set("num_jugador", estructura.num_jugador);
    
    //Convertimos la barras a JSON
    
    for(cuentaBarra=0;cuentaBarra<4;cuentaBarra++)
    {
        cuentaCoord=1;
        switch (cuentaBarra) {
            case 0:
                principio = estructura.barraSup.coordenadas.begin();
                fin = estructura.barraSup.coordenadas.end();
                break;
            case 1:
                principio = estructura.barraJ1.coordenadas.begin();
                fin = estructura.barraJ1.coordenadas.end();
                break;
            case 2:
                principio = estructura.barraJ2.coordenadas.begin();
                fin = estructura.barraJ2.coordenadas.end();
                break;
            case 3:
                principio = estructura.barraJ3.coordenadas.begin();
                fin = estructura.barraJ3.coordenadas.end();
                break;
            default:
                break;
        }
        
        for(;principio!=fin;principio++)
        {
            interno.set("x"+std::to_string(cuentaCoord),principio->x);
            interno.set("y"+std::to_string(cuentaCoord),principio->y);
            cuentaCoord++;
            if(cuentaCoord==5)
                break;
        }
        objetoJSON->set(stringBarra+std::to_string(cuentaBarra), interno);
        interno.clear();
    }
    
    std::ostringstream os;
    objetoJSON->stringify(os);
    return os.str();
}



Tablero::tablero Serializador::deserializarTablero(std::string jsonString)
{
    Tablero::tablero estructura;
    Poco::JSON::Parser jsonParser;
    Poco::Dynamic::Var parsedJSON = jsonParser.parse(jsonString);
    Poco::Dynamic::Var parsedResult = jsonParser.result();
    
    bool isX = true;
    SDL_Point coordenada[4];
    std::vector<SDL_Point> vector;
    
    Tablero::barra barraSup;
    Tablero::barra barraJ1;
    Tablero::barra barraJ2;
    Tablero::barra barraJ3;
    
    //Obtenemos los campos no anidados
    
    Poco::DynamicStruct jsonStruct = *parsedResult.extract<Poco::JSON::Object::Ptr>();
    estructura.num_jugador = jsonStruct["num_jugador"].convert<int>();
    
    
    // Obtenemos los campos anidados
    for(int cuentaBarra=0;cuentaBarra<4;cuentaBarra++)
    {
        
        string keyStr = "barra"+std::to_string(cuentaBarra);
        Poco::JSON::Object::Ptr jsonObject = parsedResult.extract<Poco::JSON::Object::Ptr>();
        
        //Buscamos la variable anidada
        Poco::Dynamic::Var metaVar = jsonObject->get(keyStr);
        Poco::JSON::Object::Ptr metaObj = metaVar.extract<Poco::JSON::Object::Ptr>();
        
        //Para resetear el número de x
        int numcoord=0;
        //El iterador tiene dos valores: first(campo) second(valor)
        for (Poco::JSON::Object::ConstIterator itr = metaObj->begin(), end = metaObj->end(); itr != end; ++itr)
        {
            
            if(numcoord<4)
            {
                coordenada[numcoord].x = itr->second.convert<int>();
                numcoord++;
                
            }
            else{
                
                coordenada[numcoord-4].y = itr->second.convert<int>();
                vector.push_back(coordenada[numcoord-4]);
                numcoord++;
            }
            
        }
        
        switch (cuentaBarra) {
            case 0:
                barraSup.coordenadas = vector;
                estructura.barraSup= barraSup;
                vector.clear();
                
                break;
            case 1:
                barraJ1.coordenadas = vector;
                vector.clear();
                estructura.barraJ1 = barraJ1;
                break;
            case 2:
                barraJ2.coordenadas = vector;
                vector.clear();
                estructura.barraJ2 = barraJ2;
                break;
            case 3:
                barraJ3.coordenadas = vector;
                vector.clear();
                estructura.barraJ3 = barraJ3;
                break;
            default:
                break;
        }
    }
    
    
    return estructura;
}