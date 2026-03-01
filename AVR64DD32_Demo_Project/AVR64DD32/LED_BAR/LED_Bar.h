/**
 * @file    LED_Bar.h
 * @brief   LED bar graph driver (legacy - pins reassigned)
 * @details Originally controlled an LED bar graph for visual indication.
 *          Now deprecated as pins have been reassigned to other peripheral
 *          functions in the current hardware configuration.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @deprecated Pins previously used for LED bar now used for:
 *             - SPI signals
 *             - Event system outputs
 *             - CCL test points
 *
 * @note    Code retained for reference but not compiled/initialized
 * @warning Do not enable - will conflict with current pin assignments
 * @see main.c line 90 for initialization comment
 */

#pragma once

#include "Common/Common.h"

#define PORT_A_0	0x01
#define PORT_A_1	0x02
#define PORT_A_2	0x04
#define PORT_A_3	0x08	
#define PORT_A_4	0x10
#define PORT_A_5	0x20
#define PORT_A_6	0x40
#define PORT_A_7	0x80

#define LED_MASK (PORT_A_7 | PORT_A_6 | PORT_A_5 | PORT_A_4 | PORT_A_3 | PORT_A_2 | PORT_A_1 | PORT_A_0)     // LEDs on PORTA[7:0]
#define Max_BarSize 8
void LED_Bar_Initialize(void);
void LED_Bar_Display(float value);
void LED_Bar_SetRange(float value);