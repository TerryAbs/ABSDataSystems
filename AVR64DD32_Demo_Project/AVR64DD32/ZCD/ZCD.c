/*
 * ZCD.c
 *
 * Created: 1/2/2026 10:59:05 AM
 *  Author: terry
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include "Delay/Delay.h"
#include "ZCD/ZCD.h"
#include "EEPRom/EEPRom.h"
#include "CommonStrings/CommonStrings.h"
#include "Display_Processor/Display_Processor.h"

volatile uint32_t positive_crossings = 0;
volatile uint32_t negative_crossings = 0;
static const char *msg_ZCD_EdgeMessage;
static uint16_t ZCDLoopCounter = 0;

ZCDConfiguration ZCDSystem = {
	SIGNATURE_BYTES_ZCD,
	true,							// Enabled/Disabled
	ZCD_INTMODE_BOTH_gc,				// Edge 
	100
};

// These pointers are not need for this system.
void ZCD_Display(void *ptrData, const char *msg)
{
	uint32_t last_pos = 0;
	uint32_t last_neg = 0;
	if(ZCDSystem.enabled)
	{
		cli();
		last_pos = positive_crossings;
		last_neg = negative_crossings;
		sei();
		
		SetColor(Color_White, Color_Black);
		printf_P(PSTR("ZCD %S Pos: %lu, Neg: %lu"), msg_ZCD_EdgeMessage, last_pos, last_neg);
	}
}

void ZCD_ReInitialize(void)
{
	ZCD_Load_Config();

    /*
     * Configure Zero Cross Detector (ZCD3)
     * Input: PC2 (ZCD3 pin)
     * Detect both positive and negative crossings
     */
    
    // Set PC2 as input (ZCD input pin)
    PORTC.DIRCLR = PC2_ZCD_PortPin;
    PORTC.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;  // Disable digital input buffer
    
	PORTF.DIRSET = PF5_SimulationPin;
	PORTF.OUTCLR = PF5_SimulationPin;
	
    // Configure ZCD3
    // - Enable ZCD
    // - Output to event system (optional)
    // - Interrupt on both edges

	ZCDLoopCounter = 0;
	
	cli();
	positive_crossings = 0;
	negative_crossings = 0;
	sei();
    
	if(ZCDSystem.enabled)
	{
		ZCD3.CTRLA = ZCD_ENABLE_bm;     // Enable ZCD
	}
	else
	{
		ZCD3.CTRLA = 0;					// Disable ZCD		
	}
   
    // Configure interrupt on state change
    ZCD3.INTCTRL = ZCDSystem.edge;  // Interrupt on edge(s)
	msg_ZCD_EdgeMessage = DecodeInterruptEdge(&ZCDSystem.edge);
}

void ZCD_Initialize(void)
{
	ZCD_ReInitialize();

	// Add item to display list.
	DisplayProcessorMap item;
	item.ptrData = &ZCDSystem;
	item.funcptrPrint = ZCD_Display;
	item.msg = NULL;
	item.enabled = ZCDSystem.enabled;
	DisplayProcessor_Add(item);
}

void ZCD_Monitor(void)
{
	if(ZCDSystem.enabled)
	{
		if(ZCDLoopCounter >= ZCDSystem.loopDelay)
		{
			PORTF.OUTTGL = PF5_SimulationPin;
			ZCDLoopCounter = 0;
			MicroDelay(1);
			PORTF.OUTTGL = PF5_SimulationPin;
		}
		else
		{
			ZCDLoopCounter++;
		}
	}
}


ISR(ZCD3_ZCD_vect)
{
	// Read the ZCD status to determine crossing direction
	if (ZCD3.STATUS & ZCD_STATE_bm) {
		// Positive crossing (low to high)
		positive_crossings++;
		} else {
		// Negative crossing (high to low)
		negative_crossings++;
	}
	
	// Clear interrupt flag
	ZCD3.STATUS = ZCD_CROSSIF_bm;
}

void *GetPtrToZCDData(void)
{
	return &ZCDSystem;
}

bool GetZCDEnabledState(void)
{
	return ZCDSystem.enabled;
}

void ZCD_Save_Config(uint8_t channel)
{
	// Save just one element
	// However, I'm maintaining the option of multiple channels. Like i did for ADC
	EEPROM_WRITE_ELEMENT(zcd_system, 0, &ZCDSystem);
	DisplayProcessor_SetEnabledFlag(&ZCDSystem, ZCDSystem.enabled);	
	ZCD_ReInitialize();
}

void ZCD_Load_Config(void)
{
	// Load just one element
	EEPROM_VERIFY_SIGNATURE_AND_LOAD(zcd_system, 0, &ZCDSystem);
}

void GetLoopDelay(const char *msg)
{
	struct EnterValueByHand *enterValue = GetEnterValuePtr();
	enterValue->fieldtype = FIELDTYPE_UINT8;
	enterValue->Min = 1.0;
	enterValue->Max = 10000.0;
	enterValue->msg = msg_LoopDelay;
	enterValue->ptrSaveFunction = SetLoopDelay;
	EnterNumberValue();
}

void SetLoopDelay(float value)
{
	ZCDSystem.loopDelay =  (uint16_t) value;
	ZCD_Save_Config(0);
}

char *DecodeInterruptEdge(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupInterruptEdgeByValue(value);
}
