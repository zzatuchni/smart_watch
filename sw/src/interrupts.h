#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "util.h"
#include "stm32l47x/uart.h"

void _on_hard_fault(void);

void _on_mem_fault(void);

void _on_bus_fault(void);

void _on_usage_fault(void);

void _on_usage_fault(void);

void _on_usage_fault(void);

void _on_systick(void);

void _on_uart2_interrupt(void);

#endif