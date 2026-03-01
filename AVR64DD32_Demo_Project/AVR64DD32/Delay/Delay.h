/*
 * Delay.h
 *
 * Created: 08/28/2023 5:58:46 AM
 * Author: TERRY SCHEVKER
 *		   ABS Data Systems, Inc.
 *		   terry@absdatasystems.com
 */
#pragma once

#define __DELAY_BACKWARD_COMPATIBLE__

#include "Config/clock_config.h"
#include <util/delay.h>


void MicroDelay(double value);		//*< MicroDelay (double value represents us) */
void MilliDelay(double value);      //*< MilliDelay (double value represents ms) */
