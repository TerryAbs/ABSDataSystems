/**
 * @file    RTC.h
 * @brief   Real-Time Counter driver for AVR64DD32
 * @details Implements RTC using internal 32.768kHz oscillator for accurate
 *          timekeeping. Supports periodic interrupts, compare match, and
 *          overflow detection. Useful for timekeeping and periodic tasks.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Clock Source
 *      - Internal 32.768kHz RC oscillator (OSCULP32K)
 *      - External 32.768kHz crystal (optional)
 *      - Prescaler: 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024
 *
 * @par Features
 *      - 16-bit counter (0-65535)
 *      - Compare match interrupt
 *      - Overflow interrupt
 *      - Periodic interrupt mode
 *      - Low power operation
 *
 * @par Typical Applications
 *      - Software timekeeping
 *      - Periodic task scheduling
 *      - Timeout generation
 *      - Event timestamping
 *
 * @note    RTC continues to run in sleep modes (except standby)
 * @note    Prescaler value affects interrupt frequency
 * @warning Internal oscillator accuracy is ±3% over temperature
 *
 * @see Menu_RTC.h for configuration interface
 */


#pragma once
#include "Common/Common.h"

#define RTCSystemSize 1
#define RTCDisplayBuffer 32

// RTC time structure
typedef struct rtc_time_t{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint16_t year;
} rtc_time_t;

typedef struct RTCConfiguration
{
	enum SIGNATURE_BYTES signature;
	bool enabled;
} RTCConfiguration;

void RTC_Display(void *ptrData, const char *msg);
void RTC_Initialize(void);
void RTC_Monitor(void);
void *GetPtrToRTCData(void);
void RTC_Save_Config(uint8_t channel);
void RTC_Load_Config(void);
void RTC_Set_Time(uint8_t hour, uint8_t minute, uint8_t second);
void RTC_Set_Date(uint16_t year, uint8_t month, uint8_t day);
void RTC_Increment_Time(void);
bool Is_Leap_Year(uint16_t year);
uint8_t Days_In_Month(uint8_t month, uint16_t year);
void FormatTime(char *buffer, const rtc_time_t *time);
bool GetRTCEnabledState(void);