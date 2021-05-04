#include "header.h"
#include "tweet.h"
//Vamos a usar la librer�a NLOHMANN JSON 

/*
* Main de prueba.
* Obtiene twits de una cuenta p�blica (en este caso, los �ltimos 10 twits de La Naci�n)
*
*/

using namespace std;

int main(void)
{
	init_allegro();
	int canttwits = 3;
	int boton = DONO;
	string autor = "lanacion";
	gui_input(boton, canttwits, autor);
	basicLCD* lcd;
	claselcd1 lcd1;
	mylcd lcd2;
	displayTrini lcd3;
	lcd = &lcd1;
	cursorPosition cursor;
	list<twits>tweet;
	list <twits> :: iterator twit;
	int h = dostuff(autor,canttwits,tweet);
	twit = tweet.begin();
	char* body = &((twit->author)[0]);
	int pos = 0;

	//while (boton != CANCEL /*&& descarga_no_terminada*/)
	//{
	//	cursor = { 0,0 };
	//	lcd->lcdSetCursorPosition(cursor);
	//	for (int i = 0; i < 16; i++)
	//	{
	//		*lcd << ((body)[i]);
	//	}
	//	cursor = { 1,0 };
	//	lcd->lcdSetCursorPosition(cursor);
	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (i >= 0 && i > (pos - 4) && i <= (pos))
	//		{
	//			*lcd << '=';
	//		}
	//		else
	//		{
	//			*lcd << ' ';
	//		}
	//	}
	//	if (pos == 19)
	//	{
	//		pos = -1;
	//	}
	//	pos++;
	//	//Se apreto un boton?
	//	al_rest(0.1);
	//}

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
			for (int i = 0; i < 16; i++)
			{
				*lcd << ((body)[i]);
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
			lcd = &lcd2;
			boton = DONO;
			break;
		case LCD3:
			lcd = &lcd3;
			boton = DONO;
			break;
		default:
			break;
		}
	}
	gui_uninst();
	return 0;
}