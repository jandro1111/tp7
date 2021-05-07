#ifndef CLASELCD1_H
#define CLASELCD1_H

#include "basicLCD.h"
#include <string>
#include <allegro5/display.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>

#define ALTODISPLAY 230
#define ANCHODISPLAY 850
#define SIZEFUENTE 100
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

    bool lcdCharIn(cursorPosition, const unsigned char);
    bool lcdDisplayUpdate();

private:
    ALLEGRO_DISPLAY* displaylcd1;
    ALLEGRO_FONT* fuente;
    cursorPosition cursor;
    lcdError estado;
    char text[32];
};

#endif // !CLASELCD1_H

