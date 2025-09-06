CC = gcc
CFLAGS = -I/opt/X11/include
LDFLAGS = -L/opt/X11/lib -lX11 -lm

OBJ = gfx3.o

all: clock

gfx3.o: gfx3.c
	$(CC) $(CFLAGS) -c gfx3.c -o gfx3.o

clock: clock.c $(OBJ)
	$(CC) $(CFLAGS) clock.c $(OBJ) $(LDFLAGS) -o clock

clean:
	rm -f clock gfx3.o
