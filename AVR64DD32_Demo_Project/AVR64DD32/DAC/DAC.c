/*
 * DAC.c
 *
 * Created: 11/25/2025 1:40:56 PM
 *  Author: terry
 */ 

#include "DAC.h"
#include "EEPRom/EEPRom.h"
#include "CommonStrings/CommonStrings.h"
#include "CommonMenus/CommonMenus.h"
#include "Display_Processor/Display_Processor.h"
#include <math.h>

static float DAC_RefVoltage = 0.0;
static uint16_t SineWaveTable[MAX_DISPLAYSAMPLES];
static uint16_t DAC_StepMachine;
static enum DACSTATEMACHINE DAC_StateMachine;
static uint16_t DAC_LastUint16;
static float DAC_LastFloat;
static float DAC_VoltsPerTick;
static float DAC_SawToothTriangle_VoltsPerDivision;
static float DAC_SawTooth_Triangle_Wave_Value;
/*

Notes:

Not sure how people are going to use this function.

That being said, the value plotted on the output are formula based
If you don't like what I did, change the formulas.
Furthermore,

I didn't create all the possible rules when saving the data.
All I'm doing is providing you with examples.

*/

DACConfiguration DACSystem = {
	 SIGNATURE_BYTES_DAC, 	
	 true,							// Enabled/Disabled
	 VREF_REFSEL_4V096_gc,			// Voltage Reference
	 DAC_DISPLAY_MODES_VALUE,		// Display mode
	 0,//DAC_VREF,					// Max voltage Reference
	 0,								// Offset from 0
	 1								// No of steps
};

void DAC_FloatWrite(float value)
{
	DAC_LastFloat = value; 
	DAC_LastUint16 = VoltsToDAC(value);
	DAC0.DATA = (DAC_LastUint16 << 6);
}

void DAC_Uint16_tWrite(uint16_t value)
{
	DAC_LastUint16 = value;
	DAC_LastFloat = DACToVolts(value);
	DAC0.DATA = value;
}

uint16_t VoltsToDAC(float value)
{
    return (uint16_t)((value / DAC_RefVoltage) * 1023.0f + 0.5f);
}

float DACToVolts(uint16_t value)
{
	// Value is the adjust value base on setting the register DAC0.Data
	// Compensate we get the raw count
	uint16_t compensated = value >> 6;
	return DAC_VoltsPerTick * (float)compensated;
}

// These pointers are not need for this system.
void DAC_Display(void *ptrData, const char *msg)
{
	PGM_P title = NULL;
	if(DACSystem.enabled)
	{
		switch(DACSystem.mode)
		{
			case DAC_DISPLAY_MODES_VALUE:
				title = msg_SETVALUE;
				break;
			case DAC_DISPLAY_MODES_TRIANGLE:
				title = msg_TRIANGLEWAVE;
				break;
			case DAC_DISPLAY_MODES_SAWTOOTH:
				title = msg_SAWTOOHWAVE;
				break;
			case DAC_DISPLAY_MODES_SINEWAVE:
				title = msg_SINEWAVE;
				break;
		}
		SetColor(Color_White, Color_Black);
		printf_P(PSTR("DAC %S: %3.3f (%u) Range(%3.3f)"), title, DAC_LastFloat, DAC_LastUint16, DAC_RefVoltage);
	}
}

void DAC_ReInitialize(void)
{
	DAC_Load_Config();
	DAC_RefVoltage = LookUp_RefVoltageBySetting(DACSystem.refRange);
	VREF.DAC0REF = DACSystem.refRange | VREF_ALWAYSON_bm;

	DAC_VoltsPerTick = DAC_RefVoltage / DAC_MaxResolution;
	
	// Enable DAC output on PD6 (OUTEN = 1)
	DAC0.CTRLA = DAC_ENABLE_bm | DAC_OUTEN_bm;
	
	// Reset all the measurements.
	DAC_StateMachine = DACSTATEMACHINE_INITIALIZE;
	DAC_StepMachine = 0;
}

void DAC_Initialize(void)
{
	// Initialize DAC0 on PD6
	/* Initializes MCU, drivers and middle ware */
	PORTD.DIRCLR = PD6_DAC_Pin;
	PORTD.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc;    // disable digital input buffer
	PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;           // ensure pull-up is off

	DAC_ReInitialize();
	
	// Add item to display list.
	DisplayProcessorMap item;
	item.ptrData = &DACSystem;
	item.funcptrPrint = DAC_Display;
	item.msg = NULL;
	item.enabled = DACSystem.enabled;
	DisplayProcessor_Add(item);
}

