// core/kernel/drivers/screen.h
#ifndef SCREEN_H
#define SCREEN_H

void screen_init();
void clear_screen();
void screen_putchar(char c);
void screen_print(const char* str);
void scroll_screen();

#endif
