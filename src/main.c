/* Griel's Quest for the Sangraal */
/* Started on 17/04/2013 */
/* Under GPL v3 license */

# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>
# include <SDL_getenv.h>
# include <SDL_image.h>
# include <SDL_mixer.h>
#if defined(GP2X)
  # include <sys/ioctl.h>
  # include <unistd.h>
  # include <sys/soundcard.h>
  # include <fcntl.h>
  #if (SDL_MAJOR_VERSION > 1 || SDL_MAJOR_VERSION == 1 && (SDL_MINOR_VERSION > 2 || SDL_MINOR_VERSION == 2 && SDL_PATCHLEVEL >= 9 ) )
    # include <SDL_gp2x.h>
  #endif
#endif

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
#ifdef _DISPLAY_16BITS
	#define SDL_DISPLAY_BITS 16
#else
	#define SDL_DISPLAY_BITS 32
#endif
#ifdef _RENDER_320_240
	screen = SDL_SetVideoMode(320,240,SDL_DISPLAY_BITS,SDL_FLAGS);
#else
	screen = SDL_SetVideoMode(512,448,SDL_DISPLAY_BITS,SDL_FLAGS);
#endif

	/* Disable cursor */
		SDL_ShowCursor(SDL_DISABLE);

	/* Name of the window */
		SDL_WM_SetCaption("Griel's Quest for the Sangraal", "Griel's Quest");

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

#if defined(GP2X)
static int InitialVolume;

// Set new GP2X mixer level, 0-100
static void Set_GP2X_Volume (int newvol) {
	int soundDev, vol;

	if ((newvol >= 0) && (newvol <= 100)) {
		soundDev = open("/dev/mixer", O_RDWR);
		if (soundDev != -1) {
			vol = ((newvol << 8) | newvol);
			ioctl(soundDev, SOUND_MIXER_WRITE_PCM, &vol);
			close(soundDev);
		}
	}
}

// Returns 0-100, current mixer volume, -1 on error.
static int Get_GP2X_Volume (void) {
	int soundDev, vol;

	vol = -1;
	soundDev = open("/dev/mixer", O_RDONLY);
	if (soundDev != -1) {
		ioctl(soundDev, SOUND_MIXER_READ_PCM, &vol);
		close(soundDev);
		if (vol != -1) {
			//just return one channel , not both channels, they're hopefully the same anyways
			return (vol & 0xFF);
		}
	}

	return vol;
}

static void Set_Initial_GP2X_Volume (void) {
	Set_GP2X_Volume(InitialVolume);
}

#endif

static void initsdl(void) {
#if defined(GP2X)
	InitialVolume = Get_GP2X_Volume();
	atexit(Set_Initial_GP2X_Volume);
#endif

	/* Center the window */
	putenv("SDL_VIDEO_CENTERED=1");

	/* Init SDLVideo */
		if (SDL_Init(SDL_INIT_VIDEO)<0) {
			printf("Error starting SDL: %s\n",SDL_GetError());
			exit(1);
		}
		atexit(SDL_Quit);
#if defined(GP2X)
		if (SDL_InitSubSystem(SDL_INIT_JOYSTICK)<0) {
			printf("Error starting SDL joystick subsystem: %s\n",SDL_GetError());
			exit(1);
		}
		SDL_JoystickOpen(0);
#if defined(SDL_GP2X__H)
		if (SDL_GP2X_MouseType() == GP2X_MOUSE_TOUCHSCREEN) {
			SDL_GP2X_TouchpadMouseMotionEvents(0);
			SDL_GP2X_TouchpadMouseButtonEvents(0);
		}
#endif
#endif

#if defined(GP2X)
#define MIX_BUFFER_SIZE 512
#else
#define MIX_BUFFER_SIZE 4096
#endif
	  if (Mix_OpenAudio (44100, AUDIO_S16, 1, MIX_BUFFER_SIZE)) {
        printf("No se pudo inicializar SDL_Mixer %s\n",Mix_GetError());
        exit(1);
		}
		Mix_AllocateChannels(3);
		atexit(Mix_CloseAudio);

	/* Disable repeat keys */
	  SDL_EnableKeyRepeat(0, 0);

#if defined(GP2X)
	Set_Initial_GP2X_Volume();
#endif
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
#if defined(GP2X)
	SDL_Surface *screen = SDL_GetVideoSurface();
	image = SDL_ConvertSurface(temp, screen->format, SDL_SWSURFACE);
#else
	image = SDL_DisplayFormat(temp);
#endif
	SDL_FreeSurface(temp);

	return image;
}

SDL_Surface *load_image_display_format_alpha (const char *filename) {
	SDL_Surface *temp, *image;

	temp = IMG_Load(filename);
#if defined(GP2X)
	SDL_PixelFormat format;
	memset(&format, 0, sizeof(format));
	format.BitsPerPixel = 32;
	format.BytesPerPixel = 4;
	//format.Rloss = 0;
	//format.Gloss = 0;
	//format.Bloss = 0;
	//format.Aloss = 0;
	format.Rshift = 16;
	format.Gshift = 8;
	//format.Bshift = 0;
	format.Ashift = 24;
	format.Rmask = 0x00ff0000;
	format.Gmask = 0x0000ff00;
	format.Bmask = 0x000000ff;
	format.Amask = 0xff000000;
    format.alpha = SDL_ALPHA_OPAQUE;
	image = SDL_ConvertSurface(temp, &format, SDL_SWSURFACE);
#else
	image = SDL_DisplayFormatAlpha(temp);
#endif
	SDL_FreeSurface(temp);

	return image;
}

#if defined(GP2X)
void Change_HW_Audio_Volume (int amount) {
	int current_volume;

	current_volume = Get_GP2X_Volume();

	if (current_volume == -1) current_volume = 68;

	if ((amount > 1) && current_volume < 12) {
		amount = 1;
	} else if ((amount < -1) && current_volume <= 12) {
		amount = -1;
	}

	current_volume += amount;

	if (current_volume > 100) {
		current_volume = 100;
	} else if (current_volume < 0) {
		current_volume = 0;
	}
	Set_GP2X_Volume(current_volume);
}
#endif
