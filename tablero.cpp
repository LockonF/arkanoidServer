#include "tablero.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <stdlib.h>
using namespace std;



Tablero::Tablero()
{
	
}

Tablero::~Tablero() 
{
	
}

void Tablero::inicializar_juego()
{
	inicializar_barras_jugadores();
	inicializar_tablero();
	set_num_jugador(1);
}

SDL_Point* Tablero::obtenerSuperior()
{
	return &tab.barraSup.coordenadas[0];	
}

SDL_Point* Tablero::obtenerBarra(int i)
{
	if(i==1)
	{
		return &tab.barraJ1.coordenadas[0];
	}
	else if(i==2)
	{
		return &tab.barraJ2.coordenadas[0];
	}
	else if(i==3)
	{
		return &tab.barraJ3.coordenadas[0];
	}
    return nullptr;
}

void Tablero::desplazamientoBarra(int cantidad)
{
	if(tab.num_jugador==1)
	{
		if((tab.barraJ1.coordenadas[0].x+cantidad!=0) && (tab.barraJ1.coordenadas[1].x+cantidad!=limiteX))
		{
			for(int i=0;i<=tab.barraJ1.coordenadas.size();i++)
			{
				tab.barraJ1.coordenadas[i].x=tab.barraJ1.coordenadas[i].x+cantidad;
			}
		}
	}
	if(tab.num_jugador==2)
	{
		if((tab.barraJ2.coordenadas[0].y+cantidad!=10) && (tab.barraJ2.coordenadas[1].y+cantidad!=limiteY))
		{
			for(int i=0;i<=tab.barraJ2.coordenadas.size();i++)
			{
				tab.barraJ2.coordenadas[i].y=tab.barraJ2.coordenadas[i].y+cantidad;
			}
		}
	}
	if(tab.num_jugador==3)
	{
		if((tab.barraJ3.coordenadas[0].y+cantidad!=10) && (tab.barraJ3.coordenadas[1].y+cantidad!=limiteY))
		{
			for(int i=0;i<=tab.barraJ3.coordenadas.size();i++)
			{
				tab.barraJ3.coordenadas[i].y=tab.barraJ3.coordenadas[i].y+cantidad;
			}
		}
	}
}

void Tablero::inicializar_barras_jugadores()
{
	inicializar_barra(400,590,100,10,tab.barraJ1.coordenadas);
	inicializar_barra(790,300,10,100,tab.barraJ2.coordenadas);
	inicializar_barra(10,300,10,100,tab.barraJ3.coordenadas);
	inicializar_barra(400,5,790,10,tab.barraSup.coordenadas);
}

void Tablero::inicializar_barra(int x,int y,int largo,int ancho,vector<SDL_Point> &coordenadas_barra)
{
	SDL_Point cN;
	cN.x=x-(largo/2);
	cN.y=y-(ancho/2);
	coordenadas_barra.push_back(cN);
	cN.x=x+(largo/2);
	cN.y=y-(ancho/2);
	coordenadas_barra.push_back(cN);
	cN.x=x+(largo/2);
	cN.y=y+(ancho/2);
	coordenadas_barra.push_back(cN);
	cN.x=x-(largo/2);
	cN.y=y+(ancho/2);
	coordenadas_barra.push_back(cN);

	coordenadas_barra.push_back(coordenadas_barra.front());
}

void Tablero::inicializar_tablero()
{
	int separacion_x=40;
	int separacion_y=10;
	int inicio_x=280;
	int inicio_y=150;
	
	for(int y=0;y<25;y++)
	{
		for(int x=0;x<7;x++)
		{
			barra bar;
			inicializar_barra(inicio_x+(x*separacion_x),inicio_y+(y*separacion_y),separacion_x,separacion_y,bar.coordenadas);
			tab.tablero.push_back(bar);
		}
	}

}

void Tablero::set_num_jugador(int i)
{
	tab.num_jugador=i;
}

int Tablero::get_size_tablero()
{
	return tab.tablero.size();
}

SDL_Point* Tablero::get_element_tablero(int i)
{
	return &tab.tablero[i].coordenadas[0];	
}

Tablero::tablero Tablero::obtenerTablero(){
    return tab;
}

void Tablero::set_barra(Tablero::barra b, int num)
{
    switch (num) {
        case 1:
            tab.barraJ1 = b;
            break;
        case 2:
            tab.barraJ2 = b;
            break;
        case 3:
            tab.barraJ3 = b;
            break;
        default:
            break;
    }
}


