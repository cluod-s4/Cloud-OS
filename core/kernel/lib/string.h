// core/kernel/lib/string.h
#ifndef STRING_H
#define STRING_H

#include <stddef.h>

int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
char* strtok(char* str, const char* delim);
char* strchr(const char* str, int c);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* ptr, int value, size_t num);
size_t strlen(const char* str);
int int_to_str(int num, char* str, int base);

#endif
