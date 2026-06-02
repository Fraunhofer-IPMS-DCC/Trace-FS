#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef void (*entry_func_t) (void);

typedef struct task_t {
    uint16_t id;
    entry_func_t entry;
    uint32_t period;
    uint32_t priority;
} task_t;

void scheduler_init (void);
void scheduler_enable (void);
void scheduler_disable (void);
void scheduler_run (void);
void scheduler_next (void);

#endif /* SCHEDULER_H */
