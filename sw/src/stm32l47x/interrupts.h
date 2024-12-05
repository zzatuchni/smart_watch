#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void _on_hard_fault(void);

void _on_mem_fault(void);

void _on_bus_fault(void);

void _on_usage_fault(void);

#endif