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

	bool islcd[3] = { true, false, false };
	string autor = "";	//Guarda el usuario a buscar
	string errorcurl = "NoError";	//Error que devuelve el curl
	claselcd1 lcd1;
	mylcd lcd2;
	displayTrini lcd3;
	basicLCD* lcd[3] = {&lcd1, &lcd2, &lcd3};	//Puntero al display principal
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
			prog_exit = curltweets(canttwits, autor, tweet, errorcurl, boton, lcd, islcd, prog_exit);	//Se descargan los tweets
			if (prog_exit == true && errorcurl != "NoError")	//Se muestran los errores devueltos
			{
				for (int i = 0; i < 3; i++)
				{
					if (islcd[i] == true)
					{
						curlerror(lcd[i], errorcurl);
					}
					else
					{
						lcd[i]->lcdClear();
					}
				}
				al_rest(2);
			}

			if ( (tweet.size() != 0) && (prog_exit == false))	//Si la lista no esta vacia
			{
				twit = tweet.begin();
				boton = DONO;
				for (int i = 0; i < 3; i++)
				{
					lcd[i]->lcdClear();
				}
				while (boton != EXIT && !prog_exit)
				{
					showtweets(boton, lcd, tweet, twit, islcd);	//Se muestran los twits y la gui para desplazarse
				}
				for (int i = 0; i < 3; i++)
				{
					lcd[i]->lcdClear();
				}
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