#include <atmel_start.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "../../../Library/CPU_Interface/CPU_Interface.h"
#include "../CommandList/CommandList.h"
#include "../../../Library/RTC/rtc.h"
#include "./ADC/ADC.h"
#include "./DAC/DAC.h"


int main(void)
{

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	CPU_Interface_Initialization();
	CommandList_Initialize();
	ADC_Initialization();
	DAC_Initialization();

	while (1) {
		CPU_Interface_Monitor();
		if(!Get_CPU_InterfaceStatusBusy())
		{
			ADC_Monitor();
			DAC_Monitor();
			RTC_Monitor();
		}
	}
}
