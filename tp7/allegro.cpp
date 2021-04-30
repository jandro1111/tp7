#include "allegro.h"
#include<string>
#include<iostream>
using namespace std;
int init_allegro() {            // incio allegro
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT* font = NULL;
    ALLEGRO_FONT* fuentemicho = NULL;
    if (!al_init()) {
        cout << "failed to initialize allegro!!" << endl;
        return -1;
    }
    else if (!al_init_image_addon()) {
        std::cout << "failed to init image addon!" << std::endl;
        return -1;
    }
    else if (!(display = al_create_display(W_PIXELS, H_PIXELS))) {
        cout << "failed to create display!" << endl;
        return -1;
    }
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    font = al_load_ttf_font("fuentemicho.ttf", 36, 0); //HAY CREAR UN FONT PARA CADA TAMAÑO DE LETRA :frowning: 
    fuentemicho = al_load_ttf_font("7SDD.ttf", 36, 0); //HAY CREAR UN FONT PARA CADA TAMAÑO DE LETRA :frowning: 
    if (!font) {
        fprintf(stderr, "Could not load '7SDD.ttf'.\n");
        return -1;
    }
    
    if (!fuentemicho) {
        fprintf(stderr, "Could not load 'fuentemicho.ttf'.\n");
        return -1;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));

    return 0;
}

void destroy_allegro(void) {
    ALLEGRO_FONT* font = NULL;
    ALLEGRO_FONT* fuentemicho = NULL;
    ALLEGRO_DISPLAY* display = NULL;
    al_destroy_display(display);
    al_shutdown_image_addon();
    al_destroy_font(font);
    al_destroy_font(fuentemicho);
}

//aldrawtext