#ifndef GUI_H
#define GUI_H

//#include "allegro.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "imgui.h"
#include "imgui_impl_allegro5.h"
#include "imgui_internal.h"
#include "imconfig.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"
#include "header.h"

int gui_init(void);

void gui_uninst(void);

void gui_input(int& flag, int& canttwits, string& autor);

void gui_searching(int& flag, int downtwits);

void gui_showtw(int& flag, float& vel);

void gui_ending(int& flag, int& cant);

#endif