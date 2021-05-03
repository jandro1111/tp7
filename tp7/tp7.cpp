#include <iostream>
#include <exception>
#include <list>
#include <curl/curl.h>
#include "json.hpp"
#include "mylcd.h"
#include "tweet.h"
#include "claselcd1.h"
//Vamos a usar la librería NLOHMANN JSON 



//string

/*
* Main de prueba.
* Obtiene twits de una cuenta pública (en este caso, los últimos 10 twits de La Nación)
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