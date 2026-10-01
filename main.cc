#include <string>
#include <stdio.h>
#include <stdlib.h>
//graficos
#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>

//delay
#include <SDL2/SDL_timer.h>


Uint32* lerEntrada(int *altura, int *largura,const char* fileName)
{
	FILE* entrada = fopen(fileName, "r");
	if (!entrada) { printf("Nao abriu %s\n", fileName); return NULL; }
	// Le o tamanho da janela
	char linha[256];
	fgets(linha, sizeof(linha), entrada);
	sscanf(linha, "%d %d", altura, largura);
		
	int total = (*altura) * (*largura);
	Uint32 *pixels = (Uint32*) calloc(total, sizeof(Uint32));
	// Le a imagem e armazena 
	for (int i = 0; i < total; i++)
	{
		unsigned char r, g, b;
		if(fscanf(entrada, "%hhx %hhx %hhx", &r, &g, &b) != 3) break;
		pixels[i] = (0xFFu << 24) | ((Uint32)r << 16) | ((Uint32)g << 8) | b;

	}
	fclose(entrada);
	return pixels;
}

int main(int argc, char * argv []){
	printf("Hello, World!\n");
	
	int windowHeight = 1000;
	int windowWidth = 1000;	

	Uint32* pixels = lerEntrada(&windowHeight, &windowWidth, "entrada0.txt");


	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {printf("Error : %s\n", SDL_GetError());}

	SDL_Window* window = SDL_CreateWindow("Game", 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 
			windowWidth, 
			windowHeight, 
			SDL_WINDOW_SHOWN);
	
	int close = 0;
	SDL_Event event;
	SDL_Renderer *s = SDL_CreateRenderer(window,
						0,
						SDL_RENDERER_ACCELERATED | 
						SDL_RENDERER_PRESENTVSYNC);
	

	SDL_Texture *tex = SDL_CreateTexture(s, 
			SDL_PIXELFORMAT_ARGB8888, 
			SDL_TEXTUREACCESS_STATIC, 
			windowWidth, 
			windowHeight);
	SDL_UpdateTexture(tex, NULL, pixels, windowWidth * sizeof(Uint32));
	free(pixels);
	
	int fila = 0;
	int filaAnterior = 0;

	while(!close)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type) 
			{
				case SDL_QUIT:
					close = 1; 
					break;
				case SDL_KEYDOWN:
					if (event.key.repeat) break;
					if (event.key.keysym.sym == SDLK_RIGHT) {fila++;}
					if (event.key.keysym.sym == SDLK_LEFT) {fila--;}
					break;
				default: 
					break;
			}
		}
		if (filaAnterior != fila){
			std::string fotoAtual = "entrada" + std::to_string(fila) + ".txt";
			pixels = lerEntrada(&windowHeight, &windowWidth, fotoAtual.c_str());
			if (pixels)
			{
				SDL_DestroyTexture(tex);
				tex = SDL_CreateTexture(s, 
					SDL_PIXELFORMAT_ARGB8888, 
					SDL_TEXTUREACCESS_STATIC, 
					windowWidth, 
					windowHeight);
				SDL_UpdateTexture(tex, NULL, pixels, windowWidth * sizeof(Uint32));
				SDL_SetWindowSize(window, windowWidth, windowHeight);
				free(pixels);
			}
			else 
			{
				fila = filaAnterior;
			}
		}	
		filaAnterior = fila;
		SDL_SetRenderDrawColor(s, 0x00, 0x00, 0x00, 0xFF);	
		SDL_RenderClear(s);
		SDL_RenderCopy(s, tex, NULL, NULL);
		SDL_RenderPresent(s);

		SDL_Delay(16);
	}
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(s);	
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
