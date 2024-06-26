/* game.c */

# include <stdlib.h>
# include <SDL_image.h>
# include <SDL_mixer.h>

# include "game.h"
# include "hero.h"
# include "hud.h"
# include "loading.h"
# include "main.h"
# include "comun.h"

static void show_tiles (struct hero *griel, int *animationtime, int map[][11][16], SDL_Surface *window, SDL_Surface *blocks, int round, int counter, Mix_Chunk *key);
static void check_obstacles (struct hero *griel, int round, int map[][11][16], Mix_Chunk *kill, uint *grieltouch);
static void controls (struct hero *griel, uint *fullscreench);

static Mix_Music *bsogame = NULL;

static SDL_Event repeat_key_event;
static Uint32 repeat_key_next_ticks = 0;

void game (SDL_Renderer *renderer, uint *state, uint *level) {

	SDL_Surface *roundscreen = NULL;
	SDL_Surface *blocks = NULL;
	SDL_Surface *headgame = NULL;
	SDL_Surface *fonts = NULL;
	SDL_Surface *blackbox = NULL;
	SDL_Surface *window = NULL;
	SDL_Surface *gameoverscreen = NULL;
	SDL_Surface *passscreen01 = NULL;
	SDL_Surface *passscreen02 = NULL;
	SDL_Surface *passscreen03 = NULL;
	SDL_Surface *passscreen04 = NULL;
	SDL_Surface *passscreen05 = NULL;
	SDL_Surface *passscreen06 = NULL;
	SDL_Surface *passscreen07 = NULL;
	SDL_Surface *passscreen08 = NULL;
	SDL_Surface *passscreen09 = NULL;
	SDL_Surface *passscreen10 = NULL;
	SDL_Texture *window_texture = NULL;

	Mix_Music *gameover;
	Mix_Chunk *stageclear;
	Mix_Chunk *giveup;
	Mix_Chunk *key;
	Mix_Chunk *kill;

	int map[58][11][16];

	uint step = 0;
	uint waittime = 0;
	uint fadecounter = 255;
	int framerate = 0;
	uint fademode = 0;
	int round = (*level - 1);
	int counter = 0;
	int animationtime = 0;
	uint loadoninit = 0;
	uint uplife = 0;
	uint soundblock = 0;
	uint grieltouch = 0;
	uint fullscreench = 0;

	/* load files */
	roundscreen = load_image_display_format(DATA_PATH "png/round.png");
	blocks = load_image_display_format_alpha(DATA_PATH "png/blocks.png");
	headgame = load_image_display_format(DATA_PATH "png/gamehead.png");
	fonts = load_image_display_format(DATA_PATH "png/fonts.png");
	blackbox = load_image_display_format(DATA_PATH "png/blackbox.png");
	window = SDL_ConvertSurface(blackbox, blackbox->format, 0);
	SDL_SetSurfaceBlendMode(blackbox, SDL_BLENDMODE_BLEND);
	gameoverscreen = load_image_display_format(DATA_PATH "png/gameover.png");
	passscreen01 = load_image_display_format(DATA_PATH "png/passw1.png");
	passscreen02 = load_image_display_format(DATA_PATH "png/passw2.png");
	passscreen03 = load_image_display_format(DATA_PATH "png/passw3.png");
	passscreen04 = load_image_display_format(DATA_PATH "png/passw4.png");
	passscreen05 = load_image_display_format(DATA_PATH "png/passw5.png");
	passscreen06 = load_image_display_format(DATA_PATH "png/passw6.png");
	passscreen07 = load_image_display_format(DATA_PATH "png/passw7.png");
	passscreen08 = load_image_display_format(DATA_PATH "png/passw8.png");
	passscreen09 = load_image_display_format(DATA_PATH "png/passw9.png");
	passscreen10 = load_image_display_format(DATA_PATH "png/passw10.png");
	stageclear = Mix_LoadWAV_RW(SDL_RWFromFile(DATA_PATH "music/stageclear.ogg", "rb"), 1);
	gameover = Mix_LoadMUS(DATA_PATH "music/gameover.ogg");
	giveup = Mix_LoadWAV_RW(SDL_RWFromFile(DATA_PATH "fx/fx_giveup.ogg", "rb"), 1);
	key = Mix_LoadWAV_RW(SDL_RWFromFile(DATA_PATH "fx/fx_key.ogg", "rb"), 1);
	kill = Mix_LoadWAV_RW(SDL_RWFromFile(DATA_PATH "fx/fx_kill.ogg", "rb"), 1);
	window_texture = SDL_CreateTexture(renderer, window->format->format, SDL_TEXTUREACCESS_STREAMING, window->w, window->h);

	/* load map data */
	loaddata(map);

	/* init array */
	struct hero griel = {
		.score = 0,
		.lifes = 3,
		.object = 0,
		.key = 0,
		.x = 112,
		.y = 48,
		.positionx = 7,
		.positiony = 1,
		.direction = 2,
		.locked = 0,
		.movement = 0,
		.endanimation = 0,
		.deathanimation = 0,
		.deathturns = 0,
		.killed = 0,
		.grial = 0,
	};

	SDL_Rect srcfonts = {0,128,8,8};
	SDL_Rect desfonts = {136,96,8,8};

	framerate = control_frames(1,0);
	while (*state == 2) {
		if (counter < 58)
			counter ++;
		else
			counter = 0;
		switch (step) {
			case 0: /* Show round screen */
							*level = round + 1;
							desfonts.x = 144;
							SDL_BlitSurface(roundscreen,NULL,window,NULL);
							if (*level < 10) {
								srcfonts.x = 0;
								SDL_BlitSurface(fonts,&srcfonts,window,&desfonts);
								srcfonts.x = *level * 8;
								desfonts.x = 152;
								SDL_BlitSurface(fonts,&srcfonts,window,&desfonts);
							}
							else {
								srcfonts.x = (*level / 10) * 8;
								SDL_BlitSurface(fonts,&srcfonts,window,&desfonts);
								srcfonts.x = (*level - ((*level / 10) * 10)) * 8;
								desfonts.x = 152;
								SDL_BlitSurface(fonts,&srcfonts,window,&desfonts);
							}
							/* Apply transparency */
							SDL_SetSurfaceAlphaMod(blackbox,(Uint8)fadecounter);
							SDL_BlitSurface(blackbox,NULL,window,NULL);
							if ((fademode == 0) && (fadecounter == 255)) {
								/* Disable push keys until game start */
								SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
							}
							if ((fademode == 0) && (fadecounter > 0))
								fadecounter -= 3;
							if ((fademode == 1) && (fadecounter < 255))
								fadecounter += 3;
							if ((fademode == 0) && (fadecounter == 0))
								fademode = 1;
							if ((fademode == 1) && (fadecounter == 255))
								waittime ++;
							if (waittime == 60) {
								step = 1;
								waittime = 0;
								loadoninit = 1;
								fademode = 0;
								SDL_EventState(SDL_KEYDOWN, SDL_ENABLE); /* Enable pushes keys */
								controls(NULL,NULL); /* flush pushed keys */
							}
							break;
			case 1: /* If stage starts now, load music */
							if (loadoninit == 1) {
								bsogame = load_music(bsogame,round);
								loadoninit = 0;
								set_hero_init (&griel, round);
							}
							/* If hero have killed, reload map */
							if (griel.killed == 1) {
								loaddata(map);
								griel.killed = 0;
							}
							/* When hero exits, play stage clear sound */
							if (waittime == 1)
								Mix_PlayChannelTimed(-1,stageclear,0,-1);
							/* Checking score for extra life (every 5000 points), 4 lifes max. */
							extralife (&griel, &uplife);
							/* Show game screen */
							SDL_BlitSurface(headgame,NULL,window,NULL);
							/* Show hud */
							show_hud (griel, fonts, window, blocks, round);
							/* Show titles */
							show_tiles (&griel, &animationtime, map, window, blocks, round, counter, key);
							/* check for obstacles */
							check_obstacles (&griel, round, map, kill, &grieltouch);
							/* Griel touched ? */
							if (grieltouch == 1)
								*state = 3;
							/* Show hero */
							show_hero(&griel, counter, window, blocks, &round, &step, &waittime, &soundblock, giveup);
							/* Key pressed */
							controls(&griel,&fullscreench);
#ifndef _FULLSCREEN_ONLY
							if (fullscreench == 1) {
								change_fullscreen ();
								fullscreench = 0;
							}
#endif
							break;
			case 2: /* gameover screen for 10 seconds */
							if (waittime < 600) {
								waittime ++;
								SDL_BlitSurface(gameoverscreen,NULL,window,NULL);
								if (waittime == 1)
									Mix_PlayMusic(gameover, 0);
							}
							else {
								*state = 0;
								Mix_HaltMusic();
							}
							break;
			case 3: /* show password info, with fade in & out */
							if (round == 4)
								SDL_BlitSurface(passscreen01,NULL,window,NULL);
							if (round == 9)
								SDL_BlitSurface(passscreen02,NULL,window,NULL);
							if (round == 14)
								SDL_BlitSurface(passscreen03,NULL,window,NULL);
							if (round == 19)
								SDL_BlitSurface(passscreen04,NULL,window,NULL);
							if (round == 24)
								SDL_BlitSurface(passscreen05,NULL,window,NULL);
							if (round == 29)
								SDL_BlitSurface(passscreen06,NULL,window,NULL);
							if (round == 35)
								SDL_BlitSurface(passscreen07,NULL,window,NULL);
							if (round == 41)
								SDL_BlitSurface(passscreen08,NULL,window,NULL);
							if (round == 47)
								SDL_BlitSurface(passscreen09,NULL,window,NULL);
							if (round == 53)
								SDL_BlitSurface(passscreen10,NULL,window,NULL);
							SDL_SetSurfaceAlphaMod(blackbox,(Uint8)fadecounter);
							SDL_BlitSurface(blackbox,NULL,window,NULL);
							if ((fademode == 0) && (fadecounter == 255)) {
								SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
							}
							if ((fademode == 0) && (fadecounter > 0))
								fadecounter -= 3;
							if ((fademode == 1) && (fadecounter < 255))
								fadecounter += 3;
							if ((fademode == 0) && (fadecounter == 0))
								waittime ++;
							if (((fademode == 0) && (fadecounter == 0)) && (waittime == 600))
								fademode = 1;
							if (waittime > 599) {
								round ++;
								waittime = 0;
								step = 0;
								fademode = 0;
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
	SDL_FreeSurface(roundscreen);
	SDL_FreeSurface(blocks);
	SDL_FreeSurface(headgame);
	SDL_FreeSurface(fonts);
	SDL_FreeSurface(blackbox);
	SDL_FreeSurface(window);
	SDL_FreeSurface(gameoverscreen);
	SDL_FreeSurface(passscreen01);
	SDL_FreeSurface(passscreen02);
	SDL_FreeSurface(passscreen03);
	SDL_FreeSurface(passscreen04);
	SDL_FreeSurface(passscreen05);
	SDL_FreeSurface(passscreen06);
	SDL_FreeSurface(passscreen07);
	SDL_FreeSurface(passscreen08);
	SDL_FreeSurface(passscreen09);
	SDL_FreeSurface(passscreen10);
	Mix_FreeMusic(gameover);
	Mix_FreeChunk(stageclear);
	Mix_FreeChunk(giveup);
	Mix_FreeChunk(key);
	Mix_FreeChunk(kill);

}

static void show_tiles (struct hero *griel, int *animationtime, int map[][11][16], SDL_Surface *window, SDL_Surface *blocks, int round, int counter, Mix_Chunk *key) {

	SDL_Rect srcblocks = {0,0,16,16};
	SDL_Rect desblocks = {0,0,16,16};

	int i = 0;
	int j = 0;

	for (i=0;i<11;i++) {
		for (j=0;j<16;j++) {
			if (map[round][i][j] > 0) {
				if (((map[round][i][j] == 11) || (map[round][i][j] == 13) || (map[round][i][j] == 15)) && (counter > 29))
					map[round][i][j] ++;
				if (((map[round][i][j] == 12) || (map[round][i][j] == 14) || (map[round][i][j] == 16)) && (counter < 30))
					map[round][i][j] --;
				if ((map[round][i][j] == 17) && (counter > 29))
					map[round][i][j] += 2;
				if ((map[round][i][j] == 19) && (counter < 30))
					map[round][i][j] -= 2;
				if ((map[round][i][j] == 25) && (griel->key == 1)) { /* Got key, so open the door */
					Mix_PlayChannelTimed(-1,key,0,-1);
					griel->key = 0;
					map[round][i][j] = 26;
				}
				if (map[round][i][j] == 35) { /* Pow tile 2 */
					if (*animationtime < 15)
						*animationtime += 1;
					else {
						map[round][i][j] = 0;
						*animationtime = 0;
					}
				}
				if (map[round][i][j] == 36) { /* Pow tile 1 */
					if (*animationtime < 15)
						*animationtime += 1;
					else {
						map[round][i][j] = 35;
						*animationtime = 0;
					}
				}
				srcblocks.x = (map[round][i][j] - ((map[round][i][j] / 10) * 10) - 1) * 16;
				srcblocks.y = (map[round][i][j] / 10) * 16;
				if (map[round][i][j] == 10) {
					srcblocks.x = 144;
					srcblocks.y = 0;
				}
				desblocks.x = j * 16;
				desblocks.y = (i * 16) + 32;
				SDL_BlitSurface(blocks,&srcblocks,window,&desblocks);
			}
		}
	}

}

static void check_obstacles (struct hero *griel, int round, int map[][11][16], Mix_Chunk *kill, uint *grieltouch) {

	int deleteobject = 0;
	int target[2] = {0,0};

	if (griel->locked == 1) {
		/* Set block to analyze */
		switch (griel->direction) {
			case 1: target[0] = map[round][griel->positiony-1][griel->positionx];
							if (griel->positiony > 1)
								target[1] = map[round][griel->positiony-2][griel->positionx];
							else
								target[1] = 0;
							break;
			case 2: target[0] = map[round][griel->positiony+1][griel->positionx];
							if (griel->positiony < 9)
								target[1] = map[round][griel->positiony+2][griel->positionx];
							else
								target[1] = 0;
							break;
			case 3: target[0] = map[round][griel->positiony][griel->positionx-1];
							if (griel->positionx > 1)
								target[1] = map[round][griel->positiony][griel->positionx-2];
							else
								target[1] = 0;
							break;
			case 4: target[0] = map[round][griel->positiony][griel->positionx+1];
							if (griel->positionx < 14)
								target[1] = map[round][griel->positiony][griel->positionx+2];
							else
								target[1] = 0;
							break;
		}
		/* Check if its a wall */
		if (((target[0] > 0) && (target[0] < 4)) || ((target[0] > 7) && (target[0] < 11)) || ((target[0] > 34) && (target[0] < 37)) || ((target[0] > 40) && (target[0] < 46)) || (target[0] == 25) || (target[0] == 99)) {
			griel->locked = 0;
			/* griel->direction = 2; */
		}
		/* Check for "arrow" tile */
		if ((target[0] > 3) && (target[0] < 8)) {
			if ((griel->direction == 1) && (target[0] == 5)) {
				griel->locked = 0;
				/* griel->direction = 2; */
			}
			if ((griel->direction == 2) && (target[0] == 4)) {
				griel->locked = 0;
				/* griel->direction = 2; */
			}
			if ((griel->direction == 3) && (target[0] == 7)) {
				griel->locked = 0;
				/* griel->direction = 2; */
			}
			if ((griel->direction == 4) && (target[0] == 6)) {
				griel->locked = 0;
				/* griel->direction = 2; */
			}
			if ((griel->direction + 3 == (uint)target[0]) && ((target[1] > 20) && (target[1] < 24)) && (griel->object > 0)) {
			/* Block movement if a object is after the arrow and Griel has a object */
				griel->locked = 0;
				/* griel->direction = 2; */
			}
		}
		/* Check if its a enemy */
		if ((target[0] == 11) || (target[0] == 12)) { /* Slim */
			if (griel->object == 1) {
				Mix_PlayChannelTimed(-1,kill,0,-1);
				deleteobject = 1;
				griel->object = 0;
				griel->score += 10;
			}
			else {
				griel->locked = 0;
				/* griel->direction = 2; */
			}
		}
		if ((target[0] == 13) || (target[0] == 14)) { /* Ghost */
			if (griel->object == 2) {
				deleteobject = 1;
				Mix_PlayChannelTimed(-1,kill,0,-1);
				griel->object = 0;
				griel->score += 60;
			}
			else {
				griel->locked = 0;
				/* griel->direction = 2; */
			}
		}
		if ((target[0] == 15) || (target[0] == 16)) { /* Ogre */
			if (griel->object == 3) {
				deleteobject = 1;
				Mix_PlayChannelTimed(-1,kill,0,-1);
				griel->object = 0;
				griel->score += 30;
			}
			else {
				griel->locked = 0;
				/* griel->direction = 2; */
			}
		}
		/* Check if its a object */
		if ((target[0] > 20) && (target[0] < 24)) {
			if (griel->object == 0) {
				griel->object = target[0] - 20;
				/* Add points to score */
				griel->score += 50;
				deleteobject = 1;
			}
			else {
				griel->locked = 0;
				/* griel->direction = 2; */
			}
		}
		if (target[0] == 24) { /* Key */
				griel->key = 1;
				griel->score += 300; /* Add to score */
				deleteobject = 1;
		}
		if (target[0] == 46) { /* Grial */
				griel->grial = 1;
				griel->score += 1000; /* Add to score */
				deleteobject = 1;
				*grieltouch = 1;
		}
		/* Deleting objects */
		if (deleteobject == 1) {
			switch (griel->direction) {
				case 1: if (map[round][griel->positiony-1][griel->positionx] < 21) {
									map[round][griel->positiony-1][griel->positionx] = 36;
									griel->locked = 0;
									/* griel->direction = 2; */
								}
								else
									map[round][griel->positiony-1][griel->positionx] = 0;
								break;
				case 2: if (map[round][griel->positiony+1][griel->positionx] < 21) {
									map[round][griel->positiony+1][griel->positionx] = 36;
									griel->locked = 0;
									/* griel->direction = 2; */
								}
								else
									map[round][griel->positiony+1][griel->positionx] = 0;
								break;
				case 3: if (map[round][griel->positiony][griel->positionx-1] < 21) {
									map[round][griel->positiony][griel->positionx-1] = 36;
									griel->locked = 0;
									/* griel->direction = 2; */
								}
								else
									map[round][griel->positiony][griel->positionx-1] = 0;
								break;
				case 4: if (map[round][griel->positiony][griel->positionx+1] < 21) {
									map[round][griel->positiony][griel->positionx+1] = 36;
									griel->locked = 0;
									/* griel->direction = 2; */
								}
								else
									map[round][griel->positiony][griel->positionx+1] = 0;
								break;
			}
		}
	}

	/* Contine moving if you are in arrow */
	if (((map[round][griel->positiony][griel->positionx] > 3) && (map[round][griel->positiony][griel->positionx] < 8)) && (griel->movement == 0)) {
		griel->locked = 1;
		griel->direction = map[round][griel->positiony][griel->positionx] - 3;
	}

	/* Check for a open door */
	if ((map[round][griel->positiony][griel->positionx] == 26) && (griel->locked == 0)) {
		griel->direction = 5;
		griel->locked = 1;
	}

}

static void check_pushed_key (struct hero *griel, uint *fullscreench, SDL_Event *keystroke) {
	if ((keystroke->key.keysym.sym == KEY_START) || (keystroke->key.keysym.sym == KEY_START2)) {
		if ((griel != NULL) && (griel->locked == 0) && (griel->deathanimation == 0)) {
			griel->locked = 1;
			griel->direction = 6;
		}
	}
	if (keystroke->key.keysym.sym == SDLK_UP) {
		if ((griel != NULL) && (griel->locked == 0) && (griel->positiony > 0)) {
			griel->locked = 1;
			griel->direction = 1;
		}
	}
	if (keystroke->key.keysym.sym == SDLK_DOWN) {
		if ((griel != NULL) && (griel->locked == 0) && (griel->positiony < 10)) {
			griel->locked = 1;
			griel->direction = 2;
		}
	}
	if (keystroke->key.keysym.sym == SDLK_LEFT) {
		if ((griel != NULL) && (griel->locked == 0) && (griel->positionx > 0)) {
			griel->locked = 1;
			griel->direction = 3;
		}
	}
	if (keystroke->key.keysym.sym == SDLK_RIGHT) {
		if ((griel != NULL) && (griel->locked == 0) && (griel->positionx < 15)) {
			griel->locked = 1;
			griel->direction = 4;
		}
	}
#ifndef _FULLSCREEN_ONLY
	if ((keystroke->key.keysym.sym == SDLK_f) && (fullscreench != NULL))
		*fullscreench = 1;
#endif
	if (keystroke->key.keysym.sym == KEY_QUIT)
		exit(0);
}

static void controls (struct hero *griel, uint *fullscreench) {

	SDL_Event keystroke;

	/* handle key repeat */
	if (repeat_key_next_ticks) {
		const Uint32 now = SDL_GetTicks();
		if (SDL_TICKS_PASSED(now, repeat_key_next_ticks)) {
			repeat_key_next_ticks = now + 30;
			check_pushed_key (griel, fullscreench, &repeat_key_event);
		}
	}

	while (SDL_PollEvent(&keystroke)) {
		if (keystroke.type == SDL_QUIT)
			exit(0);
		if ((keystroke.type == SDL_KEYDOWN) && (!keystroke.key.repeat)) {
			repeat_key_event = keystroke;
			repeat_key_next_ticks = SDL_GetTicks() + 30;
			check_pushed_key (griel, fullscreench, &keystroke);
		} else if ((keystroke.type == SDL_KEYUP)) {
			if (keystroke.key.keysym.sym == repeat_key_event.key.keysym.sym) {
				repeat_key_event.key.keysym.sym = 0;
				repeat_key_next_ticks = 0;
			}
		}
	}

}
