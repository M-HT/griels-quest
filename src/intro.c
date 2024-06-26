/* intro.c */

# include <stdlib.h>
# include <SDL_mixer.h>

# include "intro.h"
# include "main.h"
# include "comun.h"

static int passwvalidate (uint passint[]);

void game_intro (SDL_Renderer *renderer, uint *state, uint *level) {

	SDL_Surface *blackbox = NULL;
	SDL_Surface *karoshi = NULL;
	SDL_Surface *window = NULL;
	SDL_Surface *blocks = NULL;
	SDL_Surface *startscreen = NULL;
	SDL_Surface *startinfo = NULL;
	SDL_Surface *menu = NULL;
	SDL_Surface *arrow = NULL;
	SDL_Surface *passwords = NULL;
	SDL_Surface *fonts = NULL;
	Mix_Chunk *start = NULL;
	Mix_Chunk *poff = NULL;
	Mix_Chunk *error = NULL;
	Mix_Chunk *ding = NULL;
	SDL_Texture *window_texture = NULL;

	SDL_Event keystroke;

	/* Load files */
	blackbox = load_image_display_format(DATA_PATH "png/blackbox.png");
	window = SDL_ConvertSurface(blackbox, blackbox->format, 0);
	SDL_SetSurfaceBlendMode(blackbox, SDL_BLENDMODE_BLEND);
	karoshi = load_image_display_format(DATA_PATH "png/karoshi.png");
	startscreen = load_image_display_format(DATA_PATH "png/startscreen.png");
	startinfo = load_image_display_format_alpha(DATA_PATH "png/startinfo.png");
	blocks = load_image_display_format_alpha(DATA_PATH "png/blocks.png");
	menu = load_image_display_format(DATA_PATH "png/menu.png");
	passwords = load_image_display_format_alpha(DATA_PATH "png/password.png");
	fonts = load_image_display_format(DATA_PATH "png/fonts.png");
	arrow = SDL_ConvertSurface(fonts, fonts->format, 0);
	start = Mix_LoadWAV_RW(SDL_RWFromFile(DATA_PATH "fx/fx_start.ogg", "rb"), 1);
	poff = Mix_LoadWAV_RW(SDL_RWFromFile(DATA_PATH "fx/fx_poff.ogg", "rb"), 1);
	error = Mix_LoadWAV_RW(SDL_RWFromFile(DATA_PATH "fx/fx_error.ogg", "rb"), 1);
	ding = Mix_LoadWAV_RW(SDL_RWFromFile(DATA_PATH "fx/fx_ding.ogg", "rb"), 1);
	window_texture = SDL_CreateTexture(renderer, window->format->format, SDL_TEXTUREACCESS_STREAMING, window->w, window->h);

	SDL_Rect srcscreen = {0,0,256,224};
	SDL_Rect destscreen = {0,0,256,224};
	SDL_Rect srctext = {0,0,200,20};
	SDL_Rect destext = {28,20,200,20};
	SDL_Rect srcblocks = {0,0,16,16};
	SDL_Rect destblocks = {0,0,16,16};
	SDL_Rect srcarrow = {40,40,8,8};
	SDL_Rect desarrow = {80,88,8,8};
	SDL_Rect srcselector = {0,224,14,14};
	SDL_Rect destselector = {61,101,14,14};
	SDL_Rect srcletters = {0,0,16,16};
	SDL_Rect destletters = {64,64,16,16};

	uint step = 0;
	int framerate = 0;
	uint counter = 0;
	uint fadecounter = 255;
	uint animcounter = 0;
	uint posarrow = 0;
	uint selectorpos = 1;
	uint passint[8] = {0,0,0,0,0,0,0,0};
	uint n = 0;
	uint validatepass = 0;
	uint i = 0;
	uint result = 0;

	/* Loop */
	framerate = control_frames(1,0);
	while (*state == 0) {
		switch (step) {
			case 0: /* Karoshi logo */
							SDL_BlitSurface(karoshi,&srcscreen,window,&destscreen);
							/* Apply transparency */
							SDL_SetSurfaceAlphaMod(blackbox,(Uint8)fadecounter);
							SDL_BlitSurface(blackbox,&srcscreen,window,&destscreen);
							counter ++;
							while (SDL_PollEvent(&keystroke)) {
								if (keystroke.type == SDL_QUIT)
									exit(0);
								if ((keystroke.type == SDL_KEYDOWN) && (!keystroke.key.repeat)) {
									if (keystroke.key.keysym.sym == KEY_QUIT)
										exit(0);
									if ((keystroke.key.keysym.sym == KEY_START) || (keystroke.key.keysym.sym == KEY_START2)) {
										counter = 340;
										step = 1;
										fadecounter = 255;
									}
								}
							}
							if (counter < 86)
								fadecounter-=3;
							if ((counter > 254) && (counter < 340))
								fadecounter+=3;
							if (counter == 340) {
								step = 1;
								fadecounter = 255;
							}
							break;
			case 1: /* Press start screen */
							SDL_BlitSurface(startscreen,&srcscreen,window,&destscreen);
							/* Apply transparency */
							SDL_SetSurfaceAlphaMod(blackbox,(Uint8)fadecounter);
							SDL_BlitSurface(blackbox,&srcscreen,window,&destscreen);
							counter ++;
							while (SDL_PollEvent(&keystroke)) {
								if (keystroke.type == SDL_QUIT)
									exit(0);
								if ((keystroke.type == SDL_KEYDOWN) && (!keystroke.key.repeat)) {
									if (keystroke.key.keysym.sym == KEY_QUIT)
										exit(0);
									if ((keystroke.key.keysym.sym == KEY_START) || (keystroke.key.keysym.sym == KEY_START2)) {
										Mix_PlayChannelTimed(-1,start,0,-1);
										step = 3;
									}
								}
							}
							if ((counter > 340) && (counter < 426))
								fadecounter-=3;
							if ((counter > 1325) && (counter < 1387))
								fadecounter+=3;
							if (counter == 1387) {
								step = 2;
								fadecounter = 255;
							}
							break;
			case 2: /* Show instructions */
							SDL_BlitSurface(startinfo,&srcscreen,window,&destscreen);
							counter ++;
							while (SDL_PollEvent(&keystroke)) {
								if (keystroke.type == SDL_QUIT)
									exit(0);
								if ((keystroke.type == SDL_KEYDOWN) && (!keystroke.key.repeat)) {
									if (keystroke.key.keysym.sym == KEY_QUIT)
										exit(0);
									if ((keystroke.key.keysym.sym == KEY_START) || (keystroke.key.keysym.sym == KEY_START2)) {
										step = 1;
										counter = 340;
										fadecounter = 255;
									}
								}
							}
							if (animcounter < 59)
								animcounter ++;
							else
								animcounter = 0;
							/* Show animations */
							/* Hero */
							srcblocks.y = 32;
							srcblocks.x = (animcounter / 30) * 16 + 96;
							destblocks.x = 120;
							destblocks.y = 45;
							SDL_BlitSurface(blocks,&srcblocks,window,&destblocks);
							/* Demon */
							srcblocks.y = 16;
							srcblocks.x = (animcounter / 30) * 32 + 96;
							destblocks.x = 20;
							destblocks.y = 192;
							SDL_BlitSurface(blocks,&srcblocks,window,&destblocks);
							/* Ogre */
							srcblocks.x = (animcounter / 30) * 16 + 64;
							destblocks.y = 160;
							SDL_BlitSurface(blocks,&srcblocks,window,&destblocks);
							/* Ghost */
							srcblocks.x = (animcounter / 30) * 16 + 32;
							destblocks.y = 128;
							SDL_BlitSurface(blocks,&srcblocks,window,&destblocks);
							/* Slime */
							srcblocks.x = (animcounter / 30) * 16;
							destblocks.y = 96;
							SDL_BlitSurface(blocks,&srcblocks,window,&destblocks);
							if ((counter > 1387) && (counter < 1473))
								fadecounter-=3;
							if ((counter > 1387) && (counter < 1989)) {
								srctext.y = 224;
								srctext.h = 20;
								destext.h = 20;
								SDL_BlitSurface(startinfo,&srctext,window,&destext);
								SDL_SetSurfaceAlphaMod(blackbox,(Uint8)fadecounter);
								SDL_BlitSurface(blackbox,&srcscreen,window,&destscreen);
							}
							if ((counter > 1988) && (counter < 2590)) {
								srctext.y = 244;
								srctext.h = 21;
								destext.h = 21;
								SDL_BlitSurface(startinfo,&srctext,window,&destext);
							}
							if ((counter > 2589) && (counter < 3275)) {
								srctext.y = 265;
								SDL_BlitSurface(startinfo,&srctext,window,&destext);
							}
							if ((counter > 3190) && (counter < 3275)) {
								fadecounter += 3;
								SDL_SetSurfaceAlphaMod(blackbox,(Uint8)fadecounter);
								SDL_BlitSurface(blackbox,&srcscreen,window,&destscreen);
							}
							if (counter == 3274) {
								step = 0;
								counter = 0;
								animcounter = 0;
								fadecounter = 255;
							}
							break;
			case 3: /* show menu */
							SDL_BlitSurface(menu,NULL,window,NULL);
							if (posarrow == 0)
								desarrow.y = 88;
							else
								desarrow.y = 104;
							SDL_BlitSurface(arrow,&srcarrow,window,&desarrow);
							while (SDL_PollEvent(&keystroke)) {
								if (keystroke.type == SDL_QUIT)
									exit(0);
								if ((keystroke.type == SDL_KEYDOWN) && (!keystroke.key.repeat)) {
									if (keystroke.key.keysym.sym == KEY_QUIT)
										exit(0);
									if ((keystroke.key.keysym.sym == SDLK_UP) || (keystroke.key.keysym.sym == SDLK_DOWN)) {
										if (posarrow == 0)
											posarrow = 1;
										else
											posarrow = 0;
									}
									if ((keystroke.key.keysym.sym == KEY_START) || (keystroke.key.keysym.sym == KEY_START2)) {
										if (posarrow == 0) {
											*state = 1;
											*level = 1;
										}
										if (posarrow == 1)
											step = 4;
										Mix_PlayChannelTimed(-1,start,0,-1);
									}
								}
							}
							break;
			case 4: /* show password selection */
							SDL_BlitSurface(passwords,&srcscreen,window,&destscreen);
							SDL_BlitSurface(passwords,&srcselector,window,&destselector);
							while (SDL_PollEvent(&keystroke)) {
								if (keystroke.type == SDL_QUIT)
									exit(0);
								if ((keystroke.type == SDL_KEYDOWN) && (!keystroke.key.repeat)) {
									if (keystroke.key.keysym.sym == KEY_QUIT)
										exit(0);
									if (keystroke.key.keysym.sym == SDLK_RIGHT) {
										if (destselector.x < 173) {
											destselector.x += 16;
											selectorpos ++;
										}
										else {
											destselector.x -= 112;
											selectorpos -= 7;
										}
									}
									if (keystroke.key.keysym.sym == SDLK_LEFT) {
										if (destselector.x > 61) {
											destselector.x -= 16;
											selectorpos --;
										}
										else {
											destselector.x += 112;
											selectorpos += 7;
										}
									}
									if (keystroke.key.keysym.sym == SDLK_UP) {
										if (destselector.y > 101) {
											destselector.y -= 16;
											selectorpos -= 8;
										}
										else {
											destselector.y += 64;
											selectorpos += 32;
										}
									}
									if (keystroke.key.keysym.sym == SDLK_DOWN) {
										if (destselector.y < 165) {
											destselector.y += 16;
											selectorpos += 8;
										}
										else {
											destselector.y -= 64;
											selectorpos -= 32;
										}
									}
									if ((keystroke.key.keysym.sym == KEY_START) || (keystroke.key.keysym.sym == KEY_START2)) {
										if (selectorpos < 37) {
											passint[n] = selectorpos;
											if (n < 7)
												n ++;
											else
												n = 0;
											Mix_PlayChannelTimed(-1,start,0,-1);
										}
										if (selectorpos == 37) { /* Tilde */
											passint[n] = 0;
											if (n < 7)
												n ++;
											else
												n = 0;
											Mix_PlayChannelTimed(-1,start,0,-1);
										}
										if (selectorpos == 38) { /* Delete */
											if (n > 0) {
												passint[n-1] = 0;
												n --;
											}
											else {
												n = 7;
												passint[n] = 0;
											}
											Mix_PlayChannelTimed(-1,poff,0,-1);
										}
										if (selectorpos == 39) /* Ok key */
											validatepass = 1;
									}
								}
							}
							/* Showing characters */
							for (i=0;i<8;i++) {
								if (passint[i] > 0) {
									if (passint[i] < 11) {
										srcletters.y = 80;
										srcletters.x = (passint[i] - 1) * 16;
									}
									if ((passint[i] > 10) && (passint[n] < 21)) {
										srcletters.y = 96;
										srcletters.x = (passint[i] - 11) * 16;
									}
									if ((passint[i] > 20) && (passint[n] < 27)) {
										srcletters.y = 112;
										srcletters.x = (passint[i] - 21) * 16;
									}
									if ((passint[i] > 26) && (passint[i] <  37)) {
										srcletters.y = 64;
										srcletters.x = (passint[i] - 27) * 16;
									}
									destletters.x = 64 + (i * 16);
									destletters.y = 64;
									SDL_BlitSurface(fonts,&srcletters,window,&destletters);
								}
							}
							/* Showing character position */
							srcletters.x = 96;
							srcletters.y = 112;
							destletters.x = 64 + (n * 16);
							destletters.y = 80;
							SDL_BlitSurface(fonts,&srcletters,window,&destletters);
							/* Checking if password is correct */
							if (validatepass == 1) {
								result = passwvalidate(passint);
								validatepass = 0;
								if (result > 0) {
									*level = result;
									Mix_PlayChannelTimed(-1,ding,0,-1);
									*state = 2;
								}
								else
									Mix_PlayChannelTimed(-1,error,0,-1);
							}
							break;
		}
		SDL_UpdateTexture(window_texture, NULL, window->pixels, window->pitch);
		SDL_RenderClear(renderer);
#ifdef _RENDER_320_240
		SDL_Rect dst = {32,8,256,224};
		SDL_RenderCopy(renderer, window_texture, NULL, &dst);
#else
		SDL_RenderCopy(renderer, window_texture, NULL, NULL);
#endif
		SDL_RenderPresent(renderer);
		framerate = control_frames(2,framerate);
	}

	/* Cleaning */
	SDL_DestroyTexture(window_texture);
	SDL_FreeSurface(karoshi);
	SDL_FreeSurface(blackbox);
	SDL_FreeSurface(blocks);
	SDL_FreeSurface(startscreen);
	SDL_FreeSurface(startinfo);
	SDL_FreeSurface(passwords);
	SDL_FreeSurface(fonts);
	Mix_FreeChunk(start);
	Mix_FreeChunk(poff);
	Mix_FreeChunk(error);
	Mix_FreeChunk(ding);

}

