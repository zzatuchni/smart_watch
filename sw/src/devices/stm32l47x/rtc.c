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

    RTC->ISR |= BIT(7); // INIT, enable initialization mode

    WAIT_FOR_CONDITION((RTC->ISR & BIT(6)), GENERIC_TIMEOUT_NUM); // INITF, initialization mode entered

    RTC->TR = RTC->BKPxR[0];
    RTC->DR = RTC->BKPxR[1];
    RTC->SSR = RTC->BKPxR[2];
    
    RTC->ISR &= ~BIT(7); // INIT, disable initialization mode

    RCC->BDCR |= BIT(15); //RTCEN, RTC clock enable
    
    RTC->ISR &= ~BIT(5);
    WAIT_FOR_CONDITION((RTC->ISR & BIT(5)), GENERIC_TIMEOUT_NUM); // RSF, synchronization

    RTC_ENABLE_WRITE_PROTECTION();

    return RES_OK;
}

Result rtc_get_time(RTC_Time *time) {
    RTC->ISR &= ~BIT(5);
    WAIT_FOR_CONDITION((RTC->ISR & BIT(5)), GENERIC_TIMEOUT_NUM); // RSF, synchronization

    uint32_t ssr = RTC->SSR;
    uint32_t tr = RTC->TR;
    uint32_t dr = RTC->DR;

    time->hour =    (uint8_t)(RTC_TR_HT(tr) * 10 + RTC_TR_HU(tr));
    time->minute =  (uint8_t)(RTC_TR_MT(tr) * 10 + RTC_TR_MU(tr));
    time->second =  (uint8_t)(RTC_TR_ST(tr) * 10 + RTC_TR_SU(tr));

    time->subsecond = (uint16_t)ssr;

    time->year =    (uint8_t)(RTC_DR_YT(dr) * 10 + RTC_DR_YU(dr));
    time->month =   (uint8_t)(RTC_DR_MT(dr) * 10 + RTC_DR_MU(dr));
    time->day =     (uint8_t)(RTC_DR_DT(dr) * 10 + RTC_DR_DU(dr));
    time->weekday = (uint8_t)RTC_DR_WD(dr);

    RTC_DISABLE_WRITE_PROTECTION();

    RTC->BKPxR[0] = tr;
    RTC->BKPxR[1] = dr;
    RTC->BKPxR[2] = ssr;

    RTC_ENABLE_WRITE_PROTECTION();

    return RES_OK;
}

Result rtc_set_time(RTC_Time *time) {
    RTC_DISABLE_WRITE_PROTECTION();

    RTC->ISR |= BIT(7); // INIT, enable initialization mode

    WAIT_FOR_CONDITION((RTC->ISR & BIT(6)), GENERIC_TIMEOUT_NUM); // INITF, initialization mode entered

    RTC->TR = 
        RTC_SET_TR_H(time->hour) |
        RTC_SET_TR_M(time->minute) |
        RTC_SET_TR_S(time->second);

    RTC->DR =
        RTC_SET_DR_Y(time->year) |
        RTC_SET_DR_WD(time->weekday) |
        RTC_SET_DR_M(time->month) |
        RTC_SET_DR_D(time->day);
    
    RTC->ISR &= ~BIT(7); // INIT, disable initialization mode

    RTC_ENABLE_WRITE_PROTECTION();

    RTC->ISR &= ~BIT(5);
    WAIT_FOR_CONDITION((RTC->ISR & BIT(5)), GENERIC_TIMEOUT_NUM); // RSF, synchronization

    return RES_OK;
}