/* main.h */

# include <SDL.h>

int control_frames (int i, int frate);
SDL_Surface *load_image_display_format (const char *filename);
SDL_Surface *load_image_display_format_alpha (const char *filename);
#if defined(GP2X)
void Change_HW_Audio_Volume (int amount);
#endif
