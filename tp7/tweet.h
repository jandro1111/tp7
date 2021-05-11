#ifndef _TWEET_H
#define _TWEET_H

#include <iostream>
#include <exception>
#include <list>
#include <curl/curl.h>
#include "json.hpp"
#include "mylcd.h"
#include <string>
#include "header.h"
#include "logic.h"

//Funciones auxiliares

//ajusta la fecha al formato DD/MM/AA – hh:mm 
std::string dateadj(std::string date);
//llena la lista de tweets
void printNames(std::list<std::string> names, std::list<twits>&,string);

static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);
//se conecta a tweter y trae tweets baja cantidad por defecto de tweeter
int dostuff(string autor, std::list<twits>& tweet, string& error, int& boton, basicLCD** lcd, bool* islcd, bool& prog_exit);
//se conecta a tweter y trae tweets baja cantidad especificada por el usuario
int dostuff(string, int, std::list<twits>&, string& error, int& boton, basicLCD** lcd, bool* islcd, bool& prog_exit);


#endif
