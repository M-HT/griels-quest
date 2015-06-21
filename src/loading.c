/* loading.c */

# include <stdio.h>

# include "loading.h"
# include "comun.h"

void loaddata (int map[][11][16]) {

	uint i = 0;
	uint j = 0;
	uint k = 0;
	FILE *datafile = fopen(DATA_PATH "data/rounds.txt", "r");
	char line[49];
	char temp[3];
	temp[2] = 0;

	/* Load data an put in array */
	fgets (line, 49, datafile);
	for (i=0;i<58;i++) {
		for (j=0;j<11;j++) {
			for (k=0;k<16;k+=1) {
				temp[0] = line[k*3];
				temp[1] = line[k*3 + 1];
				sscanf (temp, "%d", &map[i][j][k]);
			}
			fgets (line, 49, datafile);
		}
		fgets (line, 49, datafile);
	}

	fclose(datafile);

}

Mix_Music *load_music(Mix_Music *oldbsogame, int round) {
	Mix_Music *bsogame;

	switch (round % 5)
	{
		default:
		case 0:
			bsogame = Mix_LoadMUS(DATA_PATH "music/stage1.ogg");
			break;
		case 1:
			bsogame = Mix_LoadMUS(DATA_PATH "music/stage2.ogg");
			break;
		case 2:
			bsogame = Mix_LoadMUS(DATA_PATH "music/stage3.ogg");
			break;
		case 3:
			bsogame = Mix_LoadMUS(DATA_PATH "music/stage4.ogg");
			break;
		case 4:
			bsogame = Mix_LoadMUS(DATA_PATH "music/stage5.ogg");
			break;
	}

	Mix_PlayMusic(bsogame, -1);

	if (oldbsogame != NULL) Mix_FreeMusic(oldbsogame);

	return bsogame;
}
