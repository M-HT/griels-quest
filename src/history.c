/* history.c */

# include <stdlib.h>
# include <SDL_mixer.h>

# include "history.h"
# include "main.h"
# include "zoom.h"
# include "comun.h"

void history (SDL_Surface *screen, uint *state) {

	SDL_Surface *blackbox = NULL;
	SDL_Surface *pictures = NULL;
	SDL_Surface *texts = NULL;
	SDL_Surface *temp = NULL;
	SDL_Surface *window = NULL;

	SDL_Event keystroke;
	Mix_Music *bso;
	Mix_Chunk *lol;

	int framerate = 0;
	uint step = 0;
	uint direction = 0;
	uint waittime = 0;
	uint playmusic = 0;

	SDL_Rect srctitle = {0,0,256,16};
	SDL_Rect destitle = {0,104,256,16};
	SDL_Rect srcblackbox = {0,0,256,224};
	SDL_Rect desblackbox = {0,0,256,224};
	SDL_Rect srcpicture = {0,0,128,64};
	SDL_Rect despicture = {64,35,128,64};
	SDL_Rect srcsent1 = {0,16,0,8};
	SDL_Rect srcsent2 = {0,32,0,8};
	SDL_Rect srcsent3 = {0,48,0,8};
	SDL_Rect srcsent4 = {0,64,0,8};
	SDL_Rect destsent1 = {28,120,200,8};
	SDL_Rect destsent2 = {28,136,200,8};
	SDL_Rect destsent3 = {28,152,200,8};
	SDL_Rect destsent4 = {28,168,200,8};

	/* Loading files */
	blackbox = load_image_display_format(DATA_PATH "png/blackbox.png");
	window = SDL_ConvertSurface(blackbox, blackbox->format, blackbox->flags);
	pictures = load_image_display_format(DATA_PATH "png/howtoplay.png");
	texts = load_image_display_format(DATA_PATH "png/texts.png");
	bso = Mix_LoadMUS(DATA_PATH "music/history.ogg");
	lol = Mix_LoadWAV_RW(SDL_RWFromFile(DATA_PATH "fx/fx_hahaha.ogg", "rb"), 1);

	/* Loop */
	framerate = control_frames(1,0);
	while (*state == 1) {
		while (SDL_PollEvent(&keystroke)) {
			if (keystroke.type == SDL_QUIT)
				exit(0);
			if ((keystroke.type == SDL_KEYDOWN) && (!keystroke.key.repeat)) {
				if (keystroke.key.keysym.sym == KEY_QUIT)
					exit(0);
				if ((keystroke.key.keysym.sym == KEY_START) || (keystroke.key.keysym.sym == KEY_START2))
					*state = 2;
			}
		}
		switch (step) {
			case 0: /* show title */
							if (direction == 0) {
								if (desblackbox.x < 256)
									desblackbox.x += 2;
								else
									direction = 1;
							}
							if (direction == 1) {
								if (waittime < 120)
									waittime ++;
								else {
									waittime = 0;
									direction = 2;
								}
							}
							if (direction == 2) {
								if (desblackbox.x > 0)
									desblackbox.x -= 2;
								else {
									step = 1;
									waittime = 0;
								}
							}
							SDL_BlitSurface(texts,&srctitle,window,&destitle);
							SDL_BlitSurface(blackbox,&srcblackbox,window,&desblackbox);
							break;
			case 1: /* show history 1 */
							if (waittime == 60) /* Demon LOLed */
								Mix_PlayChannelTimed(0,lol,0,-1);
							if ((playmusic == 0) && (waittime == 180)) {
								playmusic = 1;
								Mix_PlayMusic(bso,0);
							}
							SDL_BlitSurface(blackbox,NULL,window,NULL);
							SDL_BlitSurface(pictures,&srcpicture,window,&despicture);
							if (waittime < 180)
								waittime ++;
							if (waittime > 179) {
								if (srcsent1.w < 200)
									srcsent1.w ++;
								SDL_BlitSurface(texts,&srcsent1,window,&destsent1);
								if (srcsent1.w == 200) {
									if (srcsent2.w < 200)
										srcsent2.w ++;
									SDL_BlitSurface(texts,&srcsent2,window,&destsent2);
								}
								if (srcsent2.w == 200) {
									if (srcsent3.w < 200)
										srcsent3.w ++;
									SDL_BlitSurface(texts,&srcsent3,window,&destsent3);
								}
								if (srcsent3.w == 200) {
									if (srcsent4.w < 200)
										srcsent4.w ++;
									SDL_BlitSurface(texts,&srcsent4,window,&destsent4);
								}
								if (srcsent4.w == 200) {
									if (waittime < 300)
										waittime ++;
									else {
										step = 2;
										waittime = 0;
										srcpicture.y = 64;
										srcsent1.y=72;
										srcsent2.y=88;
										srcsent3.y=104;
										srcsent4.y=120;
										srcsent1.w=0;
										srcsent2.w=0;
										srcsent3.w=0;
										srcsent4.w=0;
										destsent1.x=18;
										destsent2.x=18;
										destsent3.x=18;
										destsent4.x=18;
									}
								}
							}
							break;
			case 2: /* show history 2 */
							SDL_BlitSurface(blackbox,NULL,window,NULL);
							SDL_BlitSurface(pictures,&srcpicture,window,&despicture);
							if (srcsent1.w < 215)
								srcsent1.w ++;
							SDL_BlitSurface(texts,&srcsent1,window,&destsent1);
							if (srcsent1.w == 215) {
								if (srcsent2.w < 215)
									srcsent2.w ++;
								SDL_BlitSurface(texts,&srcsent2,window,&destsent2);
							}
							if (srcsent2.w == 215) {
								if (srcsent3.w < 215)
									srcsent3.w ++;
								SDL_BlitSurface(texts,&srcsent3,window,&destsent3);
							}
							if (srcsent3.w == 215) {
								if (srcsent4.w < 215)
									srcsent4.w ++;
								SDL_BlitSurface(texts,&srcsent4,window,&destsent4);
							}
							if (srcsent4.w == 215) {
								if (waittime < 120)
									waittime ++;
								else {
									step = 3;
									waittime = 0;
									srcpicture.y = 128;
									srcsent1.y=128;
									srcsent2.y=144;
									srcsent3.y=160;
									srcsent4.y=176;
									srcsent1.w=0;
									srcsent2.w=0;
									srcsent3.w=0;
									srcsent4.w=0;
								}
							}
							break;
			case 3: /* show history 3 */
							SDL_BlitSurface(blackbox,NULL,window,NULL);
							SDL_BlitSurface(pictures,&srcpicture,window,&despicture);
							if (srcsent1.w < 215)
								srcsent1.w ++;
							SDL_BlitSurface(texts,&srcsent1,window,&destsent1);
							if (srcsent1.w == 215) {
								if (srcsent2.w < 215)
									srcsent2.w ++;
								SDL_BlitSurface(texts,&srcsent2,window,&destsent2);
							}
							if (srcsent2.w == 215) {
								if (srcsent3.w < 215)
									srcsent3.w ++;
								SDL_BlitSurface(texts,&srcsent3,window,&destsent3);
							}
							if (srcsent3.w == 215) {
								if (srcsent4.w < 215)
									srcsent4.w ++;
								SDL_BlitSurface(texts,&srcsent4,window,&destsent4);
							}
							if (srcsent4.w == 215) {
								if (waittime < 120)
									waittime ++;
								else {
									*state = 2;
									Mix_HaltMusic();
								}
							}
							break;
		}
#ifdef _RENDER_320_240
		SDL_Rect dst = {32,8,0,0};
		SDL_BlitSurface(window,NULL,screen,&dst);
#else
		BlitDoubleSurface(window,screen);
#endif
		flip_screen(screen);
		framerate = control_frames(2,framerate);
	}

	/* Cleaning */
	SDL_FreeSurface(blackbox);
	SDL_FreeSurface(pictures);
	SDL_FreeSurface(texts);
	SDL_FreeSurface(window);
	Mix_FreeMusic(bso);
	Mix_FreeChunk(lol);

}
