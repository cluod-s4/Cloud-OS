// core/kernel/process/scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

void scheduler_init();
int process_create(const char* name, void* entry_point, int priority);
void scheduler_run();
void scheduler_exit();

#endif
