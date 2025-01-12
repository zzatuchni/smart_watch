#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "util.h"
#include "devices/stm32l47x/basic.h"
#include "devices/misc/misc.h"
#include "debug.h"

void _on_hard_fault(void);

void _on_mem_fault(void);

void _on_bus_fault(void);

void _on_usage_fault(void);

void _on_usage_fault(void);

void _on_usage_fault(void);

void _on_systick(void);

void _on_uart2_interrupt(void);

void _on_button1_press(void);

#endif