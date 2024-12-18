#ifndef RTOS_BASIC_H
#define RTOS_BASIC_H

#include "rtos.h"

////////////////////////////////////////////////////////////
//  Mutex
////////////////////////////////////////////////////////////

typedef struct {
    Task_ID tid;
    bool is_free;
    Task_Priority max_waiting_priority;
} Mutex;

Result mutex_get_lock(Mutex *mutex, Task_ID tid, size_t timeout) {
    Task_Info *t_info = get_task_info(tid);
    // 
    ENTER_CRITICAL_SECTION();
    {
        if (t_info->current_priority > mutex->max_waiting_priority)
            mutex->max_waiting_priority = t_info->current_priority;
            if (get_task_info(mutex->tid)->current_priority < mutex->max_waiting_priority)
                os_set_task_priority(mutex->tid, mutex->max_waiting_priority);
        else
            os_set_task_priority(tid, mutex->max_waiting_priority);
    }
    EXIT_CRITICAL_SECTION();
    WAIT_FOR_CONDITION(mutex->is_free, timeout);
    // claim the mutex
    ENTER_CRITICAL_SECTION();
    {
        mutex->is_free = false;
        mutex->tid = tid;
    }
    EXIT_CRITICAL_SECTION();
    return RES_OK;
}

Result mutex_release_lock(Mutex *mutex, Task_ID tid) {
    if (mutex->tid = tid) { 
        ENTER_CRITICAL_SECTION();
        {
            mutex->is_free = true;
            mutex->max_waiting_priority = 0;
        }
        EXIT_CRITICAL_SECTION();
        Task_Info *t_info = get_task_info(tid);
        if (t_info->current_priority > t_info->default_priority)
            os_set_task_priority_to_default(tid);
        return RES_OK;
    }
    return RES_FAILURE;
}

////////////////////////////////////////////////////////////
//  Queue
////////////////////////////////////////////////////////////

typedef struct {
    const uint32_t *buf;
    const size_t total_capacity;
    size_t num_elements;
    size_t head_loc;
} Queue;

Result queue_give(Queue *queue, Mutex *mutex, Task_ID tid, uint32_t content, uint32_t timeout) {
    if (queue->num_elements >= queue->total_capacity) return RES_FAILURE;

    Result res = mutex_get_lock(mutex, tid, timeout);
    if (res) return res;

    const uint32_t *head_addr = (queue->buf + (queue->head_loc*sizeof(uint32_t)));
    *((uint32_t *)head_addr) = content;
    queue->num_elements++;
    queue->head_loc = (queue->head_loc + 1) % queue->total_capacity;

    Result res = mutex_release_lock(mutex, tid);
    if (res) return res;

    return RES_OK;
}

uint32_t queue_read_head(Queue *queue) {
    return *(queue->buf + (queue->head_loc*sizeof(uint32_t)));
}

uint32_t queue_take(Queue *queue, Mutex *mutex, Task_ID tid, uint32_t timeout) {
    if (queue->num_elements <= 0) return RES_FAILURE;

    Result res = mutex_get_lock(mutex, tid, timeout);
    if (res) return res;

    size_t tail_loc = (queue->head_loc + queue->num_elements) % queue->total_capacity;
    uint32_t content = *(queue->buf + (tail_loc*sizeof(uint32_t)));
    queue->num_elements--;
    queue->head_loc = (queue->head_loc - 1) % queue->total_capacity;

    Result res = mutex_release_lock(mutex, tid);
    if (res) return res;

    return content;
}

#endif