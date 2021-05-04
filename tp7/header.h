#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include<time.h>
#define MAXTWITS 200
using namespace std;

enum { DONO = 0, GO = 1, REP, NEXT, PREV, EXIT, LCD1, LCD2, LCD3, CANCEL, CLOSE };

struct twits
{
	string date;
	string author;
	string body;
};



#endif //HEADER_H