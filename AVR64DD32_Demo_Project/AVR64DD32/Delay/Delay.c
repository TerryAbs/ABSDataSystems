
/*
 * DelayWrapper.c
 *
 * Created: 10/1/2023 11:26:41 AM
 * Author: TERRY SCHEVKER
 *		   ABS Data Systems, Inc.
 *		   terry@absdatasystems.com
 */
 // These are a wrapper function to the Library calls.
 // I probably should create these as Macro's

 #include "Delay.h"

 void MilliDelay(double value)
 {
	 _delay_ms(value);
 }


 void MicroDelay(double value)
 {
	 _delay_us(value);
 }