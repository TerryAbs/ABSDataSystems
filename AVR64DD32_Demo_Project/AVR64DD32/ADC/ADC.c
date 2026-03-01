/**
 * @file    ADC.c
 * @brief   Implementation of ADC driver for AVR64DD32
 * @details Implements three ADC channels with distinct characteristics:
 *          - Instant: Single-shot reading, fast response
 *          - Average: Hardware-averaged (16 samples), noise reduction
 *          - Differential: Measures voltage difference between PD5 and PD4
 *
 * @author  Terry (ABS Data Systems)
 * @date    Created: 11/25/2025 2:01:07 PM
 * @version 1.0
 *
 * @par Algorithm
 *      1. Initialize: Configure VREF, prescaler, sample count
 *      2. Read: Trigger conversion and wait for RESRDY flag
 *      3. Convert: Apply scaling based on reference and resolution
 *      4. Display: Format and output via display processor
 *
 * @par State Machine
 *      IDLE ? INITIALIZE ? READ ? DISPLAY ? COMPLETE ? IDLE
 *
 * @note    Uses function pointers for polymorphic channel behavior
 * @note    Results are automatically displayed when channels are enabled
 * @warning Ensure global interrupts are enabled for proper operation
 * @warning Sample delay and prescaler affect conversion time and accuracy
 *
 * @see ADC.h for interface definitions
 */

#include "ADC.h"
#include "EEPRom/EEPRom.h"
#include "Menus/ADC/Menu_ADC.h"
#include "Menu_Report_Processor/Menu_Processor/Menu_Processor.h"
#include "Display_Processor/Display_Processor.h"
#include "Delay/Delay.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Menu_Report_Processor/Themes/Themes.h"

// Test setup.
// I have a resistor network connect to

// Hardware setup. (Test Circuit)
// PD5 is connected to Instant
// PD4 is connected to Average.
// Differential PD5(+) PD4(-)
// VDD --- 27k (PD5) --- 12K --- PD4 --- 10K --- GND 
// or
// VDD --- 10k (PD5) --- 5K --- PD4 --- 3.9kK --- GND 
// PD5 ~2.25V to ground
// PD4 ~1.02V to ground
// Delta ~1.23V



#define ADC_DIFF_COUNTS          (2048.0f)

// This struct contains a list of all the coded ADC Channels used for Reading.
// Current Each System is fixed to a pin. Which is based on my FTDI Demo board.

ADCProcessEngine ADCSystem[ADCSystemSize] = 
{
	{ {SIGNATURE_BYTES_ADC, ADC_CHANNELNAME_INSTANT,		true, VREF_REFSEL_4V096_gc, ADC_SAMPNUM_NONE_gc,	ADC_SAMPDLY_DLY0_gc, ADC_RESSEL_12BIT_gc,	ADC_PRESC_DIV16_gc},	0, 0.0, msg_INSTANT, ADC_Initialize_Instant, ADC_Read_Instant, ADC_Display_Instant },
	{ {SIGNATURE_BYTES_ADC, ADC_CHANNELNAME_AVERAGE,		true, VREF_REFSEL_4V096_gc, ADC_SAMPNUM_ACC16_gc,	ADC_SAMPDLY_DLY0_gc, ADC_RESSEL_12BIT_gc,	ADC_PRESC_DIV16_gc},	0, 0.0, msg_AVERAGE, ADC_Initialize_Average, ADC_Read_Average, ADC_Display_Average },		
	{ {SIGNATURE_BYTES_ADC, ADC_CHANNELNAME_DIFFERENTIAL,	true, VREF_REFSEL_4V096_gc, ADC_SAMPNUM_NONE_gc,	ADC_SAMPDLY_DLY0_gc, ADC_RESSEL_12BIT_gc,	ADC_PRESC_DIV4_gc},		0, 0.0, msg_DIFFERENTIAL, ADC_Initialize_Differential, ADC_Read_Differential, ADC_Display_Differential}
};

static enum ADC_STATE ADC_STATE_StateMachine;
static uint8_t ADCProcessEngineIndex;

void ADC_Initialize(void)
{
	DisplayProcessorMap item;
	ADCProcessEngineIndex = 0;
	ADC_STATE_StateMachine = ADC_STATE_IDLE;
	
	// Log the item into the display processor
	for(uint8_t i = 0; i < ADCSystemSize; i++)
	{
		ADC_Load_Config(i);			//  Load configuration data from EEPRom
		item.ptrData = &ADCSystem[i];
		item.funcptrPrint = ADC_Display;
		item.msg = ADCSystem[i].displayMsg;
		item.enabled = ADCSystem[i].config.enabled;
		DisplayProcessor_Add(item);
	}		 
}

// Common Code area
void ADC_Display(void *ptrData, const char *msg)
{
	ADCProcessEngine *ptr = ptrData;
	if(ptr->config.enabled)
	{
		SetColor(Color_White, Color_Black);
		printf_P(PSTR("ADC %S: %3.3f (%d)"), msg, ptr->displayValue, ptr->ADC_Results);
	}
}

