/**
 * @file    Timers.h
 * @brief   Timer/Counter driver interface for AVR64DD32
 * @details Implements TCB (Timer/Counter B) modules for PWM generation,
 *          frequency measurement, and periodic interrupts. Supports multiple
 *          timer modes including periodic interrupt and pulse width modulation.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Timer Channels
 *      - TCB0: General purpose timer
 *      - TCB1: PWM output on PF4 (TP6)
 *      - TCB2: Available for user configuration
 *
 * @par PWM Configuration
 *      - Resolution: 8-bit or 16-bit
 *      - Frequency: 61Hz to 4MHz (depending on prescaler)
 *      - Duty cycle: 0% to 100%
 *      - Output pin: PF4
 *
 * @par Features
 *      - Multiple timer modes (periodic, PWM, frequency measurement)
 *      - Configurable prescaler (DIV1, DIV2, DIV4, DIV8, DIV16)
 *      - Interrupt support for overflow and compare match
 *      - Synchronized timer start/stop
 *
 * @note    Timer frequency = F_CPU / (prescaler * (CCMP + 1))
 * @note    PWM duty cycle = CMP / CCMP * 100%
 * @warning Ensure prescaler setting doesn't exceed timer count capabilities
 *
 * @see Menu_Timers.h for configuration interface
 */
#pragma once

#include "Common/Common.h"

#define TimersSystemSize 1
#define PF4_TimerOutputPin 0x10

enum DUTYCYCLELIST{
	DUTYCYCLELIST_10  = 10,
	DUTYCYCLELIST_20  = 20,
	DUTYCYCLELIST_30  = 30,
	DUTYCYCLELIST_40  = 40,
	DUTYCYCLELIST_50  = 50,
	DUTYCYCLELIST_60  = 60,
	DUTYCYCLELIST_70  = 70,
	DUTYCYCLELIST_80  = 80,
	DUTYCYCLELIST_90 = 90
};


typedef struct TimersConfiguration
{
	enum SIGNATURE_BYTES signature;
	bool enabled;
	uint8_t clksel;
	uint8_t waveform;
	uint16_t period;
	uint16_t compare;
	uint8_t dutyCycle;
} TimersConfiguration;

void Timers_Display(void *ptrData, const char *msg);
void Timers_Initialize(void);
void Timers_Monitor(void);
void *GetPtrToTimersData(void);
void Timers_Save_Config(uint8_t channel);
void Timers_Load_Config(void);
void Timers_ReInitialize(void);
void Program_Timer(void);

void GetTimersPeriod(const char *msg);
void GetTimersCompare(const char *msg);
uint16_t ComputeDutyCycle(void);