/*
 * LED_Bar.c
 *
 * Created: 11/25/2025 1:47:16 PM
 *  Author: terry
 */ 

#include "LED_Bar.h"

static float LED_BAR_MaxRange = 0.0;

// -------- Example usage --------
void LED_Bar_Initialize(void)
{
	// Configure PA7-PA0 as output for LED
	PORTA.DIRSET = LED_MASK;
	LED_Bar_SetRange(100.0);
	LED_Bar_Display(0.0);
}

void LED_Bar_SetRange(float value)
{
	LED_BAR_MaxRange  = value;
}
void LED_Bar_Display(float value)
{
	uint8_t mask = 0;
	float displayValue = 0;
	
	// Since the Max Range of the LED Bar Graph is a variable.
	// We need to figure out what each of the Bars are in value
	
	float displayIncrement = LED_BAR_MaxRange / Max_BarSize;
	
	for (uint8_t i = 0; i < Max_BarSize; i++)
	{		
		if (displayValue >= value)
		{
			break;                // done. Post bar
		}
		displayValue += displayIncrement;
		mask |= (0x01 << i);
	}
	PORTA.OUTCLR = LED_MASK;     // clear all LED bits
	PORTA.OUTSET = (mask & LED_MASK);
}
