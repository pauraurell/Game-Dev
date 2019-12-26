#ifndef __p2Log_H__
#define __p2Log_H__

#include <windows.h>
#include <stdio.h>

#define LOG(print, format, ...) log(__FILE__, __LINE__, print, format, __VA_ARGS__)

void log(const char file[], int line, bool print, const char* format, ...);

#endif