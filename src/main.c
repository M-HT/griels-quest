/* Griel's Quest for the Sangraal */
/* Started on 17/04/2013 */
/* Under GPL v3 license */

# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>
# include <SDL_ttf.h>
# include <SDL_getenv.h>
# include <SDL_mixer.h>

# include "main.h"
# include "ending.h"
# include "game.h"
# include "history.h"
# include "intro.h"
# include "comun.h"

static void initsdl(void);

#undef main
int main() {

	SDL_Surface *screen = NULL;
	uint state = 0;
	uint level = 0;

	/* starting SDL */
	initsdl();
#ifdef _FULLSCREEN_ONLY
	#define SDL_FLAGS SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN
#else
	#define SDL_FLAGS SDL_HWSURFACE|SDL_DOUBLEBUF
#endif
#ifdef _RENDER_320_240
	screen = SDL_SetVideoMode(320,240,32,SDL_FLAGS);
#else
	screen = SDL_SetVideoMode(512,448,32,SDL_FLAGS);
#endif

	/* Loading part of the game */
	while (state < 4) {
		switch (state) {
			case 0: game_intro (screen, &state, &level);
							break;
			case 1: history (screen, &state);
							break;
			case 2: game (screen, &state, &level);
							break;
			case 3: ending (screen,&state);
							break;
		}
	}

	return 0;
}

static void initsdl(void) {

	/* Center the window */
	putenv("SDL_VIDEO_CENTERED=1");

	/* Init SDLVideo */
		if (SDL_Init(SDL_INIT_VIDEO)<0) {
			printf("Error starting SDL: %s\n",SDL_GetError());
			exit(1);
		}
		atexit(SDL_Quit);
	/* Init SDLTTF */
	  if (TTF_Init() < 0) {
			printf("Error starting SDL_ttf: %s\n",SDL_GetError());
			exit(1);
		}
		atexit(TTF_Quit);

	  if (Mix_OpenAudio (44100, AUDIO_S16, 1, 4096)) {
        printf("No se pudo inicializar SDL_Mixer %s\n",Mix_GetError());
        exit(1);
		}
		Mix_AllocateChannels(3);
		atexit(Mix_CloseAudio);

	/* Name of the window */
		SDL_WM_SetCaption("Griel's Quest for the Sangraal", "Griel's Quest");

	/* Disable cursor */
		SDL_ShowCursor(SDL_DISABLE);

	/* Disable repeat keys */
	  SDL_EnableKeyRepeat(0, 0);

}

int control_frames (int i, int frate) {

	int now = SDL_GetTicks();

	if (i==1)
		return now;

	if (i==2) {
		int diff = now - frate;
		if (diff>=16)
			return (diff<=32)?frate+16:now-16;
		do {
			SDL_Delay(1);
			now = SDL_GetTicks();
			diff = now - frate;
		} while (diff<16);
		return frate+16;
	}

	return 0;
}
