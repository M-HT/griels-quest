/* ending.c */

# include "ending.h"
# include "comun.h"

static SDL_Surface *load_ending_image (const char *filename) {
	SDL_Surface *temp, *ending;

	temp = IMG_Load(filename);
	ending = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	return ending;
}

void ending (SDL_Surface *screen, uint *state) {

	SDL_Surface *blackbox = NULL;
	SDL_Surface *ending[4] = { NULL, NULL, NULL, NULL };

	Mix_Music *bso;
	Mix_Chunk *argh;

	uint waittime = 0;
	uint step = 0;
	uint fadecounter = 255;
	uint counter = 0;
	int framerate = 0;

	/* Loading files */
#ifdef _RENDER_320_240
	blackbox = load_ending_image(DATA_PATH "png/blackbox.png");
	ending[0] = load_ending_image(DATA_PATH "png/ending1_small.png");
	ending[1] = load_ending_image(DATA_PATH "png/ending2_small.png");
	ending[2] = load_ending_image(DATA_PATH "png/ending3_small.png");
	ending[3] = load_ending_image(DATA_PATH "png/theend_small.png");
#else
	blackbox = load_ending_image(DATA_PATH "png/blackbox2.png");
	ending[0] = load_ending_image(DATA_PATH "png/ending1.png");
	ending[1] = load_ending_image(DATA_PATH "png/ending2.png");
	ending[2] = load_ending_image(DATA_PATH "png/ending3.png");
	ending[3] = load_ending_image(DATA_PATH "png/theend.png");
#endif
	argh = Mix_LoadWAV(DATA_PATH "fx/fx_uaaah.ogg");
	bso = Mix_LoadMUS(DATA_PATH "music/ending.ogg");

#ifdef _RENDER_320_240
	SDL_Rect dst = {32,8,0,0};
#else
	SDL_Rect srcending = {0,0,512,448};
	SDL_Rect destending = {0,0,512,448};
#endif

	while (*state == 3) {
		framerate = control_frames(1,0);

#ifdef _RENDER_320_240
		SDL_BlitSurface(ending[step],NULL,screen,&dst);
		/* Transparency */
		SDL_SetAlpha(blackbox,SDL_RLEACCEL|SDL_SRCALPHA,(Uint8)fadecounter);
		SDL_BlitSurface(blackbox,NULL,screen,&dst);
#else
		SDL_BlitSurface(ending[step],&srcending,screen,&destending);
		/* Transparency */
		SDL_SetAlpha(blackbox,SDL_RLEACCEL|SDL_SRCALPHA,(Uint8)fadecounter);
		SDL_BlitSurface(blackbox,&srcending,screen,&destending);
#endif

		switch (step) {
			case 0: /* ending 1 */
							counter ++;
							if (counter == 30)
								Mix_PlayMusic(bso,0);
							if (counter < 85)
								fadecounter-=3;
							if ((counter > 984) && (counter < 1069))
								fadecounter+=3;
							if (counter == 1069) {
								step = 1;
								fadecounter = 255;
							}
							break;
			case 1: /* ending 2 */
							counter ++;
							if (counter == 1070) /* Demon dying */
								Mix_PlayChannel(0,argh,0);
							if ((counter > 1069) && (counter < 1153))
								fadecounter-=3;
							if ((counter > 2053) && (counter < 2137))
								fadecounter+=3;
							if (counter == 2137) {
								step = 2;
								fadecounter = 255;
							}
							break;
			case 2: /* ending 3 */
							counter ++;
							if ((counter > 2137) && (counter < 2221))
								fadecounter-=3;
							if ((counter > 3121) && (counter < 3205))
								fadecounter+=3;
							if (counter == 3205) {
								step = 3;
								fadecounter = 255;
							}
							break;
			case 3: /* theend */
							counter ++;
							if ((counter > 3205) && (counter < 3289))
								fadecounter-=3;
							if (counter == 4189)
								*state = 0;
							break;
		}
		SDL_Flip(screen);
		control_frames(2,framerate);
		SDL_FreeSurface(screen);

	}

	SDL_FreeSurface(ending[0]);
	SDL_FreeSurface(ending[1]);
	SDL_FreeSurface(ending[2]);
	SDL_FreeSurface(ending[3]);
	SDL_FreeSurface(blackbox);
	Mix_FreeChunk(argh);
	Mix_FreeMusic(bso);

}
