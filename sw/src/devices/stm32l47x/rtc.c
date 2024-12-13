#include "rtc.h"

const char *weekday_strs[8] = {"NA", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
const uint8_t weekday_szs[8] = {2, 6, 7, 9, 8, 6, 8, 6};
const char *month_strs[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const uint8_t month_szs[12] = {7, 8, 5, 5, 3, 4, 4, 6, 9, 7, 8, 8};

Result rtc_init() {
    // enable peripheral
    RCC->APB1ENR1 |= BIT(PWR_APB1ENR1_BIT);
    RCC->APB1ENR1 |= BIT(RTC_APB1ENR1_BIT);

    // Configure LSE / RTC
    RCC->BDCR |= BIT(8); //RTCSEL = 0x1 = LSE

    RCC->BDCR |= BIT(0); //LSEON, LSE oscillator enable

    while (!(RCC->BDCR & BIT(1))) {}; //LSERDY, LSE ready

    /*
    RTC_DISABLE_WRITE_PROTECTION();

    RTC->ISR |= BIT(7); // INIT, enable initialization mode

    while (!(RTC->ISR & BIT(6))) {}; // INITF, initialization mode entered

    RTC->TR = 0x0;
    RTC->DR = 0x0;
    
    RTC->ISR &= ~BIT(7); // INIT, disable initialization mode

    RTC_ENABLE_WRITE_PROTECTION();
    */

    RCC->BDCR |= BIT(15); //RTCEN, RTC clock enable
    
    RTC->ISR &= ~BIT(5);
    while (!(RTC->ISR & BIT(5))) {}; // RSF, synchronization

    return RES_OK;
}

Result rtc_get_time(Time *time) {
    RTC->ISR &= ~BIT(5);
    while (!(RTC->ISR & BIT(5))) {}; // RSF, synchronization

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

Result rtc_set_time(Time *time) {
    RTC_DISABLE_WRITE_PROTECTION();

    RTC->ISR |= BIT(7); // INIT, enable initialization mode

    while (!(RTC->ISR & BIT(6))) {}; // INITF, initialization mode entered

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
    while (!(RTC->ISR & BIT(5))) {}; // RSF, synchronization

    return RES_OK;
}

size_t write_weekday_str(uint8_t wkday, char *buf) {
    size_t sz = (size_t)weekday_szs[wkday];
    str_copy(weekday_strs[wkday], buf, sz);
    return sz;
}

size_t write_month_str(uint8_t month, char *buf) {
    size_t sz = (size_t)month_szs[month];
    str_copy(month_strs[month], buf, sz);
    return sz;
}