void *GetPtrToADCData(uint8_t index)
{
	return &ADCSystem[index];
}

bool GetADCEnabledState(void)
{
	return ADCSystem[0].config.enabled;
}

// Instant Read Channel 
// Port DP5
void ADC_Initialize_Instant(ADCProcessEngine* ptr)
{
		// Disable the digital input buffer on PD5 to reduce power/leakage
		PORTD.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;

		// Set Reference range
		VREF.ADC0REF = ptr->config.refRange;

		// Prescaler: choose a safe ADC clock (e.g., CLK_PER/16). Adjust to meet data sheet timing/noise needs.
		ADC0.CTRLC = ptr->config.prescale;
		
		// Set the number of samples to read
		ADC0.CTRLB = ptr->config.sampNum;
		
		// Set the number of data bit, free-running
		ADC0.CTRLA = ptr->config.noDataBits | ADC_FREERUN_bm;

		// Select PD5 as input 
		ADC0.MUXPOS = ADC_MUXPOS_AIN5_gc;

		// Optionally lengthen sampling if your source impedance is high
		ADC0.SAMPCTRL = ptr->config.sampleDly; // default (2 CLK_ADC). Increase if needed.

		// Enable ADC and start free-running conversions
		ADC0.CTRLA |= ADC_ENABLE_bm;
		ADC0.COMMAND = ADC_STCONV_bm;
}

bool ADC_Read_Instant(ADCProcessEngine* ptr)
{
	bool results = false;
	if(ADC0.INTFLAGS & ADC_RESRDY_bm)
	{
		ptr->ADC_Results = ADC0.RES;
		ADC0.INTFLAGS = ADC_RESRDY_bm;
		// 12-bit, Stop free-running
		ADC0.CTRLA = ptr->config.noDataBits;
		results = true;
	}
	return results;
}

void ADC_Display_Instant(ADCProcessEngine* ptr)
{
	ptr->displayValue = ((float) ptr->ADC_Results * VoltsPerTick(ptr));
}

// Average Read Channel
// Port D4
void ADC_Initialize_Average(ADCProcessEngine* ptr)
{
	// Disable the digital input buffer on PD4 to reduce power/leakage
	PORTD.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;

	// Select reference for ADC0 (turn on always if you want minimal startup latency)
	VREF.ADC0REF = ptr->config.refRange;

	// Prescaler: choose a safe ADC clock (e.g., CLK_PER/16). Adjust to meet data sheet timing/noise needs.
	ADC0.CTRLC = ptr->config.prescale;

	// Accumulate n samples per result for stability
	ADC0.CTRLB = ptr->config.sampNum;
	
	// Number of data bits, free-running
	ADC0.CTRLA = ptr->config.noDataBits | ADC_FREERUN_bm;

	// Select PD4 as input 
	ADC0.MUXPOS = ADC_MUXPOS_AIN4_gc;

	// Optionally lengthen sampling if your source impedance is high
	ADC0.SAMPCTRL = ptr->config.sampleDly; // default (2 CLK_ADC). Increase if needed.

	// Enable ADC and start free-running conversions
	ADC0.CTRLA |= ADC_ENABLE_bm;
	ADC0.COMMAND = ADC_STCONV_bm;
}

bool ADC_Read_Average(ADCProcessEngine* ptr)
{
	bool results = false;
	if(ADC0.INTFLAGS & ADC_RESRDY_bm)
	{
		ptr->ADC_Results =  (ADC0.RES >> 4);// this is the SUM of 64 samples (SAMPNUM=ACC64)
		ADC0.INTFLAGS = ADC_RESRDY_bm;                 // clear flag
		
		//  Stop free-running
		ADC0.CTRLA = ptr->config.noDataBits;
		results = true;
	}
	return results;
}

void ADC_Display_Average(ADCProcessEngine* ptr)
{
	ptr->displayValue = ((float) ptr->ADC_Results * VoltsPerTick(ptr));
}

// Differential Read Channels
// Port D5 + input
// Port D4- input
void ADC_Initialize_Differential(ADCProcessEngine* ptr)
{
    // Select VREF (example: internal 2.5V)
    VREF.ADC0REF = ptr->config.refRange;

    // PD5 & PD4 must be inputs with no pullups
    PORTD.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc; // Disable digital input buffer
    PORTD.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;

    // Turn on ADC, prescaler 4, single-shot mode
    ADC0.CTRLA =
    ADC_ENABLE_bm          // enable ADC
	| ADC_FREERUN_bm		// Free running
    | ptr->config.noDataBits    // 12-bit resolution
    | ADC_CONVMODE_bm;       // <-- DIFFERENTIAL mode 


    // 12-bit mode, no accumulation, right-adjust result
    ADC0.CTRLB =  ptr->config.sampNum;

    // Differential mode enabled
    ADC0.CTRLC = ptr->config.prescale;

    // MUX selection: PD5 = positive input, PD4 = negative input
    ADC0.MUXPOS = ADC_MUXPOS_AIN5_gc;
    ADC0.MUXNEG = ADC_MUXNEG_AIN4_gc;

    // Optional: reduced sample rate for accuracy
    ADC0.CTRLD = 0;
  
	ADC0.SAMPCTRL = ptr->config.sampleDly; 
	
    // Clear any leftover flags
    ADC0.INTFLAGS = ADC_RESRDY_bm;
	
	ADC0.COMMAND = ADC_STCONV_bm;
}

