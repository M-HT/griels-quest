/* zoom.c */

# include <stdint.h>

# include "zoom.h"
# include "comun.h"

static void normal2x_16(const uint16_t *src, uint16_t *dst, unsigned int width, unsigned int srcstride, unsigned int dststride, unsigned int height);
static void normal2x_32(const uint32_t *src, uint32_t *dst, unsigned int width, unsigned int srcstride, unsigned int dststride, unsigned int height);

void BlitDoubleSurface (SDL_Surface *src, SDL_Surface *dst) {
    SDL_Surface *doble;

    if ((src->format->BitsPerPixel == 16) || (src->format->BitsPerPixel == 32)) {
        int allocated_surface;

        if ((2 * src->w <= dst->w) &&
            (2 * src->h <= dst->h) &&
            (src->format->BitsPerPixel == dst->format->BitsPerPixel) &&
            (src->format->Rmask == dst->format->Rmask) &&
            (src->format->Gmask == dst->format->Gmask) &&
            (src->format->Bmask == dst->format->Bmask) &&
            (src->format->Amask == dst->format->Amask)
        ) {
            // direct copy
            doble = dst;
            allocated_surface = 0;
        } else {
            allocated_surface = 1;
            doble = SDL_CreateRGBSurface(SDL_SWSURFACE, 2 * src->w, 2 * src->h, src->format->BitsPerPixel,
                                         src->format->Rmask, src->format->Gmask,
                                         src->format->Bmask, src->format->Amask);
            if (doble == NULL) return;
        }

        if (SDL_MUSTLOCK(src)) SDL_LockSurface(src);
        if (SDL_MUSTLOCK(doble)) SDL_LockSurface(doble);

        if (src->format->BitsPerPixel == 16) {
            normal2x_16((uint16_t *)src->pixels, (uint16_t *)doble->pixels, src->w, src->pitch, doble->pitch, src->h);
        } else {
            normal2x_32((uint32_t *)src->pixels, (uint32_t *)doble->pixels, src->w, src->pitch, doble->pitch, src->h);
        }

        if (SDL_MUSTLOCK(doble)) SDL_UnlockSurface(doble);
        if (SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);

        if (allocated_surface) {
            SDL_BlitSurface(doble,NULL,dst,NULL);
            SDL_FreeSurface(doble);
        }
    } else {
        SDL_Rect dstrect;
        dstrect.x = 0;
        dstrect.y = 0;
        dstrect.w = src->w * 2;
        dstrect.h = src->h * 2;
        SDL_BlitScaled(src,NULL,dst,&dstrect);
    }
}

static void normal2x_16(const uint16_t *src, uint16_t *dst, unsigned int width, unsigned int srcstride, unsigned int dststride, unsigned int height)
{
#define DST(n) ((uint32_t *)dst)[n]
#define DST2(n) ((uint32_t *)(((uintptr_t)dst) + dststride))[n]

    for (; height != 0; height--) {
        unsigned int w;

        for (w = width; w != 0; w--) {
            uint32_t src2;
            src2 = *src;
            DST(0) = DST2(0) = src2 | (src2 << 16);
            src++;
            dst+=2;
        }

        src = (const uint16_t *) (((uintptr_t)src) + srcstride - 2 * width);
        dst = (uint16_t *) (((uintptr_t)dst) + 2 * dststride - 4 * width);
    }

#undef DST2
#undef DST
}

static void normal2x_32(const uint32_t *src, uint32_t *dst, unsigned int width, unsigned int srcstride, unsigned int dststride, unsigned int height)
{
#define DST2(n) ((uint32_t *)(((uintptr_t)dst) + dststride))[n]

    for (; height != 0; height--) {
        unsigned int w;

        for (w = width; w != 0; w--) {
            dst[0] = dst[1] = DST2(0) = DST2(1) = *src;
            src++;
            dst+=2;
        }

        src = (const uint32_t *) (((uintptr_t)src) + srcstride - 4 * width);
        dst = (uint32_t *) (((uintptr_t)dst) + 2 * dststride - 8 * width);
    }

#undef DST2
}
