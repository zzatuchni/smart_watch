#ifndef RTOS_H
#define RTOS_H

#include "../util.h"

////////////////////////////////////////////////////////////
//  Constants
////////////////////////////////////////////////////////////

#define SCHEDULER_TICKS_PER_SECOND 1000
#define MAX_NUM_TASKS   5
#define TASK_STACK_SIZE 512

#define ENTER_CRITICAL_SECTION() {};
#define EXIT_CRITICAL_SECTION() {};

////////////////////////////////////////////////////////////
//  Types
////////////////////////////////////////////////////////////

typedef uint8_t Task_ID;
typedef uint8_t Task_Priority;

typedef enum { TS_READY, TS_RUNNING, TS_BLOCKED } Task_State;

typedef struct Task_Info Task_Info;
struct Task_Info {
    void        (*funcPtr)(void);

    volatile void           *stackPtr;
    volatile Task_State     state;
    volatile Task_Priority  current_priority;
    volatile Task_Priority  default_priority;
    volatile Task_Info      *nextTaskDataPtr;

    volatile uint8_t     dataPresent;
};

typedef struct {
    volatile Task_Priority max_task_priority;
} Scheduler_Info;

////////////////////////////////////////////////////////////
//  Data
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//  Functions
////////////////////////////////////////////////////////////

Task_Info *get_task_info(Task_ID tid);

Task_ID os_add_task(const void *funcPtr, Task_Priority priority);

Result os_delete_task(Task_ID tid);

Result os_set_task_priority(Task_ID tid, Task_Priority priority);

Result os_set_task_priority_to_default(Task_ID tid);

Result os_set_task_state(Task_ID tid, Task_State state);

Result os_start_scheduler();

Result os_stop_scheduler();

#endif