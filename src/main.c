/* Griel's Quest for the Sangraal */
/* Started on 17/04/2013 */
/* Under GPL v3 license */

# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>
# include <SDL_image.h>
# include <SDL_mixer.h>

# include "main.h"
# include "ending.h"
# include "game.h"
# include "history.h"
# include "intro.h"
# include "comun.h"

static void initsdl(void);

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_PixelFormat *screen_format, *alpha_format;

#undef main
int main() {

	uint state = 0;
	uint level = 0;
	Uint32 Rmask, Gmask, Bmask, Amask;

	/* starting SDL */
	initsdl();
#ifdef _FULLSCREEN_ONLY
	#define SDL_FLAGS SDL_WINDOW_FULLSCREEN_DESKTOP
#else
	#define SDL_FLAGS 0
#endif
#ifdef _RENDER_320_240
	#define SDL_WIDTH 320
	#define SDL_HEIGHT 240
#else
	#define SDL_WIDTH 512
	#define SDL_HEIGHT 448
#endif
	window = SDL_CreateWindow("Griel's Quest for the Sangraal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SDL_WIDTH, SDL_HEIGHT, SDL_FLAGS | SDL_WINDOW_HIDDEN);
	if (window == NULL) {
		printf("Error creating window: %s\n",SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("Error creating renderer: %s\n",SDL_GetError());
		return 1;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_RenderSetLogicalSize(renderer, SDL_WIDTH, SDL_HEIGHT);

	screen_format = SDL_AllocFormat(SDL_MasksToPixelFormatEnum(32, 0, 0, 0, 0));
	if (screen_format == NULL) {
		printf("Error creating screen format: %s\n",SDL_GetError());
		return 1;
	}

	Rmask = screen_format->Rmask;
	Gmask = screen_format->Gmask;
	Bmask = screen_format->Bmask;
	Amask = ~(Rmask | Gmask | Bmask);
	alpha_format = SDL_AllocFormat(SDL_MasksToPixelFormatEnum(32, Rmask, Gmask, Bmask, Amask));
	if (alpha_format == NULL) {
		printf("Error creating alpha format: %s\n",SDL_GetError());
		return 1;
	}

	SDL_ShowWindow(window);

	/* Disable cursor */
	SDL_ShowCursor(SDL_DISABLE);

	/* Loading part of the game */
	while (state < 4) {
		switch (state) {
			case 0: game_intro (renderer, &state, &level);
							break;
			case 1: history (renderer, &state);
							break;
			case 2: game (renderer, &state, &level);
							break;
			case 3: ending (renderer,&state);
							break;
		}
	}

	return 0;
}

static void initsdl(void) {
	int result;

	/* Init SDLVideo */
	if (SDL_Init(SDL_INIT_VIDEO)<0) {
		printf("Error starting SDL: %s\n",SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	/* Init SDLImage */
	if (IMG_Init(IMG_INIT_PNG) < 0) {
		printf("Error starting SDL_image: %s\n",IMG_GetError());
		exit(1);
	}
	atexit(IMG_Quit);

	/* Init SDLMixer */
	if (Mix_Init(MIX_INIT_OGG) < 0) {
		printf("Error starting SDL_mixer: %s\n",Mix_GetError());
		exit(1);
	}
	atexit(Mix_Quit);

#if SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL) >= SDL_VERSIONNUM(2,0,2)
	const SDL_version *link_version = Mix_Linked_Version();
	if (SDL_VERSIONNUM(link_version->major, link_version->minor, link_version->patch) >= SDL_VERSIONNUM(2,0,2)) {
		result = Mix_OpenAudioDevice(44100, AUDIO_S16, 1, 4096, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
	}
	else
#endif
	result = Mix_OpenAudio(44100, AUDIO_S16, 1, 4096);

	if (result != 0) {
		printf("No se pudo inicializar SDL_Mixer %s\n",Mix_GetError());
		exit(1);
	}
	Mix_AllocateChannels(3);
	atexit(Mix_CloseAudio);
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

SDL_Surface *load_image_display_format (const char *filename) {
	SDL_Surface *temp, *image;

	temp = IMG_Load(filename);
	image = SDL_ConvertSurface(temp, screen_format, 0);
	SDL_FreeSurface(temp);

	return image;
}

SDL_Surface *load_image_display_format_alpha (const char *filename) {
	SDL_Surface *temp, *image;

	temp = IMG_Load(filename);
	image = SDL_ConvertSurface(temp, alpha_format, 0);
	SDL_FreeSurface(temp);

	return image;
}

void change_fullscreen (void) {
	SDL_SetWindowFullscreen(window, (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}

