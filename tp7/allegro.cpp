#include "allegro.h"
#include<string>
#include<iostream>
using namespace std;
int init_allegro() {            // incio allegro
    if (!al_init()) {
        cout << "failed to initialize allegro!!" << endl;
        return -1;
    }
    else if (!al_init_image_addon()) {
        std::cout << "failed to init image addon!" << std::endl;
        return -1;
    }
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon

    return 0;
}

void destroy_allegro(void) {
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_image_addon();
}