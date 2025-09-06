#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "gfx.h"

#define width 1000
#define height 1000
#define radius 450
#define pi 3.14159265358979323

void drawTicks();
void drawNumbers();
void drawSecondHand(int);
void drawMinuteHand(int);
void drawHourHand(int);
void displayTime(int);

int main() {
	gfx_open(width, height, "Clock");

	//Detects keyboard/mouse input
	char c;
	//Increment amount on the clock
	int increment = 0;
	//The amount the increment variable will be incremented by
	int incrementAmount = 1;

	while (1) {
		gfx_clear();
		gfx_flush();

		//The borders
		gfx_color(255, 255, 255);
		gfx_line(0, 0, width, 0);
		gfx_line(0, height, width, height);
		gfx_line(0, 0, 0, height);
		gfx_line(width, 0, width, height);

		//The frame of the clock
		gfx_color(255, 0, 0);
		gfx_circle(width/2, height/2, radius);

		gfx_color(255, 255, 255);
		drawTicks();
		drawNumbers();
		gfx_color(255, 0, 0);
		drawSecondHand(increment);
		gfx_color(0, 255, 255);
		drawMinuteHand(increment);
		gfx_color(255, 0, 255);
		drawHourHand(increment);
		gfx_color(255, 255, 255);
		displayTime(increment);
		gfx_text(width-300, 50, "Press the left or right arrow key to shift the");
		gfx_text(width-300, 65, "the time, and press keys 1 through 9 to increase");
		gfx_text(width-300, 80, "this amount 0 to reset the time.");

		if (gfx_event_waiting()) {
			c = gfx_wait();

			if (c == 81) {
				//Unadvances the time when the left arrow is pressed
				increment -= incrementAmount;
			} else if (c == 83) {
				//Advances the time when the right arrow is pressed
				increment += incrementAmount;
			} else if (c - '0' >= 0 && c - '0' <= 9) {
				incrementAmount = c - '0';

				if (c - '0' == 0) {
					increment = 0;
				}
			} else if (c == 'q') {
				break;
			}
		}

		//60 frames per second
		usleep(16666.67);
	}

	return 0;
}

//Draws the clock ticks using similar math from symbol.c. Same way the rest of the graphics are drawn on the clock
void drawTicks() {
	float s = 2*pi/60;

	for (int i = 0; i < 60; i++) {
		gfx_line((width/2)+round(cos(i*s)*(radius-40)), (height/2)+round(sin(i*s)*(radius-40)), (width/2)+round(cos(i*s)*(radius-30)), (height/2)+round(sin(i*s)*(radius-30)));
	}
}

//Draws the numbers on the clock
void drawNumbers() {
	float s = 2*pi/12;

	for (int i = 1; i < 13; i++) {
		char c[2];
		sprintf(c, "%d\0", i);

		gfx_text((width/2)+round(cos(i*s-(pi/2))*(radius-60)), (height/2)+round(sin(i*s-(pi/2))*(radius-60)), c); 
	}
}

void drawSecondHand(int increment) {
	//seconds since Jan 1st 1970
	int t = time(NULL)%60;
	float s = 2*pi/60;

	gfx_line(width/2, height/2, width/2+round(cos(t*s-(pi/2)+(increment%60*pi/60))*(radius-80)), height/2+round(sin(t*s-(pi/2)+(increment%60*pi/60))*(radius-80)));
}

void drawMinuteHand(int increment) {
	//minutes since Jan 1st 1970
	int t = time(NULL)%3600;
	float s = 2*pi/3600;

	gfx_line(width/2, height/2, width/2+round(cos(t*s-(pi/2)+(increment%3600*pi/3600))*(radius-120)), height/2+round(sin(t*s-(pi/2)+(increment%3600*pi/3600))*(radius-120)));

}

void drawHourHand(int increment) {
	//Since time(NULL) starts from January 1st 1970 in GMT, I had to subtract 4 hours to get it into Notre Dame Time (EST) which would be 60*60*4 seconds
	int t = time(NULL)%43200-14400;
	float s = 2*pi/43200;

	gfx_line(width/2, height/2, width/2+round(cos(t*s-(pi/2)+(increment%43200*pi/43200))*(radius-160)), height/2+round(sin(t*s-(pi/2)+(increment%43200*pi/43200))*(radius-160)));

}

void displayTime(int increment) {
	time_t localT;
	struct tm *info;
  time(&localT);
  info = localtime(&localT);
	int realHour;

	realHour = info->tm_hour;

	if (info->tm_hour == 0) {
		realHour = 12;
	} else if (info->tm_hour > 12) {
		realHour = info->tm_hour-12;
	}

	char c[9];
	sprintf(c, "%d:%d:%d %s\0", realHour, info->tm_min, time(NULL)%60, info->tm_hour < 13 ? "AM" : "PM");
	gfx_text(50, 50, c);
}
