
#ifndef __Tablero__
#define __Tablero__

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <stdlib.h>

using namespace std;

class Tablero
{
	public:

		struct barra
		{
			vector<SDL_Point> coordenadas;
		};

		struct tablero
		{
			barra barraSup;
			barra barraJ1;
			barra barraJ2;
			barra barraJ3;
			vector<barra> tablero;
			int num_jugador;
		};

		Tablero();
		~Tablero();
		void inicializar_juego();
		SDL_Point* obtenerBarra(int);
		void desplazamientoBarra(int);
		void inicializar_barras_jugadores();
		void inicializar_tablero();
		SDL_Point* obtenerSuperior();
		void set_num_jugador(int);
		void inicializar_barra(int,int,int,int,vector<SDL_Point> &);
		int get_size_tablero();
		SDL_Point* get_element_tablero(int i);
        Tablero::tablero obtenerTablero();
        void set_barra(barra b, int num);
private:
		struct tablero tab;
		int limiteX=800,limiteY=600;
};


#endif