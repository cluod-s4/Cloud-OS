// core/kernel/process/context_switch.c
// Real context switch implementation

#include "context_switch.h"
#include "process.h"
#include "drivers/screen.h"

void context_switch(process_t* from, process_t* to) {
    if (from == NULL || to == NULL) {
        return;
    }
    
    // Save current process state
    __asm__ volatile(
        "mov %%esp, %0\n"
        "mov %%ebp, %1\n"
        : "=r"(from->esp), "=r"(from->ebp)
        : : "memory"
    );
    
    // Load new process state
    __asm__ volatile(
        "mov %0, %%esp\n"
        "mov %1, %%ebp\n"
        : : "r"(to->esp), "r"(to->ebp)
        : "memory"
    );
}

void switch_to_user_mode(uint32_t entry_point) {
    __asm__ volatile(
        "mov $0x23, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "push $0x23\n"
        "push %%esp\n"
        "pushf\n"
        "push $0x1B\n"
        "push %0\n"
        "iret\n"
        : : "r"(entry_point) : "memory"
    );
}

void save_context(context_t* context) {
    __asm__ volatile(
        "mov %%eax, %0\n"
        "mov %%ebx, %1\n"
        "mov %%ecx, %2\n"
        "mov %%edx, %3\n"
        "mov %%esi, %4\n"
        "mov %%edi, %5\n"
        "mov %%ebp, %6\n"
        "mov %%esp, %7\n"
        : "=m"(context->eax), "=m"(context->ebx),
          "=m"(context->ecx), "=m"(context->edx),
          "=m"(context->esi), "=m"(context->edi),
          "=m"(context->ebp), "=m"(context->esp)
        : : "memory"
    );
}

void restore_context(context_t* context) {
    __asm__ volatile(
        "mov %0, %%eax\n"
        "mov %1, %%ebx\n"
        "mov %2, %%ecx\n"
        "mov %3, %%edx\n"
        "mov %4, %%esi\n"
        "mov %5, %%edi\n"
        "mov %6, %%ebp\n"
        "mov %7, %%esp\n"
        : : "m"(context->eax), "m"(context->ebx),
            "m"(context->ecx), "m"(context->edx),
            "m"(context->esi), "m"(context->edi),
            "m"(context->ebp), "m"(context->esp)
        : "memory"
    );
}
