#ifndef LOGIC_H
#define LOGIC_H

#include "header.h"
#include "allegro.h"
#include "tweet.h"

//Inicializa allegro e ImGui
bool inits();
//Muestra la pantalla y gui mientras se cargan tweets
void pantallacarga(int& pos, basicLCD* lcd, string autor, bool* islcd, bool& exit);
//Descarga los tweets
bool curltweets(int& canttwits, string& autor, list<twits>& tweet, string& errorcurl, int& boton, basicLCD** lcd, bool* islcd, bool& prog_exit);
//Muestra errores al descargar los tweets
void curlerror(basicLCD* lcd, string& errorcurl);
//Muestra la pantalla y gui para leer los tweets
void showtweets(int& boton, basicLCD** lcd, list<twits>& tweet, list<twits>::iterator& twit, bool* islcd);
//Enciende o apaga pantallas
void changedisplay(int& boton, bool* islcd, basicLCD** lcd);
#endif // !LOGIC_H