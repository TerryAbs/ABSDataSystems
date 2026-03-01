/**
 * @file    CommonStrings.h
 * @brief   Common string constants for AVR64DD32 project
 * @details Provides centralized storage of frequently used strings in PROGMEM.
 *          Reduces code duplication and saves RAM by storing strings in flash.
 *          Includes labels, prompts, error messages, and format strings.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par String Categories
 *      - Display labels (INSTANT, AVERAGE, DIFFERENTIAL, etc.)
 *      - User prompts ("Enter value:", "Select option:", etc.)
 *      - Error messages ("Invalid input", "Out of range", etc.)
 *      - Success messages ("Configuration saved", "Operation complete")
 *      - Unit strings ("V", "Hz", "mA", etc.)
 *
 * @par Usage Pattern
 *      @code
 *      // In header:
 *      extern const char msg_INSTANT[] PROGMEM;
 *      
 *      // In source:
 *      const char msg_INSTANT[] PROGMEM = "INSTANT";
 *      
 *      // In code:
 *      printf_P(PSTR("ADC %S"), msg_INSTANT);
 *      @endcode
 *
 * @note    All strings stored in PROGMEM (flash) to save RAM
 * @note    Access via printf_P() with %S format specifier
 * @see CommonMsgs_list.h for complete string list
 */

#pragma once
#include <stdint.h>
#include <avr/pgmspace.h>

#define makeStringsWithSize(name, text) \
extern const char name[] PROGMEM;              \
enum { name##_LEN = (uint8_t)(sizeof(text) - 1) };

#include "CommonMsgs_list.h"
#undef makeStringsWithSize

