/*
	Real Time Clock.


	Written by:	Terry D. Schevker
				ABS Data Systems, Inc
				terry@absdatasystems.com

	The AVR64DD32 part does not have a real time clock. What it have is real time counter.
	This code uses the RTC hardware as the timer to update the internal clock information.

	The key to using this code is to set the time once the hardware has booted as there are not registers nor 
	battery backup to store this clock information.-

*/

#pragma once

#include <compiler.h>
#include <utils_assert.h>
/**
 *  \cond
 */
#define HOURS	  3600
#define MINUTES	  60
#define DAYINSECONDS 86400
#define EPOCHYEAR  1970
#define EPOCHMONTH 1
#define EPOCHDAY 1

#define MAXNOEVENTS 8
/**
 *  \endcond
 */
 
 /**
  *  Time structure
  */
struct structtime{
	int64_t TotalSeconds;
	int hour;
	int minute;
	int second;
	int day;
	int month;
	int year;
};

typedef void (*Callback) (void);
typedef void (*CallbackWithInt) (int value);

/**
 *  \brief struct Time Events.
 *  
 *  This is the internal structure that houses all the
 *  the information regarding the processing of a timed Event.
 *  
 */
struct structTimerEvent{
	bool Active;			/**< Active tells the Event Monitor that this is a active Timer Event*/
	int NoTimes;			/**< NoTimes is the number of timers this event will auto repeat. */
	int ResetTimeValue;		/**< ResetTimeValue is used to calculate the next Timer Event time. ResetTimeValue + GetTicks() = New time */
	int64_t TimerExporationTime;	/** TimerExporationTime -- this is the time when the Time Event will fire.  */
	int tagValue;			/**< Tag Value is set by the function that registers this event for the purpose of tracking the source */
	CallbackWithInt Stopfunc;	/** This is a pointer to the function, that if populated will be called when the timer expires */
	CallbackWithInt Startfunc;	/** This is a pointer to the function, that if populated will be called when the timer expires */
};


/**
 *  \brief RTC_Initialization().
 *  
 *  Place this function before the while in the main.c
 *  
 */
 void RTC_Initialization();

/**
 *  \brief RTC_Monitor().
 *  
 *  Monitors are used to track processes. 
 *  In this case, the process is to monitor the Timer Events to see when the are to be fired.
 *  
 */
void RTC_Monitor();


void SetTime_Cmd(char *buffer);
void DecodeDateTime(struct structtime* DateTime);
void getTimeDecoded(struct structtime* CurrentTime);
int64_t GetTicks();
void ResetTimerEvent();
bool SetTimerEvent(struct structTimerEvent Event);
bool KillTimerEvent(int tagValue);
int GetNextFreeEvent();
void SetDisplayClockPtr(void *ptr);
void SetShowClock(bool value);
void ResetDisplayClockTimers();
// this is used to track the internal CPU ticks needed for state machine Delays/
int64_t GetTicksms();
void Timer_Initialization();


enum RTC_STATE_MACHINE{
	RTC_STATE_MACHINE_INDEX,
	RTC_STATE_MACHINE_TESTTIMER,
	RTC_STATE_MACHINE_INCREMENTINDEX,
};