void DACValue_Monitor(void)
{
	switch(DAC_StateMachine)
	{
		case DACSTATEMACHINE_INITIALIZE:
			DAC_FloatWrite(DACSystem.peak);
			DAC_StateMachine = DACSTATEMACHINE_DISPLAYVALUE;
			break;
		// There is nothing more to do in this mode.
		// other than to wait for a reset
		case DACSTATEMACHINE_DISPLAYVALUE:
		case DACSTATEMACHINE_COMPUTENXEXTVALUE:
		case DACSTATEMACHINE_COMPLETE:
			break;
	}
}
void DAC_SineWave_Monitor(void)
{
	uint16_t compensated = 0;
	switch(DAC_StateMachine)
	{
		case DACSTATEMACHINE_INITIALIZE:		
			for (int n = 0; n < DACSystem.steps ; n++)
			{
				float phase = (2.0f * M_PI * n) / DACSystem.steps;
				compensated  = VoltsToDAC(DACSystem.Offset + DACSystem.peak * sinf(phase));
				SineWaveTable[n] = (compensated  << 6);
			}
			DAC_StepMachine = 0;
			DAC_StateMachine = DACSTATEMACHINE_DISPLAYVALUE;
			break;
		case DACSTATEMACHINE_DISPLAYVALUE:
			DAC_Uint16_tWrite(SineWaveTable[DAC_StepMachine]);
			DAC_StateMachine = DACSTATEMACHINE_COMPUTENXEXTVALUE;
			break;
		case DACSTATEMACHINE_COMPUTENXEXTVALUE:
			DAC_StepMachine++;
			if(DAC_StepMachine >= DACSystem.steps)
			{
				DAC_StepMachine = 0;
			}
			DAC_StateMachine = DACSTATEMACHINE_DISPLAYVALUE;
			break;
		case DACSTATEMACHINE_COMPLETE:
			// In this mode, this is never called.
			break;
	}
}
void DAC_SawTooth_Monitor(void)
{
	switch(DAC_StateMachine)
	{
		case DACSTATEMACHINE_INITIALIZE:
			DAC_SawToothTriangle_VoltsPerDivision = DACSystem.peak / (DACSystem.steps - 1);
			DAC_SawTooth_Triangle_Wave_Value = DACSystem.Offset;
			DAC_StepMachine = 0;
			DAC_StateMachine = DACSTATEMACHINE_DISPLAYVALUE;
			break;
		case DACSTATEMACHINE_DISPLAYVALUE:
			DAC_FloatWrite(DAC_SawTooth_Triangle_Wave_Value);
			DAC_StateMachine = DACSTATEMACHINE_COMPUTENXEXTVALUE;
			break;
		case DACSTATEMACHINE_COMPUTENXEXTVALUE:
			DAC_SawTooth_Triangle_Wave_Value += DAC_SawToothTriangle_VoltsPerDivision;
			DAC_StepMachine++;
			if(DAC_StepMachine >= DACSystem.steps)
			{
				DAC_StepMachine = 0;
				DAC_SawTooth_Triangle_Wave_Value = DACSystem.Offset;
			}
			DAC_StateMachine = DACSTATEMACHINE_DISPLAYVALUE;
			break;
		case DACSTATEMACHINE_COMPLETE:
			// In this mode, this is never called.
			break;
	}
}


void DAC_Triangle_Wave_Monitor(void)
{
	switch(DAC_StateMachine)
	{
		case DACSTATEMACHINE_INITIALIZE:
			DAC_SawToothTriangle_VoltsPerDivision = DACSystem.peak / (DACSystem.steps / 2);
			DAC_SawTooth_Triangle_Wave_Value = DACSystem.Offset;
			DAC_StepMachine = 0;
			DAC_StateMachine = DACSTATEMACHINE_DISPLAYVALUE;
			break;
		case DACSTATEMACHINE_DISPLAYVALUE:
			DAC_FloatWrite(DAC_SawTooth_Triangle_Wave_Value);
			DAC_StateMachine = DACSTATEMACHINE_COMPUTENXEXTVALUE;
			break;
		case DACSTATEMACHINE_COMPUTENXEXTVALUE:
		
		if(DAC_StepMachine < (DACSystem.steps /2))
		{
			DAC_SawTooth_Triangle_Wave_Value += DAC_SawToothTriangle_VoltsPerDivision;
		}
		else
		{
			DAC_SawTooth_Triangle_Wave_Value -= DAC_SawToothTriangle_VoltsPerDivision;
		}
		if(DAC_SawTooth_Triangle_Wave_Value  < DACSystem.Offset)
		{
			DAC_SawTooth_Triangle_Wave_Value  = DACSystem.Offset;
		}
			DAC_StepMachine++;
			if(DAC_StepMachine >= DACSystem.steps)
			{
				DAC_StepMachine = 0;
				DAC_SawTooth_Triangle_Wave_Value = DACSystem.Offset;
			}
			DAC_StateMachine = DACSTATEMACHINE_DISPLAYVALUE;
			break;
		case DACSTATEMACHINE_COMPLETE:
			// In this mode, this is never called.
			break;
	}
}

