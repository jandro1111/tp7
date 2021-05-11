#include "displaytrini.h"
#define CURSOR_POSITION		((size_t)this->cursor.row*(COLMAX) + (size_t)this->cursor.column)

using namespace std;


/****************************************************************
* Funciones del display
*
****************************************************************/

//Constructor
displayTrini::displayTrini() {
	initOk = true;
	initOk = allegroInit();
	lcdClear();
	if (!lcdInitOk()) {
		allegroDestroy();
	}
}

//Destructor
displayTrini::~displayTrini() {
	lcdClear();
	allegroDestroy();
}

bool displayTrini::lcdInitOk() {
	return initOk;
}

lcdError& displayTrini::lcdGetError() {
	return error;
}

bool displayTrini::lcdClear() {
	int i, j;
	for (i = 0; i < FILAMAX; i++) {
		for (j = 0; j < COLMAX; j++) {
			str[i][j] = '\0';
		}
	}
	const cursorPosition origen = { 0,0 };
	lcdSetCursorPosition(origen);
	draw();
	return true;
}


bool displayTrini::lcdClearToEOL() {
	for (int i = cursor.row; i < FILAMAX; i++) {
		int j = i > cursor.row ? 0 : cursor.column;
		for (; j < COLMAX; j++) {
			str[i][j] = 0;
		}
	}
	draw();
	return true;
}


bool displayTrini::lcdMoveCursorUp() {
	if (cursor.row > FILAMIN) {
		cursor.row -= 1;
		draw();
		return true;
	}
	else {
		return false;
	}
}

bool displayTrini::lcdMoveCursorDown() {
	if (cursor.row < FILAMAX) {
		cursor.row += 1;
		draw();
		return true;
	}
	else {
		return false;
	}
}

bool displayTrini::lcdMoveCursorRight() {  
	if (cursor.column < COLMAX) {
		cursor.column += 1;
		draw();
		return true;
	}
	else if ((cursor.row == FILAMIN) && (cursor.column == (COLMAX - 1))) {
		cursor.column = COLMIN;
		cursor.row++;
		return true;
	}
	else if ((cursor.row == (FILAMAX - 1)) && (cursor.column == (COLMAX - 1))) {
		cursor.row = FILAMIN;
		cursor.column = COLMIN;
		return true;
	}
	return false;
}

bool displayTrini::lcdMoveCursorLeft() {
	if (cursor.column > COLMIN) {
		cursor.column -= 1;
		draw();
		return true;
	}
	else if ((cursor.row == (FILAMAX-1)) && (cursor.column == COLMIN)) {
		cursor.row--;
		cursor.column = COLMAX - 1;
		return true;
	}
	else if ((cursor.row == FILAMIN) && (cursor.column == COLMIN)) {
		return false;
	}
	return false;
}

bool displayTrini::lcdSetCursorPosition(const cursorPosition c) {
	cursor.column = c.column;
	cursor.row = c.row;
	return true;
}

cursorPosition displayTrini::lcdGetCursorPosition() {
	cursorPosition c;
	c.column = cursor.column;
	c.row = cursor.row;
	return c;
}



basicLCD& displayTrini::operator<<(const unsigned char c) {
	str[cursor.row][cursor.column] = c;
	if (cursor.column < (COLMAX - 1)) {
		cursor.column += 1;
	}
	else if ((cursor.column == (COLMAX - 1)) && (cursor.row == FILAMIN)) {
		cursor.row += 1;
		cursor.column = COLMIN;
	}
	else if ((cursor.column == (COLMAX - 1)) && (cursor.row == (FILAMAX-1))) {
		cursor.row -= 1;
		cursor.column = COLMIN;
	}
	draw();
	return *this;
}

basicLCD& displayTrini::operator<<(const char* c) {
	int size = 0;
	char term = *c;
	int i = 0;

	while (term != '\0')
	{
		term = *(c + i);  //Busca el terminador para poder determinar el tamanio del string enviado
		size++;
		i++;
	}
	if (size < 32)   //Si es menor que 32 guardo sin problemas en el arreglo el string
	{
		int cont = 0;
		while (cont <= size)
		{
			int i = cursor.row;
			int j = cursor.column;
			str[i][j] = *(c + (i * COLMAX) + j);
			cont++;
			lcdMoveCursorRight();
		}
	}
	else //Si el string es mas largo que el display, solo se muestra la ultima parte
	{
		for (int i = 0; i < FILAMAX; i++)
		{
			for (int j = 0; j < COLMAX; j++)
			{
				str[i][j] = *(c + size - (FILAMAX * COLMAX));
			}
		}
	}
	draw();
	return *this;
}

/******************************************************
* Funciones de allegro
*
*******************************************************/
//inicializa allegro
bool displayTrini::allegroInit() {
	if (!(all.display = al_create_display(WIDTH, HEIGHT))) {
		cout << "failed to create display!" << endl;
		return false;
	}
	al_set_window_title(all.display, "Display 3");
	if (!(all.font = al_load_ttf_font(FONT_TRINI, 80, 0))) {
		cout << "failed to load font!" << endl;
		al_destroy_display(all.display);
		return false;
	}
	al_flip_display();
	return true;
}

//dibuja el display con todos los caracteres
void displayTrini::draw(void) {
	al_set_target_backbuffer(all.display);
	al_clear_to_color(ALLEGRO_COLOR(BLACK));

	string helpMe;
	for (int i = 0; (i < 16) && (str[0][i] != '\0'); i++)
	{
		helpMe.push_back(str[0][i]);
	}
	helpMe.push_back('\0');
	al_draw_textf(all.font, ALLEGRO_COLOR(WHITE), 0, 0, 0, "%s", helpMe.c_str());

	helpMe.clear();
	for (int i = 0; (i < 16) && (str[1][i] != '\0'); i++)
	{
		helpMe.push_back(str[1][i]);
	}
	helpMe.push_back('\0');
	al_draw_textf(all.font, ALLEGRO_COLOR(WHITE), 0, 80, 0, "%s", helpMe.c_str());

	al_flip_display();
}

//destruye los elementos de allegro
void displayTrini::allegroDestroy() {
	al_destroy_font(all.font);
	al_destroy_display(all.display);
}