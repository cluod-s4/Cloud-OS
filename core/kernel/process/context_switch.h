// core/kernel/process/context_switch.h
#ifndef CONTEXT_SWITCH_H
#define CONTEXT_SWITCH_H

#include <stdint.h>

typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t eip;
} context_t;

typedef struct process process_t;

void context_switch(process_t* from, process_t* to);
void switch_to_user_mode(uint32_t entry_point);
void save_context(context_t* context);
void restore_context(context_t* context);

#endif
