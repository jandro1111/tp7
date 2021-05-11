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

//Inicializa ImGui
int gui_init(void);
//Desinstala Imgui
void gui_uninst(void);
//Permite el ingreso de usuario y cantidad de tweets a descargar
void gui_input(int& flag, int& canttwits, string& autor);
//Permite cancelar la descarga
void gui_searching(int& flag);
//Permite navegar por los tweets descargados
void gui_showtw(int& flag, float& vel);
//Permite descargar mas tweets o terminar con la ejecucion
int gui_ending(int& flag);

#endif