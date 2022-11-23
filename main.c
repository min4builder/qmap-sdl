/*  QMAP: Quake level viewer
 *
 *   main.c    Copyright 1997 Sean Barett
 *
 *   General setup control, main "sim" loop
 */

#include <SDL/SDL.h>

#include "bspfile.h"
#include "mode.h"
#include "3d.h"
#include "fix.h"
#include "scr.h"
#include "tm.h"
#include "render.h"
#include "bsp.h"
#include "surface.h"
#include "poly.h"

double chop_temp;

vector cam_loc, cam_vel, new_loc;
angvec cam_ang, cam_angvel;

char *scr_buf;
int   scr_row;

#define ANG_STEP  0x0008
#define VEL_STEP  0.5

char colormap[64][256];

void run_sim(void)
{
   vector temp;

   scr_buf = malloc(320*200);
   scr_row = 320;
   qmap_set_output(scr_buf, scr_row);

   cam_loc.x = 500;
   cam_loc.y = 240;
   cam_loc.z = 100;

   for (;;) {

      // RENDER

      set_view_info(&cam_loc, &cam_ang);
      render_world(&cam_loc);
      blit(scr_buf);

      // UI

      SDL_Event ev;
      while (SDL_PollEvent(&ev)) {
         if (ev.type == SDL_KEYDOWN) {
            switch(ev.key.keysym.sym) {
            case SDLK_DOWN: cam_angvel.tx += ANG_STEP; break;
            case SDLK_3: cam_angvel.ty += ANG_STEP; break;
            case SDLK_RIGHT: cam_angvel.tz += ANG_STEP; break;

            case SDLK_d: cam_vel.x += VEL_STEP; break;
            case SDLK_j: cam_vel.z -= VEL_STEP; break;
            case SDLK_w: cam_vel.y += VEL_STEP; break;

            case SDLK_UP: cam_angvel.tx -= ANG_STEP; break;
            case SDLK_1: cam_angvel.ty -= ANG_STEP; break;
            case SDLK_LEFT: cam_angvel.tz -= ANG_STEP; break;

            case SDLK_a: cam_vel.x -= VEL_STEP; break;
            case SDLK_k: cam_vel.z += VEL_STEP; break;
            case SDLK_x: cam_vel.y -= VEL_STEP; break;

            case SDLK_SPACE:
               cam_vel.x = cam_vel.y = cam_vel.z = 0;
               cam_angvel.tx = cam_angvel.ty = cam_angvel.tz = 0;
               break;
            default:
               break;
            }
         } else if (ev.type == SDL_QUIT)
            return;
      }

      // "PHYSICS"

      cam_ang.tx += cam_angvel.tx;
      cam_ang.ty += cam_angvel.ty;
      cam_ang.tz += cam_angvel.tz;
      set_view_info(&cam_loc, &cam_ang);

      temp.x = cam_vel.x; temp.y = 0; temp.z = 0;
      rotate_vec(&temp);
      cam_loc.x  += temp.x; cam_loc.y += temp.y; cam_loc.z += temp.z;

      temp.x = 0; temp.y = cam_vel.y; temp.z = 0;
      rotate_vec(&temp);
      cam_loc.x  += temp.x; cam_loc.y += temp.y; cam_loc.z += temp.z;

      temp.x = 0; temp.y = 0; temp.z = cam_vel.z;
      rotate_vec(&temp);
      cam_loc.x  += temp.x; cam_loc.y += temp.y; cam_loc.z += temp.z;
   }
}

void load_graphics(void)
{
   unsigned char pal[768];
   FILE *f;
   if ((f = fopen("palette.lmp", "rb")) == NULL)
      fatal("Couldn't read palette.lmp\n");
   fread(pal, 1, 768, f);
   fclose(f);
   set_pal(pal); 
   if ((f = fopen("colormap.lmp", "rb")) == NULL)
      fatal("Couldn't read colormap.lmp\n");
   fread(colormap, 256, 64, f);
   fclose(f);
}

int main(int argc, char **argv)
{
   if (argc != 2) {
      printf("Usage: qmap <bspfile>\n");
   } else {
      SDL_Init(SDL_INIT_VIDEO);
      LoadBSPFile(argv[1]);
      set_lores();
      load_graphics();
      init_cache();
      setup_default_point_list();

      run_sim();      
      set_text();
   }
   return 0;
}
