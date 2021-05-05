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
		//error.setError ("Init error", "No se inicializo bien allegro", ALINIT);
		allegroDestroy();
	}
}

//Destructor
displayTrini::~displayTrini() {
	cout << "destructor called \n" << endl;
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
		//error.setError("MoveCursorUp error", "El cursor se encontraba la primera linea", UP_BOUNDARY);
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
		//error.setError("MoveCursorDown error", "El cursor se encontraba la ultima linea", DOWN_BOUNDARY);
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
		//error.setError("MoveCursorRight error", "El cursor se encontraba al final del display", RIGHT_BOUNDARY);
		return false;
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
		//error.setError("MoveCursorLeft error", "El cursor se encontraba al principio del display", LEFT_BOUNDARY);
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



basicLCD& displayTrini::operator<<(unsigned const char c) {
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
	string lcdString;
	string temp = ((const char*)c);

	lcdString.reserve(FILAMAX * COLMAX);
	for (int i = 0; i < FILAMAX; i++) {
		lcdString += this->str[i];
	}

	if (temp.size() <= 32) {
		while ((size_t)CURSOR_POSITION > lcdString.length()) {
			lcdString += ' ';
		}

		int totalNewPosition = 0;
		cursorPosition cur = { 0, 0 };

		if (CURSOR_POSITION + temp.size() < FILAMAX * COLMAX) {
			lcdString.replace(CURSOR_POSITION, temp.size(), temp);
		
			totalNewPosition = (int)temp.size() + CURSOR_POSITION;
		}
		else {
			size_t charsCopied = FILAMAX * COLMAX - CURSOR_POSITION;
			lcdString.replace(CURSOR_POSITION, charsCopied, temp, 0, charsCopied);
			lcdString.replace(0, temp.size() - charsCopied, temp, charsCopied, temp.size() - charsCopied);

			totalNewPosition = (int)(temp.size() - charsCopied);
		}
		if (totalNewPosition % (FILAMAX*COLMAX) == 0) {
			cur.row = 0;
			cur.column= 0;
		}
		else {
			cur.row = (int)(totalNewPosition / (FILAMAX*COLMAX));
			cur.column = (int)(totalNewPosition - cur.row * (FILAMAX*COLMAX));
		}

		lcdSetCursorPosition(cur);
	}
	else {
		//error.setError("Writing exceeded error", "El maximo largo es de 32 caracteres", WRITING_EXCEEDED);
		lcdString.assign(temp, temp.length() - FILAMAX * COLMAX, FILAMAX * COLMAX);
		cursorPosition cur = {0, 0};
	}
	
	string piece;
	bool foundEndOfString = false;
	for (int i = 0; i < FILAMAX; i++) {
		piece = lcdString.substr(piece.size() * i, COLMAX);
		const char * newStr = piece.c_str();

		for (int j = 0; j < COLMAX; j++) {
			if (!foundEndOfString) {
				str[i][j] = *(newStr + j);
			}
			else {
				str[i][j] = '\0';
			}

			if (*(newStr + j) == '\0') {
				foundEndOfString = true;
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

bool displayTrini::allegroInit() {
	//ALLEGRO AND ALLEGRO ADDONS INITIALIZATION
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro!\n");
		return false;
	}
	if (!al_init_image_addon()) {
		fprintf(stderr, "Failed to initialize image addon!\n");
		return false;
	}
	if (!al_init_font_addon()) {
		fprintf(stderr, "Failed to initialize allegro fonts!\n");
		return false;
	}
	if (!al_init_ttf_addon()) {
		fprintf(stderr, "Failed to initialize allegro ttf fonts!\n");
		return false;
	}
	if (!al_init_primitives_addon())
	{
		fprintf(stderr, "failed to initialize primitives!\n");
		return false;
	}
	if (!al_install_keyboard()) {
		fprintf(stderr, "Failed to initialize allegro keyboard!\n");
		return false;
	}
	if (!(all.display = al_create_display(WIDTH, HEIGHT))) {
		cout << "failed to create display!" << endl;
		//error.setError("Display error", "No se pudo inicializar el display", DISPLAY);
		return false;
	}
	al_set_window_title(all.display, "Display 3");
	if (!(all.font = al_load_ttf_font(FONT_TRINI, 100, 0))) {
		cout << "failed to load font!" << endl;
		al_destroy_display(all.display);
		//error.setError("Font error", "No se pudo inicializar la fuente", FONT);
		return false;
	}
	all.eventQueue = al_create_event_queue();
	if (!all.eventQueue) {
		cout << "failed to load event queue!" << endl;
		al_destroy_display(all.display);
		al_destroy_font(all.font);
		return false;
	}

	al_flip_display();
	al_register_event_source(all.eventQueue, al_get_keyboard_event_source());              //Evento teclado      
	al_register_event_source(all.eventQueue, al_get_display_event_source(all.display));        //Cruz de salir
	return true;
}

void displayTrini::draw(void) {
	al_set_target_backbuffer(all.display);
	al_clear_to_color(ALLEGRO_COLOR(BLACK));

	string helpMe;

	// Ugly hack
	/*helpMe = str[0];
	while (helpMe.size() > COLMAX) {
		helpMe.pop_back();
	}*/
	for (int i = 0; (i < 16) && (str[0][i] != '\0'); i++)
	{
		helpMe.push_back(str[0][i]);
	}
	helpMe.push_back('\0');
	al_draw_textf(all.font, ALLEGRO_COLOR(WHITE), 0, 0, 0, "%s", helpMe.c_str());

	// Ugly hack
	/*helpMe = str[1];
	while (helpMe.size() > COLMAX) {
		helpMe.pop_back();
	}*/
	helpMe.clear();
	for (int i = 0; (i < 16) && (str[1][i] != '\0'); i++)
	{
		helpMe.push_back(str[1][i]);
	}
	helpMe.push_back('\0');
	al_draw_textf(all.font, ALLEGRO_COLOR(WHITE), 0, al_get_font_line_height(all.font), 0, "%s", helpMe.c_str());

	al_flip_display();
}

void displayTrini::allegroDestroy() {
	al_destroy_font(all.font);
	al_destroy_event_queue(all.eventQueue);
	al_destroy_display(all.display);
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
	al_uninstall_keyboard();
	return;
}