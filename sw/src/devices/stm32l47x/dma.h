#ifndef DMA_H
#define DMA_H

#include "basic.h"

/*
typedef struct {
    volatile uint32_t ISR, IFCR;
} DMA_Interrupt_Regs;

#define DMA_INTERRUPT_REGS_SIZE 0x8

typedef struct {
    volatile uint32_t CCR, CNDTR, CPAR, CMAR;
} DMA_Stream_Regs;

#define DMA_STREAM_REGS_SIZE 0x18

#define DMA1_REGS_START_ADDRESS 0x40026000

#define DMA1_STREAM(x) (DMA_Stream_Regs *)((DMA1_START_ADDRESS) + (DMA_INTERRUPT_REGS_SIZE) + ((x) * (DMA_STREAM_REGS_SIZE)))
#define DMA2_STREAM(x) (DMA_Stream_Regs *)((DMA2_START_ADDRESS) + (DMA_INTERRUPT_REGS_SIZE) + ((x) * (DMA_STREAM_REGS_SIZE)))

typedef enum { 
    DMA_PFRL_TO_MEM, DMA_MEM_TO_PFRL, DMA_MEM_TO_MEM 
} DMA_Direction;

static inline void dma_init(uint8_t channel, uint8_t stream, DMA_Direction direction, void *src, void *dest, uint8_t mem_width, uint32_t num_transfers) {
    // We want channel 3 stream 3
    DMA_Stream_Regs *DMA = DMA2_STREAM(stream);

    // Enable DMA2
    RCC->AHB1ENR |= BIT(22); //DMA2

    // Set channel
    DMA->SxCR |= (channel << 25);

    // Priority high
    DMA->SxCR |= (0x2U << 16);

    // Circular mode
    DMA->SxCR |= BIT(8);

    // Memory size
    DMA->SxCR |= ((mem_width-1U) << 13);
    DMA->SxCR |= ((mem_width-1U) << 11);

    // Transfer x items
    DMA->SxNDTR |= num_transfers;

    // Mem pointer increment
    DMA->SxCR |= BIT(10);

    // Set direction 
    DMA->SxCR |= (direction << 6);

    // Set source and address
    if (direction == DMA_PFRL_TO_MEM) {
        DMA->SxM0AR = (uint32_t)(dest);
        DMA->SxPAR = (uint32_t)(src);
    }
    else if (direction == DMA_MEM_TO_PFRL) {
        DMA->SxM0AR = (uint32_t)(src);
        DMA->SxPAR = (uint32_t)(dest);
    }
    else if (direction == DMA_MEM_TO_MEM) {
        DMA->SxM0AR = (uint32_t)(dest);
        DMA->SxPAR = (uint32_t)(src);
    }
}

static inline void dma_start(uint8_t stream) {
    (DMA2_STREAM(stream))->SxCR |= BIT(0);
}
*/

#endif