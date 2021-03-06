#include "claselcd1.h"

claselcd1::claselcd1()
{
	displaylcd1 = NULL; fuente = NULL;
	cursor = { 0, 0 };
	fuente = al_load_font("Montserrat-Black.ttf", SIZEFUENTE, 0);	// Carga la fuente
	displaylcd1 = al_create_display(ANCHODISPLAY, ALTODISPLAY);	//Crea el display
	al_set_window_title(displaylcd1, "Display 1");
	lcdDisplayUpdate();
}

claselcd1::~claselcd1()
{
	al_destroy_font(fuente);
	al_destroy_display(displaylcd1);	//Destruye el display y la font
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
}

bool claselcd1::lcdInitOk()
{
	if (displaylcd1 == NULL || fuente == NULL)	//Si no se inicializaron correctamente
	{
		cout << "Display or Font could not be initiated." << endl;
		return false;
	}
	return true;
}

lcdError& claselcd1::lcdGetError() {
		return estado;
}

bool claselcd1::lcdClear()
{
	for (char t = 0; t < 32; ++t)	//Vacia todos los caracteres
	{
		text[t] = ' ';
	}
	cursor = { 0, 0 };	//Pone el cursor en la posicion original
	if (!lcdDisplayUpdate())
	{
		return false;
	}
	return true;
}

bool claselcd1::lcdClearToEOL()
{
	cursorPosition tempcursor = cursor;
	for (tempcursor.column; tempcursor.column < (((tempcursor.row)+1)*16); ++(tempcursor.column))	//Limpia desde la posicion actual hasta el fin de la linea
	{
		text[((tempcursor.row) * 16) + (tempcursor.column)] = ' ';
	}
	if (!lcdDisplayUpdate())
	{
		return false;
	}
	return true;
}

basicLCD& claselcd1::operator<<(const unsigned char c)
{
	if (lcdCharIn(cursor, (unsigned char)c))	//Carga el caracter recibido en el parametro en la posicion recibida, si estos fueran validos
	{
		lcdMoveCursorRight();
	}
	lcdDisplayUpdate();
	return *this;
}

basicLCD& claselcd1::operator<<(const char* c)
{
	for (int i = 0; (i < 32) && (c[i] != '\0'); ++i)	//Carga el string recibido en el parametro en la posicion recibida
	{
		if (lcdCharIn(cursor, c[i]))
		{
			lcdMoveCursorRight();
		}
	}
	lcdDisplayUpdate();
	return *this;
}

bool claselcd1::lcdMoveCursorUp()
{
	cursorPosition tempcursor = { (cursor.row) -1, cursor.column };	//Mueve el cursor hacia arriba
	if (!lcdSetCursorPosition(tempcursor))
	{
		return false;
	}
	return true;
}

bool claselcd1::lcdMoveCursorDown()	//Mueve el cursor hacia abajo
{
	cursorPosition tempcursor = { (cursor.row) + 1, cursor.column };
	if (!lcdSetCursorPosition(tempcursor))
	{
		return false;
	}
	return true;
}

bool claselcd1::lcdMoveCursorRight()
{
	cursorPosition tempcursor = { cursor.row, (cursor.column) + 1 };	//Mueve el cursor hacia la derecha
	if (!lcdSetCursorPosition(tempcursor))
	{
		if (tempcursor.row == 0 && tempcursor.column == 16)	//Si se llega al final da la vuelta
		{
			tempcursor = {1, 0};
		}
		else if (tempcursor.row == 1 && tempcursor.column == 16)
		{
			tempcursor = { 0, 0 };
		}
		if (!lcdSetCursorPosition(tempcursor))
		{
			return false;
		}
	}
	return true;
}

bool claselcd1::lcdMoveCursorLeft()
{
	cursorPosition tempcursor = {cursor.row, (cursor.column)-1 };	//Mueve el cursor hacia la derecha
	if (!lcdSetCursorPosition(tempcursor))
	{
		if (tempcursor.row == 0 && tempcursor.column == -1)	//Si se llega al final da la vuelta
		{
			tempcursor = { 1, 15 };
		}
		else if (tempcursor.row == 1 && tempcursor.column == -1)
		{
			tempcursor = { 0, 15 };
		}
		if (!lcdSetCursorPosition(tempcursor))
		{
			return false;
		}
	}
	return true;
}

bool claselcd1::lcdSetCursorPosition(const cursorPosition pos)
{
	if ( (pos.row >= 0 && pos.row <= 1) && (pos.column >= 0 && pos.column <= 15))	//Valida la posicion ingresada para el cursor
	{
		cursor = pos;
		lcdDisplayUpdate();
		return true;
	}
	else
	{
		return false;
	}
}

cursorPosition claselcd1::lcdGetCursorPosition()
{
	return cursor;	//Retorna la posicion del cursor
}

bool claselcd1::lcdCharIn(cursorPosition pos, const unsigned char character)
{
	if (lcdSetCursorPosition(pos))	//Valida la posicion del cursor
	{
		text[((cursor.row) * 16) + (cursor.column)] = character;
		lcdDisplayUpdate();
		return true;
	}
	return false;
}

bool claselcd1::lcdDisplayUpdate()	//Actualiza el display
{
	string tempstring;
	int i, j;
	al_set_target_backbuffer(displaylcd1);
	al_clear_to_color(BACKCOLOR);
	for (i = 0; i <= 1; ++i)
	{
		tempstring.clear();
		for (j = 0; j <= 15; ++j)
		{
			tempstring.push_back((text[(i * 16) + (j)]));
		}
		tempstring.push_back('\0');
		al_draw_textf(fuente, FONTCOLOR, 10, (i) * 100, 0, "%s", tempstring.c_str());
	}
	al_flip_display();
	return true;
}