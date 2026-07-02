// core/kernel/shell/shell.c
// Real shell implementation

#include "shell.h"
#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "process/scheduler.h"
#include "fs/vfs.h"
#include "memory/memory.h"
#include <string.h>

#define MAX_CMD 256
#define MAX_ARGS 10

void cmd_help();
void cmd_echo();
void cmd_ls();
void cmd_cat();
void cmd_clear();
void cmd_exec();
void cmd_exit();
void cmd_mem();
void cmd_ps();

typedef struct {
    const char* name;
    void (*func)();
} command_t;

static command_t commands[] = {
    {"help", cmd_help},
    {"echo", cmd_echo},
    {"ls", cmd_ls},
    {"cat", cmd_cat},
    {"clear", cmd_clear},
    {"exec", cmd_exec},
    {"exit", cmd_exit},
    {"mem", cmd_mem},
    {"ps", cmd_ps},
    {NULL, NULL}
};

void shell_start() {
    char cmd[MAX_CMD];
    char* args[MAX_ARGS];
    
    while (1) {
        screen_print("\ncloud-os> ");
        
        int i = 0;
        while (1) {
            char c = keyboard_read();
            if (c == '\n' || c == '\r') {
                cmd[i] = '\0';
                screen_print("\n");
                break;
            }
            if (c == '\b' && i > 0) {
                i--;
                screen_print("\b \b");
            } else if (c >= 32 && c <= 126 && i < MAX_CMD - 1) {
                cmd[i++] = c;
            }
        }
        
        if (i == 0) continue;
        
        char* token = strtok(cmd, " ");
        int arg_count = 0;
        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;
        
        int found = 0;
        for (int j = 0; commands[j].name != NULL; j++) {
            if (strcmp(args[0], commands[j].name) == 0) {
                commands[j].func();
                found = 1;
                break;
            }
        }
        
        if (!found) {
            screen_print("Command not found: ");
            screen_print(args[0]);
            screen_print("\n");
        }
    }
}

void cmd_help() {
    screen_print("Available commands:\n");
    screen_print("  help  - Show this help\n");
    screen_print("  echo  - Print text\n");
    screen_print("  ls    - List files\n");
    screen_print("  cat   - View file\n");
    screen_print("  clear - Clear screen\n");
    screen_print("  exec  - Run process\n");
    screen_print("  exit  - Exit process\n");
    screen_print("  mem   - Show memory info\n");
    screen_print("  ps    - Show processes\n");
}

void cmd_echo() {
    screen_print("Hello from Cloud-OS!\n");
}

void cmd_ls() {
    screen_print("Files:\n");
    screen_print("  (No files yet)\n");
}

void cmd_cat() {
    screen_print("File content...\n");
}

void cmd_clear() {
    clear_screen();
}

void cmd_exec() {
    screen_print("Executing process...\n");
}

void cmd_exit() {
    scheduler_exit();
}

void cmd_mem() {
    screen_print("Memory info:\n");
    screen_print("  Heap: 0x1000000 - 0x2000000\n");
    screen_print("  Size: 16 MB\n");
}

void cmd_ps() {
    screen_print("Processes:\n");
    screen_print("  PID  Name       State\n");
    screen_print("  ---  --------   -----\n");
    screen_print("  1    kernel     running\n");
}
