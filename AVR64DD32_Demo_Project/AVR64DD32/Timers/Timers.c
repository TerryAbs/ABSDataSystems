/*
 * Timers.c
 *
 * Created: 1/2/2026 10:57:50 AM
 *  Author: terry
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include "Timers/Timers.h"
#include "EEPRom/EEPRom.h"
#include "CommonStrings/CommonStrings.h"
#include "Display_Processor/Display_Processor.h"
#include "CommonMenus/CommonMenus.h"



/* Note: 
	If the timer values are set to fast, then the 
	SPI will fail. I think this is because the Timer system is 
	taking up too much of the CPU interrupts.'
*/


TimersConfiguration TimersSystem = {
	SIGNATURE_BYTES_TIMERS,
	true,							// Enabled/Disabled
	TCA_SINGLE_CLKSEL_DIV1_gc,
	TCA_SINGLE_WGMODE_NORMAL_gc,
	30,								// Period
	1,								// Compare
	DUTYCYCLELIST_50
};

static const char* msg_Config_Waveform;
static const char* msg_Config_CklSel;

void Timers_Display(void *ptrData, const char *msg)
{
	if(TimersSystem.enabled)
	{
		SetColor(Color_White, Color_Black);
		printf_P(PSTR("Timer: %S %u%% %S %u"), msg_Config_Waveform, TimersSystem.dutyCycle, msg_Config_CklSel, TimersSystem.period);
	}
}

uint16_t ComputeDutyCycle(void)
{
	uint16_t offset = 0;

	if(TimersSystem.waveform == TCA_SINGLE_WGMODE_SINGLESLOPE_gc)
	{
		offset = 1;
	}

	// Not sure what the difference is for Normal vs frequency
	// for now, keep them independent and return the default compare value
	// I do know that normal mode using the ISR. 
	if(TimersSystem.waveform == TCA_SINGLE_WGMODE_NORMAL_gc )
	{
		return 0;
	}
	
	if(TimersSystem.waveform == TCA_SINGLE_WGMODE_FRQ_gc  )
	{
		return TimersSystem.compare;
	}
	
	uint32_t dutyCycle = TimersSystem.dutyCycle * (TimersSystem.period + offset);
	dutyCycle /= 100;	
	return (uint16_t) dutyCycle;
}

void Program_Timer(void)
{
    // Disable TCA0 during configuration
    TCA0.SINGLE.CTRLA = 0;

    TCA0.SINGLE.CTRLB = TimersSystem.waveform        // Mode
                      | TCA_SINGLE_CMP0EN_bm;        // Enable compare channel 0
					  
    TCA0.SINGLE.CTRLA = TimersSystem.clksel;
    TCA0.SINGLE.PER = TimersSystem.period;
    TCA0.SINGLE.CMP0 = ComputeDutyCycle();

	if(TimersSystem.waveform == TCA_SINGLE_WGMODE_NORMAL_gc )
	{
		TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	}
	else
	{
		TCA0.SINGLE.INTCTRL = 0;		
	}

    // Enable timer
	if(TimersSystem.enabled)
    {
		TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;	
	}
}

void Timers_ReInitialize(void)
{
	Timers_Load_Config();
	Program_Timer();
	switch(TimersSystem.waveform)
	{
		case TCA_SINGLE_WGMODE_NORMAL_gc:		/* Normal Mode */
			msg_Config_Waveform = msg_Waveform_Normal;
			break;
		case TCA_SINGLE_WGMODE_FRQ_gc:			/* Frequency Generation Mode */
			msg_Config_Waveform = msg_Waveform_Frequency;
			break;
		case TCA_SINGLE_WGMODE_SINGLESLOPE_gc:	/* Single Slope PWM */
			msg_Config_Waveform = msg_Waveform_SingleSlope;
			break;
		case TCA_SINGLE_WGMODE_DSTOP_gc:		/* Dual Slope PWM, overflow on TOP */
			msg_Config_Waveform = msg_Waveform_DualPWMTop;
			break;
		case TCA_SINGLE_WGMODE_DSBOTH_gc:		/* Dual Slope PWM, overflow on TOP and BOTTOM */
			msg_Config_Waveform = msg_Waveform_DualPWMTopBottom;
			break;
		case TCA_SINGLE_WGMODE_DSBOTTOM_gc:		/* Dual Slope PWM, overflow on BOTTOM */
			msg_Config_Waveform = msg_Waveform_DualPWMBottom;
			break;
	}
	msg_Config_CklSel = ClkselModeDecode(&TimersSystem.clksel);
}

ISR(TCA0_OVF_vect)
{
	// Toggle PF4 on each overflow
	PORTF.OUTTGL = PF4_TimerOutputPin;
	
	// Clear the interrupt flag
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

void Timers_Initialize(void)
{
	// Set PA0 as output
	PORTF.DIRSET = PF4_TimerOutputPin;
	PORTF.OUTCLR = PF4_TimerOutputPin;
	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTF_gc;

	Timers_ReInitialize();
	
	// Add item to display list.
	DisplayProcessorMap item;
	item.ptrData = &TimersSystem;
	item.funcptrPrint = Timers_Display;
	item.msg = NULL;
	item.enabled = TimersSystem.enabled;
	DisplayProcessor_Add(item);
}

void Timers_Monitor(void)
{
	if(TimersSystem.enabled)
	{
		// TODO: I'm not using this, but in keeping with the current pattern.
		// Will figure what to do later.
	}
}



void *GetPtrToTimersData(void)
{
	return &TimersSystem;
}

void Timers_Save_Config(uint8_t channel)
{
	// Save just one element
	// However, I'm maintaining the option of multiple channels. Like i did for ADC
	// Rules Frequency Mode Duty Cycle can only be 50 percent.
	if(TimersSystem.waveform == TCA_SINGLE_WGMODE_FRQ_gc)
	{
		TimersSystem.dutyCycle = DUTYCYCLELIST_50;
	}
	EEPROM_WRITE_ELEMENT(timers_system, 0, &TimersSystem);
	DisplayProcessor_SetEnabledFlag(&TimersSystem, TimersSystem.enabled);
	Timers_ReInitialize();
}

void Timers_Load_Config(void)
{
	// Load just one element
	EEPROM_VERIFY_SIGNATURE_AND_LOAD(timers_system, 0, &TimersSystem);
}

void SetTimerPeriod(float value)
{
	TimersSystem.period = (uint16_t) value;
	Timers_Save_Config(0);
}

void SetTimerCompare(float value)
{
	TimersSystem.compare = (uint16_t) value;
	Timers_Save_Config(0);
}

void GetTimersPeriod(const char *msg)
{
	struct EnterValueByHand *enterValue = GetEnterValuePtr();
	enterValue->fieldtype = FIELDTYPE_UINT16;
	enterValue->Min = 0.0;
	enterValue->Max = 65535.0;
	enterValue->msg = msg_PATTERN;
	enterValue->ptrSaveFunction = SetTimerPeriod;
	EnterNumberValue();
}

void GetTimersCompare(const char *msg)
{
	struct EnterValueByHand *enterValue = GetEnterValuePtr();
	enterValue->fieldtype = FIELDTYPE_UINT16;
	enterValue->Min = 0.0;
	enterValue->Max = 65535.0;
	enterValue->msg = msg_PATTERN;
	enterValue->ptrSaveFunction = SetTimerCompare;
	EnterNumberValue();
}
