/*
 * Common.c
 *
 * Created: 11/27/2025 9:29:47 AM
 *  Author: terry
 */ 

#include <avr/io.h>
#include "Common.h"

struct RefVoltage_Range refVoltage_Range[] = {
	{ VREF_REFSEL_1V024_gc, Range_1024,	1.024},
	{ VREF_REFSEL_2V048_gc, Range_2048,	2.048},
	{ VREF_REFSEL_2V500_gc, Range_2500,	2.500}, 
	{ VREF_REFSEL_4V096_gc, Range_4096,	4.096},
	{ VREF_REFSEL_VDD_gc,	Range_VDD,  5.000},
// TODO: Need to determine how to handle.
// maybe a saved configuration value.		
	{ VREF_REFSEL_VREFA_gc, Range_VREF, 0.0}			// We have know idea of what the uses has connected to the input.
};

#define RefVoltage_Size sizeof(refVoltage_Range)/sizeof(struct RefVoltage_Range)

struct RefVoltage_Range LoopUp_RefVoltage(enum REFVOLTAGE value)
{
	return refVoltage_Range[value];
};

float LookUp_RefVoltageBySetting(uint8_t value)
{
	for(int i = 0; i < RefVoltage_Size; i++)
	{
		if(refVoltage_Range[i].RegisterSetting == value)
		{
			return refVoltage_Range[i].MaxRange;
		}
	}
	return 0.0;
};

// Generic
char *EnableDisableDecode(void *ptr)
{
	bool boolValue = *(bool *)ptr;
	return LookupEnableDisabledByValue((uint16_t) boolValue);
}


char *PortDirectionDecode(void *ptr)
{
	bool boolValue = *(bool *)ptr;
	return LookupPortDirectionByValue((uint16_t) boolValue);
}

char *PortInvertedDecode(void *ptr)
{
	bool boolValue = *(bool *)ptr;
	return LookupPortInvertedByValue((uint16_t) boolValue);
}

char *PortResistorDecode(void *ptr)
{
	bool boolValue = *(bool *)ptr;
	return LookupPortResistorByValue((uint16_t) boolValue);
}

char *HighLowDecode(void *ptr)
{
	bool boolValue = *(bool *)ptr;
	return LookupHighLowByValue((uint16_t) boolValue);
}

char *PortInputLevelDecode(void *ptr)
{
	bool boolValue = *(bool *)ptr;
	return LookupPortInputLevelByValue((uint16_t) boolValue);
}
char *RefDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupVRefByValue(value);
}
