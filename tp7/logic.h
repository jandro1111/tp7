#ifndef LOGIC_H
#define LOGIC_H

#include "header.h"
#include "allegro.h"
#include "tweet.h"

//Inicializa allegro e ImGui
bool inits();
//Muestra la pantalla y gui mientras se cargan tweets
basicLCD* pantallacarga(int& pos, int& boton, basicLCD* lcd, list<twits>& tweet, list<twits>::iterator& twit, claselcd1* dlcd1, mylcd* dlcd2, displayTrini* dlcd3, bool& exit);
//Descarga los tweets
bool curltweets(int& canttwits, string& autor, list<twits>& tweet, basicLCD* lcd);
//Muestra la pantalla y gui para leer los tweets
basicLCD* showtweets(int& boton, basicLCD* lcd, list<twits>& tweet, list<twits>::iterator& twit, claselcd1* dlcd1, mylcd* dlcd2, displayTrini* dlcd3);

#endif // !LOGIC_H