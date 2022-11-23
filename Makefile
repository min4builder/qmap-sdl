CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -Og
LDFLAGS =
LIBS =

OBJS = main.o poly.o render.o 3d.o mode.o bsp.o \
       tmap3d.o clippoly.o tm_c.o surface.o readbsp.o

qmap: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<

main.o: main.c
clippoly.o: clippoly.c
render.o: render.c
3d.o: 3d.c
mode.o: mode.c
bsp.o: bsp.c
readbsp.o: readbsp.c
tmap3d.o: tmap3d.c
poly.o: poly.c
tm_c.o: tm_c.c
surface.o: surface.c
