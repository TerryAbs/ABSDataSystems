/*
 * CCL.c
 *
 * Created: 12/31/2025 2:10:10 PM
 *  Author: terry
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include "Delay/Delay.h"
#include "CCL/CCL.h"
#include "EEPRom/EEPRom.h"
#include "CommonStrings/CommonStrings.h"
#include "Display_Processor/Display_Processor.h"

static uint8_t CCL_StepsData;

CCLConfiguration CCLSystem = {
	SIGNATURE_BYTES_CCL,
	true,							// Enabled/Disabled
	7,								// Number of steps for testing
	CCL_INPUTS_A_B_C,				// InputSources
	0xFF							// Pattern
};

// These pointers are not need for this system.
void CCL_Display(void *ptrData, const char *msg)
{
	if(CCLSystem.enabled)
	{
		SetColor(Color_White, Color_Black);
		printf("CCL Step(%u) Pattern %u (0x%02X)", CCL_StepsData, CCLSystem.pattern, CCLSystem.pattern);
	}
}

void CCL_DriveTestData(int value)
{
	char data = PORTD.IN & 0xF0;
	PORTD.OUT = ( data | (value << 1));		// Adjust for starting with PD1 as there is no PD0
}

void CCL_ReInitialize(void)
{
	CCL_Load_Config();
	
	// Disable CCL while configuring
	CCL.CTRLA = 0;
	CCL.LUT3CTRLA = 0;
	switch(CCLSystem.inputSource)	    
	{
		case CCL_INPUTS_A:
			CCL.LUT3CTRLB = CCL_INSEL0_IN0_gc | CCL_INSEL1_MASK_gc;
			CCL.LUT3CTRLC = CCL_INSEL2_MASK_gc;
			break;
		case CCL_INPUTS_A_B:
			CCL.LUT3CTRLB = CCL_INSEL0_IN0_gc | CCL_INSEL1_IN1_gc;
			CCL.LUT3CTRLC = CCL_INSEL2_MASK_gc;
			break;
		case CCL_INPUTS_A_B_C:
			CCL.LUT3CTRLB = CCL_INSEL0_IN0_gc | CCL_INSEL1_IN1_gc;   
			CCL.LUT3CTRLC = CCL_INSEL2_IN2_gc;   
			break;
	}
	CCL.TRUTH3 = CCLSystem.pattern;
	    
	// Enable LUT output to pin and enable the LUT
	CCL.LUT3CTRLA = CCL_OUTEN_bm | CCL_ENABLE_bm;
	    
	// Enable CCL system
	CCL.CTRLA = CCL_ENABLE_bm;
	CCL_StepsData = 0;
}

void CCL_Initialize(void)
{
	// Configure input pins with pullups (active LOW)
	PORTF.DIRCLR = PF0_INPUT_A_PIN | PF1_INPUT_B_PIN | PF2_INPUT_C_PIN;
	PORTF.PIN0CTRL = PORT_PULLUPEN_bm;
	PORTF.PIN1CTRL = PORT_PULLUPEN_bm;
	PORTF.PIN2CTRL = PORT_PULLUPEN_bm;
	    
	// Configure LUT output pin
	PORTF.DIRSET = PF3_LUT3_OUT_PIN;  // PA3 - LUT0 output

	PORTD.DIRSET = PD1_OUTPUT_A_PIN | PD2_OUTPUT_B_PIN | PD3_OUTPUT_C_PIN;

	CCL_ReInitialize();
	
	// Add item to display list.
	DisplayProcessorMap item;
	item.ptrData = &CCLSystem;
	item.funcptrPrint = CCL_Display;
	item.msg = NULL;
	item.enabled = CCLSystem.enabled;
	DisplayProcessor_Add(item);
}


void CCL_Monitor(void)
{
	if(CCLSystem.enabled)
	{
		CCL_DriveTestData(CCL_StepsData);
		CCL_StepsData++;
		if(CCL_StepsData > CCLSystem.steps)
		{
			CCL_StepsData = 0;
		}
		// I would normally never use a delay like this.
		// The only reason for this delay is to let the PORTD
		// settle down so the pattern is stable on the scope.
		// the whole premise behind the CCL is not to be port driven,
		// But to be hardware driven. So PORTD is only used as a simulator.
		// Hence the reason for the delay
		MilliDelay(1);
	}
}

void *GetPtrToCCLData(void)
{
	return &CCLSystem;
}

bool GetCCLEnabledState(void)
{
	return CCLSystem.enabled;
}

void CCL_Save_Config(uint8_t channel)
{
	// Save just one element
	// However, I'm maintaining the option of multiple channels. Like i did for ADC
	EEPROM_WRITE_ELEMENT(ccl_system, 0, &CCLSystem);
	DisplayProcessor_SetEnabledFlag(&CCLSystem, CCLSystem.enabled);
	CCL_ReInitialize();
}

void CCL_Load_Config(void)
{
	// Load just one element
	EEPROM_VERIFY_SIGNATURE_AND_LOAD(ccl_system, 0, &CCLSystem);
}

void GetCCLSteps(const char *msg)
{
	struct EnterValueByHand *enterValue = GetEnterValuePtr();
	enterValue->fieldtype = FIELDTYPE_UINT8;
	enterValue->Min = 0.0;
	enterValue->Max = 7.0;
	enterValue->msg = msg_STEPS;
	enterValue->ptrSaveFunction = SetCCLSteps;
	EnterNumberValue();
}

void SetCCLSteps(float value)
{
	CCLSystem.steps =  (uint8_t) value;
	CCL_Save_Config(0);
}

	
void GetCCLPattern(const char *msg)
{
	struct EnterValueByHand *enterValue = GetEnterValuePtr();
	enterValue->fieldtype = FIELDTYPE_UINT8;
	enterValue->Min = 0.0;
	enterValue->Max = 255.0;
	enterValue->msg = msg_PATTERN;
	enterValue->ptrSaveFunction = SetCCLPattern;
	EnterNumberValue();

}

void SetCCLPattern(float value)
{
	CCLSystem.pattern = (uint8_t) value;
	CCL_Save_Config(0);
}
