#ifndef _TWEET_H
#define _TWEET_H

#include <iostream>
#include <exception>
#include <list>
#include <curl/curl.h>
#include "json.hpp"
#include "mylcd.h"
#include<string>
//Funciones auxiliares
void printNames(std::list<std::string> names);
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);
int dostuff(void);
int dostuff(int);


#endif
