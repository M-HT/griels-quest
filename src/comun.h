/* Definiciones comunes para facilitar portabilidad */

#ifndef _COMUN_H_
#define _COMUN_H_

#ifdef _GCW_ZERO
  #define _RUTAS_RELATIVAS
  #define _RENDER_320_240
#endif
 
#ifdef _RUTAS_RELATIVAS
  #define DATA_PATH ""
#else
  #define DATA_PATH "/usr/share/griels/"
#endif

#endif /* _COMUN_H_ */
