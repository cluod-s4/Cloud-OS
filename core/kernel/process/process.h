// core/kernel/process/process.h
#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    PROCESS_READY = 0,
    PROCESS_RUNNING = 1,
    PROCESS_WAITING = 2,
    PROCESS_TERMINATED = 3
} process_state_t;

typedef struct process {
    int pid;
    char name[32];
    process_state_t state;
    int priority;
    uint32_t esp;
    uint32_t ebp;
    void* stack;
    void* entry_point;
} process_t;

void process_init();
int process_create(const char* name, void* entry_point, int priority);
void process_terminate(int pid);
process_t* process_get(int pid);
process_t* process_get_current();
void process_set_state(int pid, process_state_t state);
void process_yield();

#endif
