/**
 * @file    Display_Processor.h
 * @brief   Real-time data display processor for AVR64DD32 system
 * @details Manages automatic display of peripheral data in the terminal's
 *          right-side data area (columns 85-120). Supports registration of
 *          display items with custom formatting callbacks.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Display Architecture
 *      Each peripheral registers display items containing:
 *      - Pointer to data structure
 *      - Custom formatting function
 *      - Display message/label
 *      - Enable/disable flag
 *
 * @par Display Area
 *      - Location: Columns 85-120, rows 1-30
 *      - Update: Automatic on each main loop iteration
 *      - Format: Module-specific (ADC voltages, Timer frequencies, etc.)
 *
 * @par Features
 *      - Dynamic display item registration
 *      - Individual item enable/disable
 *      - Custom formatting via function pointers
 *      - Automatic screen area clearing
 *
 * @note    Display updates do not block main loop
 * @note    Maximum number of display items configurable
 * @warning Excessive display items may impact refresh rate
 *
 * @see DisplayProcessorMap structure
 * @see Terminal_Commands.h for positioning
 */

#pragma once

#include "Common/DataStructures.h"
// For now this is just a place holder number.
// The future plan is to make this dynamic.

#define MAXNODISPLAYPROCESSORS 15
#define DISPLAYPROCESSOR_DISPLAYCOL		85
enum DISPLAYPROCESSORSTATES {
	DISPLAYPROCESSORSTATES_PRINT,
	DISPLAYPROCESSORSTATES_LOOP
};

void DisplayProcessor_Initialize(void);
void DisplayProcessor_Monitor(void);
void DisplayProcessor_Add(struct DisplayProcessorMap item);
void DisplayProcessor_SetEnabledFlag(void *functPtr, bool enabled);