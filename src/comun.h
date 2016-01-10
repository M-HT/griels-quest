/* Definiciones comunes para facilitar portabilidad */

#ifndef _COMUN_H_
#define _COMUN_H_

#ifdef _GCW_ZERO
  #define _RUTAS_RELATIVAS
  #define _RENDER_320_240
  #define KEY_START SDLK_RETURN
  #define KEY_START2 SDLK_LCTRL
  #define KEY_QUIT SDLK_ESCAPE
#elif defined(PANDORA)
  #define _RUTAS_RELATIVAS
  #define _FULLSCREEN_ONLY
  #define _DISPLAY_16BITS
  #define KEY_START SDLK_SPACE
  #define KEY_START2 SDLK_PAGEDOWN
  #define KEY_QUIT SDLK_ESCAPE
#elif defined(GP2X)
  #define _RUTAS_RELATIVAS
  #define _RENDER_320_240
  #define _FULLSCREEN_ONLY
  #define _DISPLAY_16BITS
  #define KEY_START SDLK_SPACE
  #define KEY_START2 SDLK_SPACE
  #define KEY_QUIT SDLK_ESCAPE
  // GP2X buttons
  #define GP2X_BUTTON_UP              (0)
  #define GP2X_BUTTON_DOWN            (4)
  #define GP2X_BUTTON_LEFT            (2)
  #define GP2X_BUTTON_RIGHT           (6)
  #define GP2X_BUTTON_UPLEFT          (1)
  #define GP2X_BUTTON_UPRIGHT         (7)
  #define GP2X_BUTTON_DOWNLEFT        (3)
  #define GP2X_BUTTON_DOWNRIGHT       (5)
  #define GP2X_BUTTON_CLICK           (18)
  #define GP2X_BUTTON_A               (12)
  #define GP2X_BUTTON_B               (13)
  #define GP2X_BUTTON_X               (14)
  #define GP2X_BUTTON_Y               (15)
  #define GP2X_BUTTON_L               (10)
  #define GP2X_BUTTON_R               (11)
  #define GP2X_BUTTON_START           (8)
  #define GP2X_BUTTON_SELECT          (9)
  #define GP2X_BUTTON_VOLUP           (16)
  #define GP2X_BUTTON_VOLDOWN         (17)

  #define BUTTON_START GP2X_BUTTON_START
  #define BUTTON_START2 GP2X_BUTTON_X
  #define BUTTON_QUIT GP2X_BUTTON_SELECT
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
