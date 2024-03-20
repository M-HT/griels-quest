/* main.h */

# include <SDL.h>

int control_frames (int i, int frate);
SDL_Surface *load_image_display_format (const char *filename);
SDL_Surface *load_image_display_format_alpha (const char *filename);
void change_fullscreen (void);
