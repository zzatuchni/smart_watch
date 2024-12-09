#include "rtc.h"

const char *weekday_strs[8] = {"NA", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
const uint8_t weekday_szs[8] = {2, 6, 7, 9, 8, 6, 8, 6};
const char *month_strs[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const uint8_t month_szs[12] = {7, 8, 5, 5, 3, 4, 4, 6, 9, 7, 8, 8};

Result rtc_init() {
    // enable peripheral
    RCC->APB1ENR1 |= BIT(PWR_APB1ENR1_BIT);
    RCC->APB1ENR1 |= BIT(RTC_APB1ENR1_BIT);

    PWR->CR[0] |= BIT(8); //DBP, Disable backup domain write protection

    // Configure LSE / RTC
    RCC->BDCR |= BIT(15); //RTCEN, RTC enable

    RCC->BDCR |= BIT(8); //RTCSEL = 0x1 = LSE

    RCC->BDCR |= BIT(0); //LSEON, LSE oscillator enable

    while (!(RCC->BDCR & BIT(1))) {}; //LSERDY, LSE ready

    // disable RTC write protection
    //RTC->WPR |= 0xCA;
    //RTC->WPR |= 0x53;

    //RTC->ISR |= BIT(7); // INIT, enable initialization mode
    //uart_write_value(USART2, RTC->ISR);
    //uart_write_byte(USART2, '1');
    //while (!(RTC->ISR & BIT(6))) {}; // INITF, initializatio mode entered
    //uart_write_byte(USART2, '2');

    //RTC->TR = 0x0;
    //RTC->DR = 0x0;

    //RTC->ISR &= ~BIT(7); // INIT, enable initializatio mode
    //spin(5000);

    RCC->BDCR |= BIT(15); //RTCEN, RTC clock enable

    return RES_OK;
}

Result rtc_get_time(Time *time) {
    time->year =    (uint8_t)(RTC_DR_YT * 10 + RTC_DR_YU);
    time->month =   (uint8_t)(RTC_DR_MT * 10 + RTC_DR_MU);
    time->day =     (uint8_t)(RTC_DR_DT * 10 + RTC_DR_DU);

    time->weekday = (uint8_t)RTC_DR_WD;

    time->hour =    (uint8_t)(RTC_TR_HT * 10 + RTC_TR_HU);
    time->minute =  (uint8_t)(RTC_TR_MT * 10 + RTC_TR_MU);
    time->second =  (uint8_t)(RTC_TR_ST * 10 + RTC_TR_SU);

    time->subsecond = (uint16_t)RTC->SSR;

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

size_t write_date_str(Time *time, char *buf) {
    size_t sz = 0;
    sz += write_weekday_str(time->weekday, buf);
    
    int_to_str(time->day, buf+sz, 2, 10);
    sz += 2;

    sz += write_month_str(time->month, buf+sz);

    int_to_str(time->year, buf+sz, 4, 10);
    sz += 4;

    return sz;
}