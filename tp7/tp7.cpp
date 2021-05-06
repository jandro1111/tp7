#include "header.h"
#include "tweet.h"
//Vamos a usar la librería NLOHMANN JSON 

/*
* Main de prueba.
* Obtiene twits de una cuenta pública (en este caso, los últimos 10 twits de La Nación)
*
*/

using namespace std;

int main(void)
{
	int error = 0;
	error = init_allegro();
	if (!error)
	{
		return -1;
	}
	error = gui_init();
	if (!error)
	{
		return -1;
	}
	int canttwits = 0;
	int boton = DONO;
	string autor = "";
	string errorcurl = "NoError";
	while (autor == "")
	{
		gui_input(boton, canttwits, autor);
		if (autor == "")
		{
			boton = WRONG;
		}
	}
	basicLCD* lcd;
	claselcd1 lcd1;
	mylcd lcd2;
	displayTrini lcd3;
	lcd = &lcd1;
	cursorPosition cursor;
	list<twits>tweet;
	list <twits> :: iterator twit;
	if (canttwits <= 0)
	{
		error = dostuff(autor, tweet, errorcurl);
	}
	else
	{
		error = dostuff(autor, canttwits, tweet, errorcurl);
	}
	int exit_screen = false;
	errorcurl = "Este es un error muy muy muy largo asi ocupa mas de 16 caracteres de mierda";
	if (errorcurl != "NoError")
	{
		char* errorcurlstr = &errorcurl[0];
		while (exit_screen == false)
		{
			cursor = { 0,0 };
			lcd->lcdSetCursorPosition(cursor);
			*lcd << "Error, Exiting!";
			cursor = { 1,0 };
			lcd->lcdSetCursorPosition(cursor);
			for (int i = 0; (i < 16) && (((errorcurlstr)[i]) != '\0'); i++)
			{
				*lcd << ((errorcurlstr)[i]);
			}
			if ((string(errorcurlstr)).length() > 16)	//No toma el terminador
			{
				errorcurlstr++;
			}
			else
			{
				exit_screen = true;
			}
			al_rest(0.1);
		}
		al_rest(2);
		gui_uninst();
		destroy_allegro();
		return -1;
	}
	//LISTA NO VACIO!!!!!!!!!!!!!!!!!!!!!!!

	twit = tweet.begin();
	char* body;
	int pos = 0;
	float vel = 0;
	while (boton != CANCEL /*&& descarga_no_terminada*/)
	{
		cursor = { 0,0 };
		lcd->lcdSetCursorPosition(cursor);
		*lcd << (&(twit->author)[0]);
		cursor = { 1,0 };
		lcd->lcdSetCursorPosition(cursor);
		for (int i = 0; i < 16; i++)
		{
			if (i >= 0 && i > (pos - 4) && i <= (pos))
			{
				*lcd << '=';
			}
			else
			{
				*lcd << ' ';
			}
		}
		if (pos == 19)
		{
			pos = -1;
		}
		pos++;
		gui_searching(boton, tweet.size());
		switch (boton)
		{
		case LCD1:
			lcd->lcdClear();
			lcd = &lcd1;
			boton = DONO;
			break;
		case LCD2:
			lcd->lcdClear();
			lcd = &lcd2;
			boton = DONO;
			break;
		case LCD3:
			lcd->lcdClear();
			lcd = &lcd3;
			boton = DONO;
			break;
		}
	}
	boton = DONO;
	body = &((twit->body)[0]);

	while (boton != EXIT)
	{
		while (boton == DONO)
		{
			cursor = { 0,0 };
			lcd->lcdSetCursorPosition(cursor);
			*lcd << ((twit->date).c_str());

			cursor = { 1,0 };
			lcd->lcdSetCursorPosition(cursor);
			for (int i = 0; (i < 16) && (((body)[i]) != '\0'); i++)
			{
				*lcd << ((body)[i]);
			}
			if ((string(body)).length() > 16)	//No toma el terminador
			{
				body++;
			}
			gui_showtw(boton, vel);
			al_rest(vel);
		}
		switch (boton)
		{
		case REP:
			body = &((twit->body)[0]);
			boton = DONO;
			break;
		case NEXT:	//Pasa al twit siguiente
			if (twit != (--(tweet.end())))
			{
				twit++;
			}
			else
			{
				lcd->lcdClear();
				cursor = { 0,0 };
				lcd->lcdSetCursorPosition(cursor);
				*lcd << "Ultimo Tweet";
				al_rest(1);
			}
			body = &((twit->body)[0]);
			boton = DONO;
			break;
		case PREV:	//Pasa al twit anterior
			if (twit != tweet.begin())
			{
				twit--;
			}
			body = &((twit->body)[0]);
			boton = DONO;
			break;
		case LCD1:	//Cambia el display en uso
			lcd->lcdClear();
			lcd = &lcd1;
			boton = DONO;
			break;
		case LCD2:
			lcd->lcdClear();
			lcd = &lcd2;
			boton = DONO;
			break;
		case LCD3:
			lcd->lcdClear();
			lcd = &lcd3;
			boton = DONO;
			break;
		default:
			break;
		}
	}
	lcd->lcdClear();
	boton = DONO;
	while (boton != EXIT)
	{
		gui_ending(boton, canttwits);
	}
	gui_uninst();
	destroy_allegro();
	return 0;
}

//flujo del main
//velocidad scroll bloqueante

//Limpiar main y comentar