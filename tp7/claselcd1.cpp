#include "claselcd1.h"

claselcd1::claselcd1()
{
	displaylcd1 = NULL; fuente = NULL;
	cursor = { 0, 0 };
	fuente = al_load_font("colleges.ttf", SIZEFUENTE, 0);	// Carga la fuente
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
		//errorptr->ErrorName = "Display Or Font Init";
		//errorptr->ErrorDescription = "Display or Font could not be initiated.";
		//errorptr->ErrorCode = DISPLAY;
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
		//errorptr->ErrorName = "Display Not Updated";
		//errorptr->ErrorDescription = "The Display Could Not Be Updated.";
		//errorptr->ErrorCode = DISPLAY;
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
		//errorptr->ErrorName = "Display Not Updated";
		//errorptr->ErrorDescription = "The Display Could Not Be Updated.";
		//errorptr->ErrorCode = DISPLAY;
		return false;
	}
	return true;
}

basicLCD& claselcd1::operator<<(const unsigned char c)
{
	if (lcdCharIn(cursor, c))	//Carga el caracter recibido en el parametro en la posicion recibida, si estos fueran validos
	{
		lcdMoveCursorRight();
	}
	lcdDisplayUpdate();
	return *this;
}

basicLCD& claselcd1::operator<<(const char* c)
{
	for (int i = 0; (i < 32) && (c[i] != '\0'); ++i)	//Carga el string recibido en el parametro en la posicion recibida, si estos fueran valido
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
		//errorptr->ErrorName = "Cursor Limit";
		//errorptr->ErrorDescription = "Can't Move Cursor Up.";
		//errorptr->ErrorCode = CURSOR;
		return false;
	}
	return true;
}

bool claselcd1::lcdMoveCursorDown()	//Mueve el cursor hacia abajo
{
	cursorPosition tempcursor = { (cursor.row) + 1, cursor.column };
	if (!lcdSetCursorPosition(tempcursor))
	{
		//errorptr->ErrorName = "Cursor Limit";
		//errorptr->ErrorDescription = "Can't Move Cursor Down.";
		//errorptr->ErrorCode = CURSOR;
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
			//errorptr->ErrorName = "Cursor Limit";
			//errorptr->ErrorDescription = "Can't Move Cursor Right.";
			//errorptr->ErrorCode = CURSOR;
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
			//errorptr->ErrorName = "Cursor Limit";
			//errorptr->ErrorDescription = "Can't Move Cursor Left.";
			//errorptr->ErrorCode = CURSOR;
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
		//errorptr->ErrorName = "Invalida Cursor";
		//errorptr->ErrorDescription = "The Cursor Position is Invalid.";
		//errorptr->ErrorCode = CURSOR;
		return false;
	}
}

cursorPosition claselcd1::lcdGetCursorPosition()
{
	return cursor;	//Retorna la posicion del cursor
}

bool claselcd1::lcdCharIn(cursorPosition pos, char character)
{
	if (lcdSetCursorPosition(pos))	//Valida la posicion del cursor
	{	//Valida el caracter
		/*if ( ((character >= 65) && (character <= 90)) || ((character >= 97) && (character <= 122)) || ((character >= 48) && (character <= 57)) 
			|| (character == 36) || (character == 43) || (character == 45) || (character == 61) || (character == 34) || (character == 35) || (character == ' ')
			|| (character == 39) || (character == 44) || (character == 46) || (character == 58) || (character == 59) || (character == 636) || (character == 33))
		{*/
			text[((cursor.row) * 16) + (cursor.column)] = character;
			lcdDisplayUpdate();
			return true;
		//}
		//else
		//{
		//	//errorptr->ErrorName = "Invalida Character";
		//	//errorptr->ErrorDescription = "One or more characters entered were invalid and ignored.";
		//	//errorptr->ErrorCode = CHARACTER;
		//}
	}
	return false;
}

bool claselcd1::lcdDisplayUpdate()	//Actualiza el display
{
	al_set_target_backbuffer(displaylcd1);
	al_clear_to_color(BACKCOLOR);
	for (int i = 0; i <= 1; ++i)
	{
		for (int j = 0; j <= 15; ++j)
		{
			char tempstring[2] = { (text[(i * 16) + (j)]), '\0' };
			al_draw_text(fuente, FONTCOLOR, ((j) * 50) + OFFSET, (i) * 50, 0, tempstring);
		}
	}
	al_flip_display();
	return true;
}