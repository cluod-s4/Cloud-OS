// core/kernel/drivers/keyboard.c
// Real keyboard driver

#include "keyboard.h"
#include "screen.h"
#include "interrupts/idt.h"
#include <stdint.h>

#define KEYBOARD_PORT 0x60
#define KEYBOARD_STATUS 0x64

static char key_map[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', 0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', 0, 0, 'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static char key_buffer[256];
static int buffer_index = 0;

void keyboard_handler() {
    uint8_t scancode = 0;
    __asm__ volatile("inb %%dx, %%al" : "=a"(scancode) : "d"(KEYBOARD_PORT));
    
    if (scancode & 0x80) {
        return;
    }
    
    char key = key_map[scancode];
    if (key != 0) {
        if (buffer_index < 255) {
            key_buffer[buffer_index++] = key;
            screen_putchar(key);
        }
    }
}

void keyboard_init() {
    register_irq_handler(1, keyboard_handler);
    screen_print("[+] Keyboard driver initialized\n");
}

char keyboard_read() {
    if (buffer_index > 0) {
        char key = key_buffer[0];
        for (int i = 0; i < buffer_index - 1; i++) {
            key_buffer[i] = key_buffer[i + 1];
        }
        buffer_index--;
        return key;
    }
    return 0;
}

int keyboard_available() {
    return buffer_index > 0;
}
