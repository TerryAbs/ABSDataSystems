/**
* \file
*
* \brief Real Time Counter To Real Time clock.
*
*/
/*
Real Time Clock.


Written by:	Terry D. Schevker
ABS Data Systems, Inc
terry@absdatasystems.com
*/

/**
* \defgroup doc_driver_system_rtc Real Time Counter (RTC)
* \ingroup doc_driver_system
*
* \section doc_driver_rtc_rev Revision History
* - v0.0.0.1 Initial Commit
*
*@{
*/

#include "./rtc.h"
#include "../CPU_Interface/CPU_Interface.h"
#include <stdio.h>

/**
* This code setup up the RTC and Interrupt settings for the RTC timer,
* which is set at a rate of twice per second.
* I tired to run the RTC Timer at once per second, but failed. Not sure why.
* Terry Schevker Custom Real time Clock
*
*/

static 	int64_t GlobalTickCounterSec;
static 	int64_t GlobalTickCountermSec;
static 	int64_t GlobalDisplayTickCounter;
static struct structTimerEvent EventList[MAXNOEVENTS];
static enum RTC_STATE_MACHINE RTC_State_Machine;
static int RTC_Index;

static bool TickUpdateFlag = false;
static void (*RTC_DisplayClock)(void) = NULL;
static bool RTC_ShowClock = false;

void SetDisplayClockPtr(void *ptr)
{
	RTC_DisplayClock = ptr;
}

void SetShowClock(bool value)
{
	RTC_ShowClock = false;
	if(value)
	{
		RTC_ShowClock = true;
		ResetDisplayClockTimers();
	}
}

void ResetDisplayClockTimers()
{
	GlobalDisplayTickCounter = GetTicks() + 1;
}

ISR(TCA0_OVF_vect)
{
	/* Insert your TCA overflow interrupt handling code */
	if(TickUpdateFlag )
	{
		GlobalTickCountermSec++;
		TickUpdateFlag = false;
	}else{
		TickUpdateFlag = true;
	}
	/* The interrupt flag has to be cleared manually */
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}


// Timer is used to track internal tick counts
void Timer_Initialization()
{
	TCA0.SINGLE.INTCTRL = 0 << TCA_SINGLE_CMP0_bp   /* Compare 0 Interrupt: disabled */
	| 0 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: disabled */
	| 0 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: disabled */
	| 1 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt: enabled */

	TCA0.SINGLE.PER = 46; /* Period: 0x1e85 */

	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV256_gc    /* System Clock / 256 */
	| 1 << TCA_SINGLE_ENABLE_bp    /* Module Enable: enabled */
	| 0 << TCA_SINGLE_RUNSTDBY_bp; /* RUN STANDBY: disabled */

	GlobalTickCountermSec = 0;				// This counter is used for state machine delays)
}

int64_t GetTicksms()
{
	return GlobalTickCountermSec;
}

void RTC_Initialization()
{

	while (RTC.STATUS > 0) { /* Wait for all register to be synchronized */
	}

	// RTC.CMP = 0x0; /* Compare: 0x0 */

	// RTC.CNT = 0x0; /* Counter: 0x0 */

	RTC.CTRLA = RTC_PRESCALER_DIV2048_gc /* 2048 */
	| 0 << RTC_RTCEN_bp      /* Enable: disabled */
	| 0 << RTC_RUNSTDBY_bp;  /* Run In Standby: disabled */

	// RTC.PER = 0xffff; /* Period: 0xffff */

	RTC.CLKSEL = RTC_CLKSEL_OSC32K_gc; /* Internal 32.768 kHz oscillator */

	// RTC.DBGCTRL = 0 << RTC_DBGRUN_bp; /* Run in debug: disabled */

	// RTC.INTCTRL = 0 << RTC_CMP_bp /* Compare Match Interrupt enable: disabled */
	//		 | 0 << RTC_OVF_bp; /* Overflow Interrupt enable: disabled */

	while (RTC.PITSTATUS > 0) { /* Wait for all register to be synchronized */
	}

	RTC.PITCTRLA = RTC_PERIOD_CYC16384_gc /* RTC Clock Cycles 8192 */
	| 1 << RTC_PITEN_bp;  /* Enable: enabled */

	// RTC.PITDBGCTRL = 0 << RTC_DBGRUN_bp; /* Run in debug: disabled */

	RTC.PITINTCTRL = 1 << RTC_PI_bp; /* Periodic Interrupt: enabled */

	ResetTimerEvent();
	RTC_State_Machine = RTC_STATE_MACHINE_INDEX;
	RTC_Index = 0;

	Timer_Initialization();
	RTC_DisplayClock = NULL;
	RTC_ShowClock = false;
}

