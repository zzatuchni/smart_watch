#ifndef RTC_H
#define RTC_H

#include "basic.h"
#include "uart.h"

#define RTC_REGS_START_ADDRESS 0x40002800

#define RTC_APB1ENR1_BIT 10

typedef struct {
    volatile uint32_t TR, DR, CR, ISR, PRER, WUTR, ALRMAR, ALRMBR, WPR, SSR, SHIFTR,
        TSTR, TSDR, TSSSR, CALR, TAMPCR, ALRMASSR, ALRMBSSR, OR, BKPxR[19];
} RTC_Regs;

#define RTC ((RTC_Regs *)(RTC_REGS_START_ADDRESS))

#define RTC_TR_HT ((RTC->TR & (BIT(20) | BIT(21))) >> 20)
#define RTC_TR_HU ((RTC->TR & (BIT(16) | BIT(17) | BIT(18) | BIT(19))) >> 16)
#define RTC_TR_MT ((RTC->TR & (BIT(12) | BIT(13) | BIT(14))) >> 12)
#define RTC_TR_MU ((RTC->TR & (BIT(8) | BIT(9) | BIT(10) | BIT(11))) >> 8)
#define RTC_TR_ST ((RTC->TR & (BIT(4) | BIT(5) | BIT(6))) >> 4)
#define RTC_TR_SU ((RTC->TR & (BIT(0) | BIT(1) | BIT(2) | BIT(3))) >> 0)

#define RTC_DR_YT ((RTC->DR & (BIT(20) | BIT(21) | BIT(22) | BIT(23))) >> 20)
#define RTC_DR_YU ((RTC->DR & (BIT(16) | BIT(17) | BIT(18) | BIT(19))) >> 16)
#define RTC_DR_WD ((RTC->DR & (BIT(13) | BIT(14) | BIT(15))) >> 13)
#define RTC_DR_MT ((RTC->DR & (BIT(12)) >> 12))
#define RTC_DR_MU ((RTC->DR & (BIT(8) | BIT(9) | BIT(10) | BIT(11))) >> 8)
#define RTC_DR_DT ((RTC->DR & (BIT(4) | BIT(5))) >> 4)
#define RTC_DR_DU ((RTC->DR & (BIT(0) | BIT(1) | BIT(2) | BIT(3))) >> 0)

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t weekday;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t subsecond;
} Time;

extern const char *weekday_strs[8];
extern const uint8_t weekday_szs[8];
extern const char *month_strs[12];
extern const uint8_t month_szs[12];

Result rtc_init();

Result rtc_get_time(Time *time);

size_t write_weekday_str(uint8_t wkday, char *buf);

size_t write_month_str(uint8_t month, char *buf);

size_t write_date_str(Time *time, char *buf);

#endif
