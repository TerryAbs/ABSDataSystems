/*
 * Events.c
 *
 * Created: 1/2/2026 10:55:57 AM
 *  Author: terry
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include "Events/Events.h"
#include "EEPRom/EEPRom.h"
#include "CommonStrings/CommonStrings.h"
#include "Display_Processor/Display_Processor.h"


EventsConfiguration EventsSystem = {
	SIGNATURE_BYTES_EVENTS,
	true,							// Enabled/Disabled
	EVENTMODES_RTC
};

static const char *msg_EventsMode;
// These pointers are not need for this system.
void Events_Display(void *ptrData, const char *msg)
{
	if(EventsSystem.enabled)
	{
		SetColor(Color_White, Color_Black);
		printf_P(PSTR("Events: %S"), msg_EventsMode);
	}
}

void Events_ReInitialize(void)
{
	Events_Load_Config();
	
	switch(EventsSystem.mode)	
	{
		case EVENTMODES_ADC:
			EVSYS.CHANNEL0 = EVSYS_CHANNEL0_ADC0_RESRDY_gc;
			msg_EventsMode = msg_EventModeADC;
			if(!GetADCEnabledState());
			{
				LogErrorToScreen(msg_EventModeADCError);
			}
			break;
		case EVENTMODES_CCL:
			EVSYS.CHANNEL0 = EVSYS_CHANNEL0_CCL_LUT3_gc;
			msg_EventsMode = msg_EventModeCCL;
			if(!GetCCLEnabledState())
			{
				LogErrorToScreen(msg_EventModeCCLCError);
			}
			break;			
		case EVENTMODES_RTC:
			EVSYS.CHANNEL0 = EVSYS_CHANNEL0_RTC_OVF_gc;
			msg_EventsMode = msg_EventModeRTC;
			if(!GetRTCEnabledState())
			{
				LogErrorToScreen(msg_EventModeRTCCError);
			}
			break;
		case EVENTMODES_ZCD:
			EVSYS.CHANNEL0 = EVSYS_CHANNEL0_ZCD3_gc;
			msg_EventsMode = msg_EventModeZCD;
			if(!GetZCDEnabledState());
			{
				LogErrorToScreen(msg_EventModeZCDCError);
			}
			break;
	}
	
	EVSYS.USEREVSYSEVOUTD = EVSYS_USER_CHANNEL0_gc;
	PORTMUX.EVSYSROUTEA = PORTMUX_EVOUTD_ALT1_gc;
}

void Events_Initialize(void)
{
	
	PORTD.DIRSET = EVOUT_Pin;
	PORTD.OUTCLR = EVOUT_Pin;
	
	Events_ReInitialize();
		
	// Add item to display list.
	DisplayProcessorMap item;
	item.ptrData = &EventsSystem;;
	item.funcptrPrint = Events_Display;
	item.msg = NULL;
	item.enabled = EventsSystem.enabled;
	DisplayProcessor_Add(item);
}

void Events_Monitor(void)
{
	if(EventsSystem.enabled)
	{
	}
}



void *GetPtrToEventsData(void)
{
	return &EventsSystem;
}

void Events_Save_Config(uint8_t channel)
{
	// Save just one element
	// However, I'm maintaining the option of multiple channels. Like i did for ADC
	EEPROM_WRITE_ELEMENT(events_system, 0, &EventsSystem);
	DisplayProcessor_SetEnabledFlag(&EventsSystem, EventsSystem.enabled);
	Events_ReInitialize();
}

void Events_Load_Config(void)
{
	// Load just one element
	EEPROM_VERIFY_SIGNATURE_AND_LOAD(events_system, 0, &EventsSystem);
}

