/**
 * @file    Common.h
 * @brief   Common definitions and utility functions for AVR64DD32 project
 * @details Provides project-wide includes, type definitions, and utility
 *          functions. Acts as central header for shared functionality across
 *          all modules.
 *
 * @author  Terry (ABS Data Systems)
 * @date    Created: 11/27/2025 6:57:11 AM
 * @version 1.0
 *
 * @par Includes
 *      - Standard AVR libraries (avr/io.h, util/delay.h)
 *      - Standard C libraries (stdio.h, string.h, stdbool.h)
 *      - PROGMEM support (avr/pgmspace.h)
 *      - Project-specific headers
 *
 * @par Utility Functions
 *      - Decoder functions: Convert enums to human-readable strings
 *      - Reference voltage lookup tables
 *      - Common data conversions
 *
 * @note    Include this file in all module implementations
 * @see DataStructures.h for type definitions
 */

#pragma once

#include <atmel_start.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "CommandList/CommandList.h"
#include "USART/usart_basic.h"
#include "SerialComm/SerialComm_Receive.h"
#include "Menu_Report_Processor/Terminal_Commands/Terminal_Commands.h"
#include "Menus/Main_Menu/Main_Menu.h"

enum REFVOLTAGE
{
	Range_1024,
	Range_2048,
	Range_2500,
	Range_4096,
	Range_VDD,
	Range_VREF
};



typedef struct RefVoltage_Range
{
	uint8_t RegisterSetting;
	enum REFVOLTAGE refVoltage;	
	float MaxRange;
}RefVoltage_Range;

// Generic
char *EnableDisableDecode(void *ptr);
char *PortDirectionDecode(void *ptr);
char *PortInvertedDecode(void *ptr);
char *PortResistorDecode(void *ptr);
char *HighLowDecode(void *ptr);
char *PortInputLevelDecode(void *ptr);
char *RefDecode(void *ptr);

struct RefVoltage_Range LoopUp_RefVoltage(enum REFVOLTAGE value);
float LookUp_RefVoltageBySetting(uint8_t value);