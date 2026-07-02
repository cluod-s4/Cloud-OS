// core/kernel/process/scheduler.c
// Real process scheduler

#include "scheduler.h"
#include "memory/memory.h"
#include "drivers/screen.h"

#define MAX_PROCESSES 32
#define STACK_SIZE 4096

typedef struct context {
    uint32_t esp;
    uint32_t ebp;
} context_t;

typedef struct process {
    int pid;
    char name[32];
    context_t context;
    uint8_t stack[STACK_SIZE];
    int state;
    int priority;
} process_t;

static process_t processes[MAX_PROCESSES];
static int current_pid = -1;
static int process_count = 0;

void scheduler_init() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].state = 3;
    }
    screen_print("[+] Scheduler initialized\n");
}

int process_create(const char* name, void* entry_point, int priority) {
    if (process_count >= MAX_PROCESSES) {
        screen_print("Error: Max processes reached\n");
        return -1;
    }
    
    int index = -1;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].state == 3) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -1;
    }
    
    process_t* p = &processes[index];
    p->pid = index;
    
    int i = 0;
    while (name[i] != '\0' && i < 31) {
        p->name[i] = name[i];
        i++;
    }
    p->name[i] = '\0';
    
    p->state = 0;
    p->priority = priority;
    
    uint32_t* stack = (uint32_t*)&p->stack[STACK_SIZE];
    *--stack = (uint32_t)entry_point;
    *--stack = 0;
    p->context.esp = (uint32_t)stack;
    p->context.ebp = (uint32_t)stack;
    
    process_count++;
    
    char pid_str[4];
    int_to_str(index, pid_str, 10);
    screen_print("Process created: ");
    screen_print(name);
    screen_print(" (PID: ");
    screen_print(pid_str);
    screen_print(")\n");
    
    return index;
}

void context_switch(int from, int to) {
    __asm__ volatile(
        "mov %%esp, %0\n"
        "mov %%ebp, %1\n"
        : "=r"(processes[from].context.esp),
          "=r"(processes[from].context.ebp)
    );
    
    __asm__ volatile(
        "mov %0, %%esp\n"
        "mov %1, %%ebp\n"
        : 
        : "r"(processes[to].context.esp),
          "r"(processes[to].context.ebp)
    );
}

void scheduler_run() {
    if (process_count == 0) return;
    
    int next = (current_pid + 1) % MAX_PROCESSES;
    int attempts = 0;
    
    while (attempts < MAX_PROCESSES) {
        if (processes[next].state == 0) {
            break;
        }
        next = (next + 1) % MAX_PROCESSES;
        attempts++;
    }
    
    if (attempts >= MAX_PROCESSES) return;
    
    if (current_pid != -1) {
        context_switch(current_pid, next);
    }
    
    current_pid = next;
    processes[current_pid].state = 1;
}

void scheduler_exit() {
    if (current_pid == -1) return;
    
    processes[current_pid].state = 3;
    process_count--;
    current_pid = -1;
}