void DAC_Monitor(void)
{
	if(DACSystem.enabled)
	{
		switch(DACSystem.mode)
		{		
			case DAC_DISPLAY_MODES_VALUE:
				DACValue_Monitor();
				break;
			case DAC_DISPLAY_MODES_SINEWAVE:
				DAC_SineWave_Monitor();
				break;
			case DAC_DISPLAY_MODES_SAWTOOTH:
				DAC_SawTooth_Monitor();
				break;
			case DAC_DISPLAY_MODES_TRIANGLE:
				DAC_Triangle_Wave_Monitor();
				break;
		}
	}
}

void *GetPtrToDACData(void)
{
	return &DACSystem;
}

void DAC_Save_Config(uint8_t channel)
{
	// Before we save, we need to make sure that rules match.
	// In this the Sine wave uses a buffer and the
	// Buffer size is small that the other settings.
	uint16_t steps = (uint16_t)GetMaxStepSize();
	if(DACSystem.steps > steps)
	{
		DACSystem.steps = steps;
	}

	// Test to make sure that peak is with in range
	float peak = GetMaxPeak(LookUp_RefVoltageBySetting(DACSystem.refRange));
	if(DACSystem.peak > peak)
	{
		DACSystem.peak = peak;
	}
	
	
	// Save just one element
	// However, I'm maintaining the option of multiple channels. Like i did for ADC
	EEPROM_WRITE_ELEMENT(dac_system, 0, &DACSystem);
	DisplayProcessor_SetEnabledFlag(&DACSystem, DACSystem.enabled);
	// Saving data means that our parameters must have changed.
	DAC_StateMachine = DACSTATEMACHINE_INITIALIZE;
	DAC_ReInitialize();
}

void DAC_Load_Config(void)
{
	// Load just one element
	EEPROM_VERIFY_SIGNATURE_AND_LOAD(dac_system, 0, &DACSystem);
}

char *DACModeDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupDACModeByValue(value);
}

float GetMaxPeak(float defaultValue)
{
	float results = 0.0;
	// Based on the Mode
	// this function returns the max
	// max peak for the current mode;		
	switch(DACSystem.mode)
	{		
		case DAC_DISPLAY_MODES_VALUE:
		case DAC_DISPLAY_MODES_SAWTOOTH:
		case DAC_DISPLAY_MODES_TRIANGLE:
			results = defaultValue;
			break;
		case DAC_DISPLAY_MODES_SINEWAVE:
			results = defaultValue / 2.0f;
			break;
	}
	return results;

}

void GetDACPeakValue(const char *msg)
{
	struct EnterValueByHand *enterValue = GetEnterValuePtr();
	enterValue->fieldtype = FIELDTYPE_FLOAT;
	enterValue->Min = 0.0;
	enterValue->Max = GetMaxPeak(LookUp_RefVoltageBySetting(DACSystem.refRange));
	enterValue->msg = msg_PEAK;
	enterValue->ptrSaveFunction = SetDACPeakValue;
	EnterNumberValue();
}

void SetDACPeakValue(float value)
{
	DACSystem.peak = value;
	DAC_Save_Config(0);
}

void GetDACOffsetValue(const char *msg)
{
	struct EnterValueByHand *enterValue = GetEnterValuePtr();
	enterValue->fieldtype = FIELDTYPE_FLOAT;
	enterValue->Min = 0.0;
	enterValue->Max = LookUp_RefVoltageBySetting(DACSystem.refRange) / 2.0;
	enterValue->msg = msg_OFFSET;
	enterValue->ptrSaveFunction = SetDACOffsetValue;
	EnterNumberValue();
}

void SetDACOffsetValue(float value)
{
	DACSystem.Offset = value;
	DAC_Save_Config(0);
}

float GetMaxStepSize(void)
{
	float results = 0.0;
	// Based on the Mode
	// this function returns the max
	// step count for the current mode;
		// Input Rules
	switch(DACSystem.mode)
	{		
		case DAC_DISPLAY_MODES_VALUE:
			results = 1;				// This really isn't an option for this setting.
			break;
		case DAC_DISPLAY_MODES_SINEWAVE:
			results = MAX_DISPLAYSAMPLES;
			break;
		case DAC_DISPLAY_MODES_SAWTOOTH:
		case DAC_DISPLAY_MODES_TRIANGLE:
			results = DAC_MaxResolution;
			break;
	}
	return results;
}

void GetDACStepValue(const char *msg)
{
	struct EnterValueByHand *enterValue = GetEnterValuePtr();
	enterValue->fieldtype = FIELDTYPE_UINT16;
	enterValue->Min = 0.0;
	// Input Rules
	enterValue->Max = (float) GetMaxStepSize();
	enterValue->msg = msg_STEPS;
	enterValue->ptrSaveFunction = SetDACStepValue;
	EnterNumberValue();
}

void SetDACStepValue(float value)
{
	DACSystem.steps = (uint16_t) value;
	DAC_Save_Config(0);
}
