// core/kernel/drivers/screen.c
// Real screen driver

#include "screen.h"
#include <stdint.h>
#include <stddef.h>

#define VIDEO_MEMORY 0xb8000
#define COLUMNS 80
#define ROWS 25
#define COLOR_WHITE_ON_BLACK 0x0f

static uint16_t* video_memory = (uint16_t*)VIDEO_MEMORY;
static int cursor_row = 0;
static int cursor_col = 0;

void screen_init() {
    clear_screen();
}

void clear_screen() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            video_memory[row * COLUMNS + col] = 
                (COLOR_WHITE_ON_BLACK << 8) | ' ';
        }
    }
    cursor_row = 0;
    cursor_col = 0;
}

void screen_putchar(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else if (c == '\t') {
        cursor_col = (cursor_col + 4) & ~3;
    } else {
        video_memory[cursor_row * COLUMNS + cursor_col] = 
            (COLOR_WHITE_ON_BLACK << 8) | c;
        cursor_col++;
    }
    
    if (cursor_col >= COLUMNS) {
        cursor_row++;
        cursor_col = 0;
    }
    
    if (cursor_row >= ROWS) {
        scroll_screen();
    }
}

void screen_print(const char* str) {
    while (*str) {
        screen_putchar(*str++);
    }
}

void scroll_screen() {
    for (int row = 0; row < ROWS - 1; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            video_memory[row * COLUMNS + col] = 
                video_memory[(row + 1) * COLUMNS + col];
        }
    }
    
    for (int col = 0; col < COLUMNS; col++) {
        video_memory[(ROWS - 1) * COLUMNS + col] = 
            (COLOR_WHITE_ON_BLACK << 8) | ' ';
    }
    
    cursor_row = ROWS - 1;
    cursor_col = 0;
}