bool ADC_Read_Differential(ADCProcessEngine* ptr)
{
	bool results = false;
	// Wait for result ready
	if(ADC0.INTFLAGS & ADC_RESRDY_bm)
	{
		ptr->ADC_Results = ADC0.RES;
		// Clear flag for next time
		ADC0.INTFLAGS = ADC_RESRDY_bm;
		
		// 12-bit, Stop free-running
		ADC0.CTRLA = ptr->config.noDataBits;
		results = true;	
	}
	return results;
}

void ADC_Display_Differential(ADCProcessEngine* ptr)
{
	float results = LookUp_RefVoltageBySetting(ptr->config.refRange);
	results /= ADC_DIFF_COUNTS;
	ptr->displayValue = results * ptr->ADC_Results;
}

void ADC_Monitor(void)
{
	switch(ADC_STATE_StateMachine)
	{
		case ADC_STATE_IDLE:
			// We don't allow process in the Menu Processor is running
			if(Menu_Processor_Status())
			{
				CursorOff();
				ADC_STATE_StateMachine = ADC_STATE_ACTIVE;
			}
		case ADC_STATE_ACTIVE:
			// Test to see if this ADC System is enabled.
			if (ADCSystem[ADCProcessEngineIndex].config.enabled)
			{
				// Yes
				ADC_STATE_StateMachine = ADC_STATE_INITIALIZE;
			}
			else
			{
				// No
				ADC_STATE_StateMachine = ADC_STATE_COMPLETE;
			}
			break;
				
		case ADC_STATE_INITIALIZE:
			// Test to make sure the function exits and if so call it.
			if (ADCSystem[ADCProcessEngineIndex].funcptr_Initialize != NULL)
			{
				ADCSystem[ADCProcessEngineIndex].funcptr_Initialize(&ADCSystem[ADCProcessEngineIndex]);
				ADC_STATE_StateMachine = ADC_STATE_READ;
			}
			break;
		case ADC_STATE_READ:
			// Test to make sure the function exits and if so call it.
			if (ADCSystem[ADCProcessEngineIndex].funcptr_Read != NULL)
			{
				if(ADCSystem[ADCProcessEngineIndex].funcptr_Read(&ADCSystem[ADCProcessEngineIndex]))
				{
					ADC_STATE_StateMachine = ADC_STATE_DISPLAY;
				}
			}
			break;
		case ADC_STATE_DISPLAY:
			// Test to make sure the function exits and if so call it.
			if (ADCSystem[ADCProcessEngineIndex].funcptr_Display != NULL)
			{
				ADCSystem[ADCProcessEngineIndex].funcptr_Display(&ADCSystem[ADCProcessEngineIndex]);
				ADC_STATE_StateMachine = ADC_STATE_COMPLETE;
			}
			break;
		case ADC_STATE_COMPLETE:
			// Point to the next item in the ADC Process List
			ADCProcessEngineIndex++;
			if (ADCProcessEngineIndex >= ADCSystemSize )
			{
				// We've loop
				ADCProcessEngineIndex = 0;
			}
			ADC_STATE_StateMachine = ADC_STATE_IDLE;
			break;
		}
}

// ADC


char *ADCPresScaleDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupPreScaleByValue((uint16_t) value);	
}

char *ADCSampleDelayDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupSampleDelayByValue((uint16_t) value);	
}

char *ADC_BitsDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupBitsByValue((uint16_t) value);	
}

char *ADCSampleNumberDecode(void *ptr)
{
	uint8_t value = *(uint8_t *)ptr;
	return LookupSampleNumberByValue((uint16_t)value);
}


void ADC_Save_Config(uint8_t channel)
{
	// Save just one element
	EEPROM_WRITE_ELEMENT(adc_system, channel, &ADCSystem[channel].config);
	DisplayProcessor_SetEnabledFlag(&ADCSystem[channel], ADCSystem[channel].config.enabled);
}

void ADC_Load_Config(uint8_t channel)
{
	// Load just one element
	EEPROM_VERIFY_SIGNATURE_AND_LOAD(adc_system, channel, &ADCSystem[channel].config);
}

float VoltsPerTick(ADCProcessEngine *ptr)
{
	float results = LookUp_RefVoltageBySetting(ptr->config.refRange);
	if(ptr->config.noDataBits == ADC_RESSEL_12BIT_gc)
	{
			results /= 4096.0;
	}
	else
	{
		results /= 1024.0;
	}
	return results;
}

