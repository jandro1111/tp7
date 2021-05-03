#pragma once
#ifndef CLASELCD1_H
#define CLASELCD1_H

#include "basicLCD.h"
#include <allegro5/display.h>
#include <allegro5/allegro_font.h>

#define ALTODISPLAY 100
#define ANCHODISPLAY 800
#define SIZEFUENTE 50
#define BACKCOLOR (al_map_rgb(0, 0, 255))
#define FONTCOLOR (al_map_rgb(0, 0, 0))
#define POINTERCOLOR (al_map_rgb(255, 0, 0))
#define OFFSET 12

enum { NONE = 0, CURSOR, CHARACTER, DISPLAY };

class claselcd1 : public basicLCD
{
public:
    claselcd1();
    ~claselcd1();

    bool lcdInitOk();
    lcdError& lcdGetError();
    bool lcdClear();
    bool lcdClearToEOL();
    basicLCD& operator<<(const unsigned char c);
    basicLCD& operator<<(const char* c);
    bool lcdMoveCursorUp();
    bool lcdMoveCursorDown();
    bool lcdMoveCursorRight();
    bool lcdMoveCursorLeft();
    bool lcdSetCursorPosition(const cursorPosition pos);
    cursorPosition lcdGetCursorPosition();

    bool lcdCharIn(cursorPosition, char);
    bool lcdDisplayUpdate();

private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* fuente;
    cursorPosition cursor;
    lcdError estado;
    char text[32];
};

#endif // !CLASELCD1_H

