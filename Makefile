all: clock

funanim: clock.c 
	gcc clock.c gfx.o -lX11 -lm -o clock

clean:
	rm clock
