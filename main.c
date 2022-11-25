/*
 * General setup control, main "sim" loop 
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
int scr_row;

#define ANG_STEP  0x0010
#define VEL_STEP  0.25

char colormap[64][256];

void
run_sim(void)
{
	static char pressed[SDLK_LAST] = { 0 };
	vector temp;
	int now, before, delta;

	scr_buf = malloc(WIDTH * HEIGHT);
	scr_row = WIDTH;
	qmap_set_output(scr_buf, scr_row);

	cam_loc.x = 500;
	cam_loc.y = 240;
	cam_loc.z = 100;

	now = SDL_GetTicks();
	for (;;) {
		before = now;
		now = SDL_GetTicks();
		delta = now - before;

		// RENDER

		set_view_info(&cam_loc, &cam_ang);
		render_world(&cam_loc);
		blit(scr_buf);

		// UI

		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_KEYDOWN)
				pressed[ev.key.keysym.sym] = 1;
			else if (ev.type == SDL_KEYUP)
				pressed[ev.key.keysym.sym] = 0;
			else if (ev.type == SDL_QUIT)
				return;
		}

		cam_vel.x = cam_vel.y = cam_vel.z = 0;
		cam_angvel.tx = cam_angvel.ty = cam_angvel.tz = 0;

		if (pressed[SDLK_DOWN])
			cam_angvel.tx += ANG_STEP * delta;
		if (pressed[SDLK_UP])
			cam_angvel.tx -= ANG_STEP * delta;
		if (pressed[SDLK_RIGHT])
			cam_angvel.tz += ANG_STEP * delta;
		if (pressed[SDLK_LEFT])
			cam_angvel.tz -= ANG_STEP * delta;
		if (pressed[SDLK_w])
			cam_vel.y += VEL_STEP * delta;
		if (pressed[SDLK_s])
			cam_vel.y -= VEL_STEP * delta;
		if (pressed[SDLK_d])
			cam_vel.x += VEL_STEP * delta;
		if (pressed[SDLK_a])
			cam_vel.x -= VEL_STEP * delta;
		if (pressed[SDLK_k])
			cam_vel.z += VEL_STEP * delta;
		if (pressed[SDLK_j])
			cam_vel.z -= VEL_STEP * delta;

		// "PHYSICS"

		cam_ang.tx += cam_angvel.tx;
		cam_ang.ty += cam_angvel.ty;
		cam_ang.tz += cam_angvel.tz;
		set_view_info(&cam_loc, &cam_ang);

		temp.x = cam_vel.x;
		temp.y = 0;
		temp.z = 0;
		rotate_vec(&temp);
		cam_loc.x += temp.x;
		cam_loc.y += temp.y;
		cam_loc.z += temp.z;

		temp.x = 0;
		temp.y = cam_vel.y;
		temp.z = 0;
		rotate_vec(&temp);
		cam_loc.x += temp.x;
		cam_loc.y += temp.y;
		cam_loc.z += temp.z;

		temp.x = 0;
		temp.y = 0;
		temp.z = cam_vel.z;
		rotate_vec(&temp);
		cam_loc.x += temp.x;
		cam_loc.y += temp.y;
		cam_loc.z += temp.z;

		if (delta)
			printf("%dms, %d fps\n", delta, 1000 / delta);
	}
}

void
load_graphics(void)
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

int
main(int argc, char **argv)
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
