/*
 * RTC.c
 *
 * Created: 1/2/2026 10:51:29 AM
 *  Author: terry
 */ 
#include "RTC/RTC.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include "RTC/RTC.h"
#include "EEPRom/EEPRom.h"
#include "CommonStrings/CommonStrings.h"
#include "Display_Processor/Display_Processor.h"

static char CurrentDisplayTime[RTCDisplayBuffer];
static rtc_time_t current_time = {0, 0, 12, 1, 1, 2025};
static bool time_updated = false;
static uint8_t lastSecond = 0;

RTCConfiguration RTCSystem = {
	SIGNATURE_BYTES_RTC,
	true,							// Enabled/Disabled
};

// These pointers are not need for this system.
void RTC_Display(void *ptrData, const char *msg)
{
	if(RTCSystem.enabled)
	{
		// Only update once per second change
		if(lastSecond != current_time.second)
		{
			lastSecond = current_time.second;
			SetColor(Color_White, Color_Black);
			FormatTime(CurrentDisplayTime, &current_time);
			printf("RTC: %s",CurrentDisplayTime);
			// Print to the console. 
			// It will be accepted if not busy. Otherwise we'll print the next time around.
			USART_0_Print(CurrentDisplayTime);
		}
	}
}

void RTC_Initialize(void)
{
	// Disable RTC while configuring
	while (RTC.STATUS & RTC_CTRLABUSY_bm);
	RTC.CTRLA = 0;
	
	// Configure for internal 32.768 kHz oscillator
	// Select internal 32kHz oscillator as RTC clock source
	RTC.CLKSEL = RTC_CLKSEL_OSC32K_gc;
	    
	// Set period to 32768 for 1 second with 32.768 kHz oscillator
	RTC.PER = 32768 - 1;
		    
	// Enable overflow interrupt for 1 second tick
	RTC.INTCTRL = RTC_OVF_bm;
	    
	// Enable RTC with prescaler DIV1 (no division)
	while (RTC.STATUS & RTC_CTRLABUSY_bm);
	RTC.CTRLA = RTC_RTCEN_bm | RTC_PRESCALER_DIV1_gc;

	RTC_Load_Config();
	
	// Add item to display list.
	DisplayProcessorMap item;
	item.ptrData = &RTCSystem;
	item.funcptrPrint = RTC_Display;
	item.msg = NULL;
	item.enabled = RTCSystem.enabled;
	DisplayProcessor_Add(item);

	// TODO: Need a set time command	
	// Set initial time: 12:30:45
	RTC_Set_Time(12, 30, 45);
	
	// TODO: Need a set Date command	
	// Set initial date: January 2, 2025
	RTC_Set_Date(2025, 1, 2);

}



/*
 * Set current time
 */
void RTC_Set_Time(uint8_t hour, uint8_t minute, uint8_t second) 
{
    cli();
    current_time.hour = hour;
    current_time.minute = minute;
    current_time.second = second;
    sei();
}

/*
 * Set current date
 */
void RTC_Set_Date(uint16_t year, uint8_t month, uint8_t day) 
{
    cli();
    current_time.year = year;
    current_time.month = month;
    current_time.day = day;
    sei();
}

/*
 * Check if year is leap year
 */
bool Is_Leap_Year(uint16_t year) 
{
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    if (year % 4 == 0) return true;
    return false;
}

/*
 * Get number of days in month
 */
uint8_t Days_In_Month(uint8_t month, uint16_t year) {
    const uint8_t days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month == 2 && Is_Leap_Year(year))
        return 29;
    
    return days[month - 1];
}

/*
 * Increment time by one second
 */
void RTC_Increment_Time(void) 
{
    current_time.second++;
    
    if (current_time.second >= 60) 
	{
        current_time.second = 0;
        current_time.minute++;
        
        if (current_time.minute >= 60) 
		{
            current_time.minute = 0;
            current_time.hour++;
            
            if (current_time.hour >= 24) 
			{
                current_time.hour = 0;
                current_time.day++;
                
                if (current_time.day > Days_In_Month(current_time.month, current_time.year)) 
				{
                    current_time.day = 1;
                    current_time.month++;
                    
                    if (current_time.month > 12) 
					{
                        current_time.month = 1;
                        current_time.year++;
                    }
                }
            }
        }
    }
    
    time_updated = true;
}

/*
 * Format time as string "YYYY-MM-DD HH:MM:SS"
 */
void FormatTime(char *buffer, const rtc_time_t *time) 
{
    sprintf(buffer, "%02d-%02d-%04d %02d:%02d:%02d",
            time->month, time->day, time->year,
            time->hour, time->minute, time->second);
}

/*
 * RTC overflow interrupt - triggered every second
 */
ISR(RTC_CNT_vect) 
{
    // Clear interrupt flag
    RTC.INTFLAGS = RTC_OVF_bm;
    
    // Increment time
    RTC_Increment_Time();
}

void RTC_Monitor(void)
{
	if(RTCSystem.enabled)
	{
		if (time_updated) 
		{
			time_updated = false;
			        
			// Format and print time
			//cli();
			//rtc_time_t time_copy = current_time;
			//sei();
		}
	}
}



void *GetPtrToRTCData(void)
{
	return &RTCSystem;
}

bool GetRTCEnabledState(void)
{
	return RTCSystem.enabled;
}

void RTC_Save_Config(uint8_t channel)
{
	// Save just one element
	// However, I'm maintaining the option of multiple channels. Like i did for ADC
	EEPROM_WRITE_ELEMENT(rtc_system, 0, &RTCSystem);
	DisplayProcessor_SetEnabledFlag(&RTCSystem, RTCSystem.enabled);
}

void RTC_Load_Config(void)
{
	// Load just one element
	EEPROM_VERIFY_SIGNATURE_AND_LOAD(rtc_system, 0, &RTCSystem);
}
