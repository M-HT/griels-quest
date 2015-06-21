/* hero.h */

# include <SDL.h>
# include <SDL_mixer.h>

# include "array.h"

void set_hero_init (struct hero *griel, int round);
void extralife (struct hero *griel, uint *uplife);
void show_hero (struct hero *griel, int counter, SDL_Surface *window, SDL_Surface *blocks, int *round, uint *step, uint *waittime, uint *soundblock, Mix_Chunk *giveup);
