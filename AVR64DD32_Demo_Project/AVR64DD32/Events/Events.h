/**
 * @file    Events.h
 * @brief   Event System driver for AVR64DD32
 * @details Implements the Event System (EVSYS) for hardware-based peripheral
 *          interconnection without CPU intervention. Allows direct routing of
 *          events between peripherals (timers, ADC, CCL, ports, etc.).
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Event System Capabilities
 *      - 10 event channels (0-9)
 *      - Synchronous and asynchronous events
 *      - Direct peripheral-to-peripheral connection
 *      - Software event generation
 *      - Port pin event inputs
 *
 * @par Common Event Routing Examples
 *      - Timer overflow ? ADC start conversion
 *      - Port pin change ? CCL input
 *      - RTC overflow ? Event output pin
 *      - CCL output ? Timer capture input
 *
 * @par Event Generators (Examples)
 *      - Timer/Counter overflow or compare match
 *      - ADC conversion complete
 *      - RTC compare or overflow
 *      - Port pins (EVOUT)
 *      - CCL LUT outputs
 *
 * @par Event Users (Examples)
 *      - ADC start conversion
 *      - Timer capture/count
 *      - CCL inputs
 *      - EVOUT pins
 *
 * @note    Events propagate in hardware - no CPU overhead
 * @note    Asynchronous events work even when CPU is in sleep mode
 * @warning Ensure event generators and users are on compatible channels
 * @warning Some events are synchronous - verify timing requirements
 *
 * @see AVR64DD32 datasheet section 16 (Event System)
 * @see Menu_EVENTS.h for configuration interface
 */
#pragma once
#include "Common/Common.h"

#define EventsSystemSize 1
#define EVOUT_Pin	0x80

enum EVENTMODES
{
	EVENTMODES_ADC,
	EVENTMODES_CCL,
	EVENTMODES_RTC,
	EVENTMODES_ZCD
};

typedef struct EventsConfiguration
{
	enum SIGNATURE_BYTES signature;
	bool enabled;
	enum EVENTMODES mode;
} EventsConfiguration;

void Events_Display(void *ptrData, const char *msg);
void Events_Initialize(void);
void Events_Monitor(void);
void *GetPtrToEventsData(void);
void Events_Save_Config(uint8_t channel);
void Events_Load_Config(void);
void Events_ReInitialize(void);