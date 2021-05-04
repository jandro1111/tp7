#include <iostream>
#include <exception>
#include <list>
#include <curl/curl.h>
#include "json.hpp"
#include "mylcd.h"
#include "tweet.h"
#include "claselcd1.h"
#include "header.h"

//Vamos a usar la librería NLOHMANN JSON 

/*
* Main de prueba.
* Obtiene twits de una cuenta pública (en este caso, los últimos 10 twits de La Nación)
*
*/

using namespace std;

int main(void)
{
	init_allegro();
	// a reemplazar con imgui
	int canttwits = 3;
	string autor = "lanacion";
	//
	basicLCD* lcd;
	claselcd1 lcd1;
	mylcd lcd2;
	//displaytrini lcd3;	CREAR DISPLAY TRINI!!!!!!!
	lcd = &lcd1;
	cursorPosition cursor;
	list<twits>tweet;
	list <twits> :: iterator twit;
	twit = tweet.begin();
	dostuff(autor,canttwits,tweet);
	char* body = &(twit->author)[0];
	int pos = 0;

	int boton = DONO; //CAMBIAR POR VAR DE IMGUI!!!!!!!!!!!!!!!!!!!!!!

	while (boton != CANCEL /*&& descarga_no_terminada*/)
	{
		cursor = { 0,0 };
		lcd->lcdSetCursorPosition(cursor);
		for (int i = 0; i < 16; i++)
		{
			*lcd << ((body)[0]);
			lcd->lcdMoveCursorRight();
		}
		if ((string(body)).length() > 16)	//No toma el terminador
		{
			body++;
		}
		else
		{
			body = &(twit->author)[0];
		}
		cursor = { 1,0 };
		lcd->lcdSetCursorPosition(cursor);
		for (int i = 0; i < 16; i++)
		{
			if (i >= 0 && i > (pos-4) && i <= (pos))
			{
				*lcd << '=';
			}
			else
			{
				*lcd << ' ';
			}
			lcd->lcdMoveCursorRight();
			pos++;
		}
		if (pos == 18)
		{
			pos = 0;
		}
		//Se apreto un boton?
		al_rest(0.1);
	}

	body = &(twit->body)[0];

	while (boton != EXIT)
	{
		while (boton == DONO)
		{
			cursor = { 0,0 };
			lcd->lcdSetCursorPosition(cursor);
			*lcd << (twit->date).c_str();

			cursor = { 1,0 };
			lcd->lcdSetCursorPosition(cursor);
			for (int i = 0; i < 16; i++)
			{
				*lcd << ((body)[0]);
				lcd->lcdMoveCursorRight();
			}
			if ((string(body)).length() > 16)	//No toma el terminador
			{
				body++;
			}
			//Se apreto un boton?
			al_rest(0.1);
		}
		switch (boton)
		{
		case REP:
			body = &((twit->body)[0]);
			boton = DONO;
			break;
		case NEXT:	//Pasa al twit siguiente
			twit++;
			body = &((twit->body)[0]);
			boton = DONO;
			break;
		case PREV:	//Pasa al twit anterior
			twit--;
			body = &((twit->body)[0]);
			boton = DONO;
			break;
		case LCD1:	//Cambia el display en uso
			lcd = &lcd1;
			boton = DONO;
			break;
		case LCD2:
			lcd = &lcd1;	//CAMBIAR!!!!!!!!!!!!!!!!!
			boton = DONO;
			break;
		case LCD3:
			lcd = &lcd1;
			boton = DONO;
			break;
		default:
			break;
		}
	}

	
}