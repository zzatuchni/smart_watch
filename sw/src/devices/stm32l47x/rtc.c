#include "rtc.h"

Result rtc_init() {
    // enable peripheral
    RCC->APB1ENR1 |= BIT(PWR_APB1ENR1_BIT);
    RCC->APB1ENR1 |= BIT(RTC_APB1ENR1_BIT);

    RTC_DISABLE_WRITE_PROTECTION();

    // Configure LSE / RTC
    RCC->BDCR |= BIT(8); //RTCSEL = 0x1 = LSE

    RCC->BDCR |= BIT(0); //LSEON, LSE oscillator enable

    WAIT_FOR_CONDITION((RCC->BDCR & BIT(1)), GENERIC_TIMEOUT_NUM); //LSERDY, LSE ready

    /*
    RTC->ISR |= BIT(7); // INIT, enable initialization mode

    WAIT_FOR_CONDITION((RTC->ISR & BIT(6)), GENERIC_TIMEOUT_NUM); // INITF, initialization mode entered

    RTC->TR = 0x0;
    RTC->DR = 0x0;
    
    RTC->ISR &= ~BIT(7); // INIT, disable initialization mode
    */

    RCC->BDCR |= BIT(15); //RTCEN, RTC clock enable
    
    RTC->ISR &= ~BIT(5);
    WAIT_FOR_CONDITION((RTC->ISR & BIT(5)), GENERIC_TIMEOUT_NUM); // RSF, synchronization

    RTC_ENABLE_WRITE_PROTECTION();

    return RES_OK;
}

Result rtc_get_time(RTC_Time *time) {
    RTC->ISR &= ~BIT(5);
    WAIT_FOR_CONDITION((RTC->ISR & BIT(5)), GENERIC_TIMEOUT_NUM); // RSF, synchronization

    time->hour =    (uint8_t)(RTC_TR_HT * 10 + RTC_TR_HU);
    time->minute =  (uint8_t)(RTC_TR_MT * 10 + RTC_TR_MU);
    time->second =  (uint8_t)(RTC_TR_ST * 10 + RTC_TR_SU);

    time->subsecond = (uint16_t)RTC->SSR;

    time->year =    (uint8_t)(RTC_DR_YT * 10 + RTC_DR_YU);
    time->month =   (uint8_t)(RTC_DR_MT * 10 + RTC_DR_MU);
    time->day =     (uint8_t)(RTC_DR_DT * 10 + RTC_DR_DU);

    time->weekday = (uint8_t)RTC_DR_WD;

    return RES_OK;
}

Result rtc_set_time(RTC_Time *time) {
    RTC_DISABLE_WRITE_PROTECTION();

    RTC->ISR |= BIT(7); // INIT, enable initialization mode

    WAIT_FOR_CONDITION((RTC->ISR & BIT(6)), GENERIC_TIMEOUT_NUM); // INITF, initialization mode entered

    RTC_SET_TR_H(time->hour);
    RTC_SET_TR_M(time->minute);
    RTC_SET_TR_S(time->second);

    RTC_SET_DR_Y(time->year);
    RTC_SET_DR_WD(time->weekday);
    RTC_SET_DR_M(time->month);
    RTC_SET_DR_D(time->day);
    
    RTC->ISR &= ~BIT(7); // INIT, disable initialization mode

    RTC_ENABLE_WRITE_PROTECTION();

    RTC->ISR &= ~BIT(5);
    WAIT_FOR_CONDITION((RTC->ISR & BIT(5)), GENERIC_TIMEOUT_NUM); // RSF, synchronization

    return RES_OK;
}