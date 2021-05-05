#include "allegro.h"
#include<string>
#include<iostream>
using namespace std;
int init_allegro() {            // incio allegro
    if (!al_init()) {
        cout << "failed to initialize allegro!!" << endl;
        return false;
    }
    else if (!al_init_image_addon()) {
        std::cout << "failed to init image addon!" << std::endl;
        return false;
    }
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon

    return true;
}

void destroy_allegro(void) {
    al_shutdown_image_addon();
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
}