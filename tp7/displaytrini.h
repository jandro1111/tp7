#pragma once
#ifndef _DISPLAYTRINI_H
#define _DISPLAYTRINI_H

#include <iostream>
#include <string>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "basicLCD.h"

#define COLMAX 16
#define COLMIN 0
#define FILAMAX 2
#define FILAMIN 0

#define HEIGHT 200
#define WIDTH 800

#define FONT_TRINI "font.ttf"

#define BLACK al_map_rgb(0, 0, 0)
#define LIGHT_BLUE al_map_rgb(12, 183, 242)
#define WHITE al_map_rgb(255, 255, 255)

using namespace std;

typedef struct {
	//Display
	ALLEGRO_DISPLAY* display;
	//Cola de eventos
	ALLEGRO_EVENT_QUEUE* eventQueue;
	//font
	ALLEGRO_FONT* font;

}all_t;

class displayTrini :public basicLCD
{
public:
	//Constructor
	displayTrini();

	//Destructor
	~displayTrini();

	bool lcdInitOk();

	lcdError& lcdGetError();

	bool lcdClear();

	bool lcdClearToEOL();

	basicLCD& operator<< (unsigned const char c);

	basicLCD& operator<< (const char* c);

	bool lcdMoveCursorUp();

	bool lcdMoveCursorDown();

	bool lcdMoveCursorRight();

	bool lcdMoveCursorLeft();

	bool lcdSetCursorPosition(const cursorPosition pos);

	cursorPosition lcdGetCursorPosition();


private:
	cursorPosition cursor;

	char str[2][16];

	all_t all;

	bool initOk;

	lcdError error;

	bool allegroInit(void);

	void allegroDestroy(void);

	void draw(void);
};

#endif
