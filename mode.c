/* QMAP: Quake level viewer
 *
 *   mode.c    Copyright 1997 Sean Barett
 *
 *   General screen functions (set graphics
 *   mode, blit to framebuffer, set palette)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>

#include "s.h"
#include "mode.h"

static SDL_Surface *screen;

void blit(char *src)
{
   SDL_LockSurface(screen);
   memcpy(screen->pixels, src, 320*200);
   SDL_UnlockSurface(screen);
   SDL_Flip(screen);
}

void set_pal(uchar *pal)
{
   SDL_Color colors[256];
   int i;
   for (i=0; i < 256; ++i) {
      colors[i].r=pal[i*3];
      colors[i].g=pal[i*3+1];
      colors[i].b=pal[i*3+2];
   }
   SDL_SetColors(screen, colors, 0, 256);
}

bool graphics=0;
void set_lores(void)
{
   graphics = 1;
   SDL_SetVideoMode(320, 200, 8, SDL_SWSURFACE);
   screen = SDL_GetVideoSurface();
}
   
void set_text(void)
{
   if (graphics) {
      graphics = 0;
   }
}

void fatal_error(char *message, char *file, int line)
{
   set_text();
   printf("Error (%s line %d): %s\n", file, line, message);
   exit(1);
}
