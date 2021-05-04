#ifndef _TWEET_H
#define _TWEET_H

#include <iostream>
#include <exception>
#include <list>
#include <curl/curl.h>
#include "json.hpp"
#include "mylcd.h"
#include<string>
#include"header.h"
//Funciones auxiliares
void printNames(std::list<std::string> names, std::list<twits>&);
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);
int dostuff(std::list<twits>&);
int dostuff(int, std::list<twits>&);


#endif
