#include "header.h"
#include "tweet.h"
#include "logic.h"
//Vamos a usar la librería NLOHMANN JSON 

/*
* Main de prueba.
* Obtiene twits de una cuenta pública (en este caso, los últimos 10 twits de La Nación)
*
*/

using namespace std;

int main(void)
{
	bool prog_exit = false;	//Para terminar el programa
	bool once = false;
	int canttwits = 0;	//Cantidad de twits a descargar
	int boton = DONO;	//Flags de botones apretados en la gui

	prog_exit = inits();	//Inicializa allegro y la gui

	string autor = "";	//Guarda el usuario a buscar
	basicLCD* lcd;	//Puntero al display principal
	claselcd1 lcd1;
	mylcd lcd2;
	displayTrini lcd3;
	lcd = &lcd1;
	list<twits>tweet;	//Lista donde se almacenan los twits descargados
	list <twits> :: iterator twit;	//Creo el iterador

	while (prog_exit == false)
	{
		if (once == false)	//La primera vez llama a la gui para ingresar el usuario
		{
			while (autor == "")	//Hasta que se ingresa algun caracter
			{
				gui_input(boton, canttwits, autor);
				if (autor == "")	//Si no se ingreso nada muestra un mensaje de aviso
				{
					boton = WRONG;
				}
			}
			once = true;
		}
		else	//Se pregunta si se quiere continuar
		{
			int canttemp;
			while (boton == DONO)
			{
				canttemp = gui_ending(boton);
			}
			canttwits += canttemp;
		}

		if (boton != EXIT && prog_exit == false)
		{
			tweet.clear();
			prog_exit = curltweets(canttwits, autor, tweet, lcd);	//Se descargan los tweets
			if ( (tweet.size() != 0) && (prog_exit == false))	//Si la lista no esta vacia
			{
				twit = tweet.begin();
				int pos = 0;
				while (boton != CANCEL && boton != EXIT)
				{
					lcd = pantallacarga(pos, boton, lcd, tweet, twit, &lcd1, &lcd2, &lcd3, prog_exit);	//Muestra la pantalla de carga mientras se descargan
				}
				boton = DONO;

				while (boton != EXIT && !prog_exit)
				{
					lcd = showtweets(boton,lcd,tweet,twit,&lcd1,&lcd2,&lcd3);	//Se muestran los twits y la gui para desplazarse
				}
				lcd->lcdClear();
				boton = DONO;
			}
			else
			{
				prog_exit = true;
			}
		}
		else
		{
			prog_exit = true;
		}
	}
	gui_uninst();	//Se desinstala allegro y gui
	destroy_allegro();
	return 0;
}