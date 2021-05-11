#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <exception>
#include <list>
#include <curl/curl.h>
#include <string>
#include <time.h>
#include "json.hpp"
#include "mylcd.h"
#include "claselcd1.h"
#include "displaytrini.h"
#include "gui.h"

#define MAXTWITS 200
using namespace std;

enum { DONO = 0, GO = 1, REP, NEXT, PREV, EXIT, LCD1, LCD2, LCD3, WRONG };

struct twits
{
	string date;
	string author;
	string body;
};

struct islcd
{
	bool islcd1;
	bool islcd2;
	bool islcd3;
};



#endif //HEADER_H