#ifndef RTC_H
#define RTC_H

#include "basic.h"
#include "uart.h"

#define RTC_REGS_START_ADDRESS 0x40002800

#define RTC_APB1ENR1_BIT 10

typedef struct {
    volatile uint32_t TR, DR, CR, ISR, PRER, WUTR, RESERVED, ALRMAR, ALRMBR, WPR, SSR, SHIFTR,
        TSTR, TSDR, TSSSR, CALR, TAMPCR, ALRMASSR, ALRMBSSR, OR, BKPxR[19];
} RTC_Regs;

#define RTC ((RTC_Regs *)(RTC_REGS_START_ADDRESS))

#define RTC_TR_HT(tr) (((tr) & (BIT(20) | BIT(21))) >> 20)
#define RTC_TR_HU(tr) (((tr) & (BIT(16) | BIT(17) | BIT(18) | BIT(19))) >> 16)
#define RTC_TR_MT(tr) (((tr) & (BIT(12) | BIT(13) | BIT(14))) >> 12)
#define RTC_TR_MU(tr) (((tr) & (BIT(8) | BIT(9) | BIT(10) | BIT(11))) >> 8)
#define RTC_TR_ST(tr) (((tr) & (BIT(4) | BIT(5) | BIT(6))) >> 4)
#define RTC_TR_SU(tr) (((tr) & (BIT(0) | BIT(1) | BIT(2) | BIT(3))) >> 0)

#define RTC_SET_TR_H(x) ((((uint32_t)x) / 10) << 20) | ((((uint32_t)x) % 10) << 16)
#define RTC_SET_TR_M(x) ((((uint32_t)x) / 10) << 12) | ((((uint32_t)x) % 10) << 8)
#define RTC_SET_TR_S(x) ((((uint32_t)x) / 10) << 4) | ((((uint32_t)x) % 10) << 0)

#define RTC_DR_YT(dr) (((dr) & (BIT(20) | BIT(21) | BIT(22) | BIT(23))) >> 20)
#define RTC_DR_YU(dr) (((dr) & (BIT(16) | BIT(17) | BIT(18) | BIT(19))) >> 16)
#define RTC_DR_WD(dr) (((dr) & (BIT(13) | BIT(14) | BIT(15))) >> 13)
#define RTC_DR_MT(dr) (((dr) & (BIT(12)) >> 12))
#define RTC_DR_MU(dr) (((dr) & (BIT(8) | BIT(9) | BIT(10) | BIT(11))) >> 8)
#define RTC_DR_DT(dr) (((dr) & (BIT(4) | BIT(5))) >> 4)
#define RTC_DR_DU(dr) (((dr) & (BIT(0) | BIT(1) | BIT(2) | BIT(3))) >> 0)

#define RTC_SET_DR_Y(x) ((((uint32_t)x) / 10) << 20) | ((((uint32_t)x) % 10) << 16)
#define RTC_SET_DR_WD(x) (((uint32_t)x) << 13)
#define RTC_SET_DR_M(x) ((((uint32_t)x) / 10) << 12) | ((((uint32_t)x) % 10) << 8)
#define RTC_SET_DR_D(x) ((((uint32_t)x) / 10) << 4) | ((((uint32_t)x) % 10) << 0)

// DPB bit in PWR, write code in WPR
#define RTC_DISABLE_WRITE_PROTECTION() { PWR->CR[0] |= BIT(8); RTC->WPR = 0xCA; RTC->WPR = 0x53; }
#define RTC_ENABLE_WRITE_PROTECTION() { PWR->CR[0] &= ~BIT(8); RTC->WPR = 0xFF; }

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t weekday;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t subsecond;
} RTC_Time;

Result rtc_init();

Result rtc_get_time(RTC_Time *time);

Result rtc_set_time(RTC_Time *time);

#endif
