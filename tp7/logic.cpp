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

bool curltweets(int& canttwits, string& autor, list<twits>& tweet, basicLCD* lcd) //descarga los tweets
{
	int error = 0;
	string errorcurl = "NoError";
	cursorPosition cursor;
	if (canttwits <= 0)
	{
		error = dostuff(autor, tweet, errorcurl);
		canttwits = 20;
	}
	else
	{
		error = dostuff(autor, canttwits, tweet, errorcurl);
	}
	bool exit_screen = false;
	if (errorcurl != "NoError") //si hubo un error al descargar los tweets, se muestra en el display un mensaje de error
	{
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
		al_rest(2);
		return true;
	}
	if (error != 0)
	{
		return true;
	}
	return false;
}

basicLCD* pantallacarga(int& pos, int& boton, basicLCD* lcd, list<twits>& tweet, list<twits>::iterator& twit, claselcd1* dlcd1, mylcd* dlcd2, displayTrini* dlcd3, bool& exit) //muestra la pantalla de carga en el display
{
	cursorPosition cursor;
	cursor = { 0,0 };
	lcd->lcdSetCursorPosition(cursor);
	*lcd << (&(twit->author)[0]); //muestra el autor mietras se cargan los tweets
	cursor = { 1,0 };
	lcd->lcdSetCursorPosition(cursor);
	static long count = 0;
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
	gui_searching(boton, tweet.size()); //se llama a la gui para poder cambiar de display mientras se cargan los tweets
	al_rest(0.05);
	count++;
	switch (boton)
	{
	case LCD1: //cambia de lcd
		lcd->lcdClear();
		lcd = dlcd1;
		boton = DONO;
		break;
	case LCD2:
		lcd->lcdClear();
		lcd = dlcd2;
		boton = DONO;
		break;
	case LCD3:
		lcd->lcdClear();
		lcd = dlcd3;
		boton = DONO;
		break;
	case EXIT:
		exit = true;
		break;
	}
	if (count == 40)
	{
		boton = CANCEL;
		count = 0;
	}
	return lcd;
}

basicLCD* showtweets(int& boton, basicLCD* lcd, list<twits>& tweet, list<twits>::iterator& twit, claselcd1* dlcd1, mylcd* dlcd2, displayTrini* dlcd3)  //muestra los tweets en el display
{
	cursorPosition cursor;
	static float vel = 0;
	char* body;
	body = &((twit->body)[0]);	//Carga el cuerpo del tweet
	while (boton == DONO)
	{
		cursor = { 0,0 };
		lcd->lcdSetCursorPosition(cursor);
		*lcd << ((twit->date).c_str()); //muestra la fecha en el primer renglon del display

		cursor = { 1,0 };
		lcd->lcdSetCursorPosition(cursor);
		for (int i = 0; (i < 16) && (((body)[i]) != '\0'); i++) //muestra el tweet en la segunda fila del display
		{
			*lcd << ((body)[i]);
		}
		if ((string(body)).length() > 16)	//No toma el terminador
		{
			body++;
		}
		gui_showtw(boton, vel); //muestra la gui
		al_rest(vel/10); //espera el tiempo que se haya determinado en la gui para mostrar los tweets
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
			lcd->lcdClear();
			cursor = { 0,0 };
			lcd->lcdSetCursorPosition(cursor);
			*lcd << "Ultimo Tweet";	//Informa que se llego al ultimo tweet
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
		lcd = dlcd1;
		boton = DONO;
		break;
	case LCD2:
		lcd->lcdClear();
		lcd = dlcd2;
		boton = DONO;
		break;
	case LCD3:
		lcd->lcdClear();
		lcd = dlcd3;
		boton = DONO;
		break;
	default:
		break;
	}
	return lcd;
}