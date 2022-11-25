CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -O2 -fno-strict-aliasing
LDFLAGS =
LIBS = -lSDL

OBJS = main.o poly.o render.o 3d.o mode.o bsp.o \
       tmap3d.o clippoly.o tm_c.o surface.o readbsp.o
SRCS = main.c poly.c render.c 3d.c mode.c bsp.c \
       tmap3d.c clippoly.c tm_c.c surface.c readbsp.c

qmap: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

tags: $(SRCS)
	ctags $(SRCS)

indent:
	indent -orig -di0 -nbc -ut -i8 -nlp -ci8 -l80 -hnl $(SRCS)

clean:
	$(RM) qmap *.o

.c.o:
	$(CC) $(CFLAGS) -c $<

main.o: main.c bspfile.h mode.h 3d.h fix.h scr.h tm.h render.h bsp.h surface.h poly.h
clippoly.o: clippoly.c 3d.h
render.o: render.c bspfile.h 3d.h render.h bsp.h scr.h poly.h
3d.o: 3d.c 3d.h bspfile.h
mode.o: mode.c s.h mode.h
bsp.o: bsp.c bspfile.h 3d.h bsp.h render.h
readbsp.o: readbsp.c bspfile.h mode.h
tmap3d.o: tmap3d.c 3d.h bspfile.h tmap3d.h tm.h
poly.o: poly.c bspfile.h fix.h 3d.h tm.h tmap3d.h surface.h
tm_c.o: tm_c.c s.h fix.h tm.h
surface.o: surface.c s.h tm.h bspfile.h surface.h tmap3d.h
