#pragma once
#ifndef GUI_H
#define GUI_H

#include "allegro.h"
#include "imgui.h"
#include "imgui_impl_allegro5.h"
#include "imgui_internal.h"
#include "imconfig.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"
#include "header.h"

int gui_init(int flag);

int gui_searching(int flag);

int gui_showtw (int flag);

#endif