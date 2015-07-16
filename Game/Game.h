// Game.h
// Es la interfaz de la clase main, en donde se declaran los metodos necesarios para cargar nuestra aplicacion 

#ifndef __Game__
#define __Game__

#include <iostream>
#include <SDL2/SDL.h>
#include "Serializador.h"
class Game
{
	public:
		Game();
		~Game();
		void init(const char *, int ,int ,int ,int ,int );
		void render();
		void update();
		void handleEvents();
		void clean();
		bool running();
	private:
		bool m_bRunning;
		SDL_Window* m_pWindow;
		SDL_Renderer* m_pRenderer;
		SDL_Event event;				//Detecta la accion de cierre de la ventana
		const Uint8 * keystate;
		Tablero tab;
        //Implementación del servidor

    
};


#endif