// core/kernel/process/process.c
// Real process management

#include "process.h"
#include "memory/heap.h"
#include "drivers/screen.h"
#include <string.h>

#define MAX_PROCESSES 64
#define STACK_SIZE 4096

static process_t processes[MAX_PROCESSES];
static int process_count = 0;

void process_init() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].pid = -1;
        processes[i].state = PROCESS_TERMINATED;
    }
    screen_print("[+] Process manager initialized\n");
}

int process_create(const char* name, void* entry_point, int priority) {
    if (process_count >= MAX_PROCESSES) {
        screen_print("Error: Max processes reached\n");
        return -1;
    }
    
    int pid = -1;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == -1) {
            pid = i;
            break;
        }
    }
    
    if (pid == -1) {
        return -1;
    }
    
    process_t* p = &processes[pid];
    p->pid = pid;
    p->priority = priority;
    p->state = PROCESS_READY;
    p->entry_point = entry_point;
    
    strcpy(p->name, name);
    
    // Allocate stack
    p->stack = kmalloc(STACK_SIZE);
    if (p->stack == NULL) {
        return -1;
    }
    
    // Initialize stack
    uint32_t* stack_top = (uint32_t*)((uint8_t*)p->stack + STACK_SIZE);
    *--stack_top = (uint32_t)entry_point;
    *--stack_top = 0;
    p->esp = (uint32_t)stack_top;
    p->ebp = (uint32_t)stack_top;
    
    process_count++;
    
    char pid_str[4];
    int_to_str(pid, pid_str, 10);
    screen_print("Process created: ");
    screen_print(name);
    screen_print(" (PID: ");
    screen_print(pid_str);
    screen_print(")\n");
    
    return pid;
}

void process_terminate(int pid) {
    if (pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        return;
    }
    
    process_t* p = &processes[pid];
    p->state = PROCESS_TERMINATED;
    p->pid = -1;
    
    if (p->stack) {
        kfree(p->stack);
        p->stack = NULL;
    }
    
    process_count--;
}

process_t* process_get(int pid) {
    if (pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        return NULL;
    }
    return &processes[pid];
}

process_t* process_get_current() {
    // In real implementation, track current process
    return NULL;
}

void process_set_state(int pid, process_state_t state) {
    if (pid < 0 || pid >= MAX_PROCESSES || processes[pid].pid == -1) {
        return;
    }
    processes[pid].state = state;
}

void process_yield() {
    // Yield CPU to next process
    // Implemented in scheduler
}
