#include "logic.h"


bool inits() //inicializacion de allegro y la gui
{
	if (!init_allegro()) //inicializacion de allegro
	{
		return true;
	}
	if (!gui_init()) //inicializacion de la gui
	{
		return true;
	}
	return false;
}

//llama a las funciones que bajan los tweets, dependiendo si se especifico la cantidad de tweets o no
bool curltweets(int& canttwits, string& autor, list<twits>& tweet, string& errorcurl, int& boton, basicLCD** lcd, bool* islcd, bool& prog_exit)
{
	bool error = false;
	if (canttwits <= 0)
	{
		error = dostuff(autor, tweet, errorcurl, boton, lcd, islcd, prog_exit);
		canttwits = 20;
	}
	else
	{
		error = dostuff(autor, canttwits, tweet, errorcurl, boton, lcd, islcd, prog_exit);
	}
	return error;
}

void curlerror(basicLCD* lcd, string& errorcurl) //muestra errores al descargar los tweets
{
	cursorPosition cursor;
	bool exit_screen = false;
		char* errorcurlstr = &errorcurl[0];
		while (exit_screen == false)
		{
			cursor = { 0,0 };
			lcd->lcdSetCursorPosition(cursor);
			*lcd << "Error, Exiting!";
			cursor = { 1,0 };
			lcd->lcdSetCursorPosition(cursor);
			for (int i = 0; (i < 16) && (((errorcurlstr)[i]) != '\0'); i++) //muestra el mensaje de error en el display
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
			al_rest(0.1); //pausa para que no se pasen tan rapido los tweets en el display
		}
		exit_screen = false;
}

void pantallacarga(int& pos, basicLCD* lcd, string autor, bool* islcd, bool& exit) //muestra la pantalla de carga en el display
{
	cursorPosition cursor;
	cursor = { 0,0 };
	lcd->lcdSetCursorPosition(cursor);
	*lcd << (autor.c_str()); //muestra el autor mietras se cargan los tweets
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
}

void showtweets(int& boton, basicLCD** lcd, list<twits>& tweet, list<twits>::iterator& twit, bool* islcd)  //muestra los tweets en el display
{
	cursorPosition cursor;
	static float vel = 0.5;
	char* body = &((twit->body)[0]);	//Carga el cuerpo del tweet
	while (boton == DONO)
	{
		for (int j = 0; j < 3; j++)
		{
			if (islcd[j] == true)
			{
				cursor = { 0,0 };
				lcd[j]->lcdSetCursorPosition(cursor);
				*lcd[j] << ((twit->date).c_str()); //muestra la fecha en el primer renglon del display

				cursor = { 1,0 };
				lcd[j]->lcdSetCursorPosition(cursor);
				for (int i = 0; (i < 16) && (((body)[i]) != '\0'); i++) //muestra el tweet en la segunda fila del display
				{
					*lcd[j] << ((body)[i]);
				}
			}
			if ((string(body)).length() > 16)	//No toma el terminador
			{
				body++;
			}
		}
		gui_showtw(boton, vel); //muestra la gui
		changedisplay(boton, islcd, lcd);
		al_rest(vel / 10); //espera el tiempo que se haya determinado en la gui para mostrar los tweets
	}
	switch (boton)
	{
	case REP:  //repite el tweet
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
			for (int i = 0; i < 3; i++)
			{
				if (islcd[i] == true)
				{
					lcd[i]->lcdClear();
					cursor = { 0,0 };
					lcd[i]->lcdSetCursorPosition(cursor);
					*lcd[i] << "Ultimo Tweet";	//Informa que se llego al ultimo tweet
				}
			}
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
	}
}

void changedisplay(int& boton, bool* islcd, basicLCD** lcd)
{
	switch (boton)
	{
	case LCD1: //cambia de lcd
		if (islcd[0] == true)
		{
			lcd[0]->lcdClear();
			islcd[0] = false;
		}
		else
		{
			islcd[0] = true;
		}
		boton = DONO;
		break;
	case LCD2:
		if (islcd[1] == true)
		{
			lcd[1]->lcdClear();
			islcd[1] = false;
		}
		else
		{
			islcd[1] = true;
		}
		boton = DONO;
		break;
	case LCD3:
		if (islcd[2] == true)
		{
			lcd[2]->lcdClear();
			islcd[2] = false;
		}
		else
		{
			islcd[2] = true;
		}
		boton = DONO;
		break;
	case EXIT:
		boton = EXIT;
		break;
	}
}