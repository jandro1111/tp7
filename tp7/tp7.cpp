#include <iostream>
#include <exception>
#include <list>
#include <curl/curl.h>
#include "json.hpp"
#include "mylcd.h"
#include "tweet.h"
#include "claselcd1.h"
//Vamos a usar la librer�a NLOHMANN JSON 



//string

/*
* Main de prueba.
* Obtiene twits de una cuenta p�blica (en este caso, los �ltimos 10 twits de La Naci�n)
*
*/

int main(void)
{
	init_allegro();
	claselcd1 lcd1;
	int canttwits = 20;
	//Si se ingresa otro valor por GUI
	

	
	//claselcd1 j;
	return dostuff(canttwits);
}