static int passwvalidate (uint passint[]) {

	int n = 0;

	if ((passint[0] == 7) && (passint[1] == 5) && (passint[2] == 14) && (passint[3] == 5) && (passint[4] == 19) && (passint[5] == 9) && (passint[6] == 19) && (passint[7] == 0))
		n = 6;

	if ((passint[0] == 16) && (passint[1] == 19) && (passint[2] == 1) && (passint[3] == 12) && (passint[4] == 13) && (passint[5] == 19) && (passint[6] == 0) && (passint[7] == 0))
		n = 11;

	if ((passint[0] == 18) && (passint[1] == 15) && (passint[2] == 13) && (passint[3] == 1) && (passint[4] == 14) && (passint[5] == 19) && (passint[6] == 0) && (passint[7] == 0))
		n = 16;

	if ((passint[0] == 10) && (passint[1] == 1) && (passint[2] == 13) && (passint[3] == 5) && (passint[4] == 19) && (passint[5] == 0) && (passint[6] == 0) && (passint[7] == 0))
		n = 21;

	if ((passint[0] == 5) && (passint[1] == 3) && (passint[2] == 3) && (passint[3] == 12) && (passint[4] == 5) && (passint[5] == 19) && (passint[6] == 9) && (passint[7] == 1))
		n = 26;

	if ((passint[0] == 8) && (passint[1] == 5) && (passint[2] == 2) && (passint[3] == 18) && (passint[4] == 5) && (passint[5] == 23) && (passint[6] == 19) && (passint[7] == 0))
		n = 31;

	if ((passint[0] == 10) && (passint[1] == 15) && (passint[2] == 8) && (passint[3] == 14) && (passint[4] == 0) && (passint[5] == 0) && (passint[6] == 0) && (passint[7] == 0))
		n = 37;

	if ((passint[0] == 16) && (passint[1] == 18) && (passint[2] == 15) && (passint[3] == 22) && (passint[4] == 5) && (passint[5] == 18) && (passint[6] == 2) && (passint[7] == 19))
		n = 43;

	if ((passint[0] == 9) && (passint[1] == 19) && (passint[2] == 1) && (passint[3] == 9) && (passint[4] == 1) && (passint[5] == 8) && (passint[6] == 0) && (passint[7] == 0))
		n = 49;

	if ((passint[0] == 13) && (passint[1] == 1) && (passint[2] == 20) && (passint[3] == 20) && (passint[4] == 8) && (passint[5] == 5) && (passint[6] == 23) && (passint[7] == 0))
		n = 55;

	return n;

}