void ResetTimerEvent()
{
	/*
	Initialize the Event System
	*/
	for(int i = 0; i < MAXNOEVENTS; i++)
	{
		EventList[i].Active = false;
		EventList[i].Startfunc = NULL;
		EventList[i].Stopfunc = NULL;
	}
}


int is_leap_year(int year) {
	return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

int days_in_month(int month, int year) {
	int days[] = {31, 28 + is_leap_year(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	return days[month - 1];
}

void SetTime_Cmd(char *value)
{
	struct structtime InternalClockTime;
	sscanf(value, "%d:%d:%d:%d:%d:%d", &InternalClockTime.month, &InternalClockTime.day, &InternalClockTime.year, &InternalClockTime.hour, &InternalClockTime.minute, &InternalClockTime.second);
	DecodeDateTime(&InternalClockTime);

	// Copy this value to Global counter
	GlobalTickCounterSec = InternalClockTime.TotalSeconds;
	SendData("OK");
}

// This function decodes
void DecodeDateTime(struct structtime* DateTime)
{
	// Count the number of seconds
	DateTime->TotalSeconds = 0;

	for (int year = EPOCHYEAR; year < DateTime->year; year++) {
		for (int month = (year == EPOCHYEAR ? EPOCHMONTH : 1); month <= 12; month++) {
			for (int day = (year == EPOCHYEAR && month == EPOCHMONTH ? EPOCHDAY : 1); day <= days_in_month(month, year); day++) {
				DateTime->TotalSeconds += DAYINSECONDS; // Add the number of seconds in a day
			}
		}
	}

	for (int month = EPOCHMONTH; month < DateTime->month; month++) {
		for (int day = (month == EPOCHMONTH ? EPOCHDAY : 1); day <= days_in_month(month, DateTime->year); day++) {
			DateTime->TotalSeconds  += DAYINSECONDS; // Add the number of seconds in a day
		}
	}
	
	int64_t ExtraDays = (int64_t)((DateTime->day - EPOCHDAY) * DAYINSECONDS);
	
	DateTime->TotalSeconds += ExtraDays; // Add the remaining days
	
	int64_t ExtraHours = (int64_t)DateTime->hour;
	ExtraHours *=  3600;
	DateTime->TotalSeconds += ExtraHours;
	
	int64_t ExtraMinutes = (int64_t)DateTime->minute;
	ExtraMinutes *= 60;
	DateTime->TotalSeconds += ExtraMinutes;
	
	DateTime->TotalSeconds += (int64_t) DateTime->second;
}

// This takes total Seconds and converts it into a date time.
void getTimeDecoded(struct structtime* CurrentTime)
{
	CurrentTime->hour = 0;
	CurrentTime->minute = 0;
	CurrentTime->second = 0;
	CurrentTime->day = EPOCHDAY;
	CurrentTime->month = EPOCHMONTH;
	CurrentTime->year = EPOCHYEAR;
	
	while (CurrentTime->TotalSeconds >= DAYINSECONDS)
	{
		// Get the number of day in this month
		int days = days_in_month(CurrentTime->month, CurrentTime->year);
		// Project if the number days is available in TotalSeconds
		if ((CurrentTime->TotalSeconds - (days * DAYINSECONDS)) > 0)
		{
			CurrentTime->TotalSeconds -= (days * DAYINSECONDS);
		}
		else
		{
			for (int i = 1; i < days; i++)
			{
				if (CurrentTime->TotalSeconds >= DAYINSECONDS)
				{
					CurrentTime->TotalSeconds -= DAYINSECONDS;
					CurrentTime->day++;
				}
				else
				{
					break;
				}
			}
		}
		if (CurrentTime->TotalSeconds >= DAYINSECONDS)
		{
			CurrentTime->month++;
			if (CurrentTime->month > 12)
			{
				CurrentTime->month = 1;
				CurrentTime->year++;
			}
		}
	}

	while (CurrentTime->TotalSeconds >= DAYINSECONDS)
	{
		int days = days_in_month(CurrentTime->month, CurrentTime->year);
		for (int i = 1; i < days; i++)
		{
			if (CurrentTime->TotalSeconds >= DAYINSECONDS)
			{
				CurrentTime->TotalSeconds -= DAYINSECONDS;
				CurrentTime->day++;
			}
			else {
				break;
			}
		}
	}

	while (CurrentTime->TotalSeconds >= HOURS)
	{
		if (CurrentTime->TotalSeconds >= HOURS)
		{
			CurrentTime->TotalSeconds -= HOURS;
			CurrentTime->hour++;
		}
		else
		{
			break;
		}
	}

	while (CurrentTime->TotalSeconds >= MINUTES)
	{
		if (CurrentTime->TotalSeconds >= MINUTES)
		{
			CurrentTime->TotalSeconds -= MINUTES;
			CurrentTime->minute++;
		}
		else
		{
			break;
		}
	}

	CurrentTime->second = (int)CurrentTime->TotalSeconds;
}

int64_t GetTicks()
{
	return GlobalTickCounterSec;
}

static int swap = 0;
ISR(RTC_PIT_vect)
{
	if((swap % 2) == 0)
	{
		GlobalTickCounterSec++;
	}
	swap++;
	RTC.PITINTFLAGS = RTC_PI_bm;
}

int GetNextFreeEvent()
{
	int results = -1;

	for(int i = 0; i < MAXNOEVENTS; i++)
	{
		if(EventList[i].Active == false)
		{
			results = i;
			break;
		}
	}
	return results;
}

bool KillTimerEvent(int tagValue)
{
	bool Results = false;

	for(int i = 0; i < MAXNOEVENTS; i++)
	{
		if(EventList[i].tagValue == tagValue)
		{
			EventList[i].Active = false;
			EventList[i].Startfunc = NULL;
			EventList[i].Stopfunc = NULL;
			Results = true;
			break;
		}
	}
	return Results;
}

bool SetTimerEvent(struct structTimerEvent Event)
{
	int index = GetNextFreeEvent();
	if(index != -1)
	{
		EventList[index].Active = true;
		EventList[index].Startfunc = Event.Startfunc;
		EventList[index].Stopfunc = Event.Stopfunc;
		EventList[index].TimerExporationTime = Event.TimerExporationTime;
		EventList[index].NoTimes = Event.NoTimes;
		EventList[index].ResetTimeValue  = Event.ResetTimeValue;
		EventList[index].tagValue = Event.tagValue;
		if(Event.Startfunc != NULL)
		{
			Event.Startfunc(Event.tagValue);
		}
		return true;
	}
	// There is no room for this event.
	return false;
}



/**
* This function Monitors and fires Active Events
*/
void RTC_Monitor()
{
	switch(RTC_State_Machine)
	{
		case RTC_STATE_MACHINE_INDEX:
		if(EventList[RTC_Index].Active == true)
		{
			RTC_State_Machine = RTC_STATE_MACHINE_TESTTIMER;
		}
		else
		{
			RTC_State_Machine = RTC_STATE_MACHINE_INCREMENTINDEX;
		}
		break;
		case RTC_STATE_MACHINE_TESTTIMER:
		if(GetTicks() >= EventList[RTC_Index].TimerExporationTime)
		{
			if(EventList[RTC_Index].Stopfunc != NULL)
			{
				// Call the function
				EventList[RTC_Index].Stopfunc(EventList[RTC_Index].tagValue);
				if(EventList[RTC_Index].NoTimes > 0 )
				{
					EventList[RTC_Index].NoTimes--;
					EventList[RTC_Index].TimerExporationTime = GetTicks() + EventList[RTC_Index].ResetTimeValue;
				}
				else
				{
					// Reset the event so its back being available
					EventList[RTC_Index].Active = false;
				}
			}
		}
		RTC_State_Machine = RTC_STATE_MACHINE_INCREMENTINDEX;
		break;
		case RTC_STATE_MACHINE_INCREMENTINDEX:
		RTC_Index++;
		// Test to see if we are at the end of the list
		if(RTC_Index == MAXNOEVENTS)
		{
			// Reset the pointer back to the beginning of the list.
			RTC_Index = 0;
		}
		RTC_State_Machine = RTC_STATE_MACHINE_INDEX;
		break;
	}

	if (RTC_ShowClock)
	{
		if(GetTicks() >= GlobalDisplayTickCounter)
		{
			if(RTC_DisplayClock!= NULL)
			{
				RTC_DisplayClock();
				ResetDisplayClockTimers();
			}
		}
	}
}
