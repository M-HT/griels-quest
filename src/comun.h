/* Definiciones comunes para facilitar portabilidad */

#ifndef _COMUN_H_
#define _COMUN_H_

#ifdef _GCW_ZERO
  #define _RUTAS_RELATIVAS
  #define _RENDER_320_240
  #define KEY_START SDLK_RETURN
  #define KEY_START2 SDLK_LCTRL
  #define KEY_QUIT SDLK_ESCAPE
#elif defined(PANDORA) || defined(PYRA)
  #define _RUTAS_RELATIVAS
  #define _FULLSCREEN_ONLY
  #define KEY_START SDLK_SPACE
  #define KEY_START2 SDLK_PAGEDOWN
  #define KEY_QUIT SDLK_ESCAPE
#else
  #define KEY_START SDLK_SPACE
  #define KEY_START2 SDLK_SPACE
  #define KEY_QUIT SDLK_ESCAPE
#endif

#ifdef _RUTAS_RELATIVAS
  #define DATA_PATH ""
#else
  #define DATA_PATH "/usr/share/griels/"
#endif

#endif /* _COMUN_H_ */
