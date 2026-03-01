/**
 * @file    ADC.h
 * @brief   Analog-to-Digital Converter driver interface for AVR64DD32
 * @details Provides multi-channel ADC functionality with support for instant,
 *          averaged, and differential measurements. Includes configuration
 *          persistence via EEPROM and integration with display processor.
 *
 * @author  Terry (ABS Data Systems)
 * @date    Created: 11/25/2025 2:01:20 PM
 * @version 1.0
 *
 * @par Hardware Configuration
 *      Test Circuit (Resistor Network):
 *      @verbatim
 *      VDD --- 27k? --- [PD5] --- 12k? --- [PD4] --- 10k? --- GND
 *      
 *      Expected Voltages:
 *        PD5 (Instant):      ~2.25V to GND
 *        PD4 (Average):      ~1.02V to GND
 *        PD5-PD4 (Diff):     ~1.23V
 *      @endverbatim
 *
 * @par Features
 *      - Three independent ADC channels (Instant, Average, Differential)
 *      - 12-bit resolution (4096 counts)
 *      - Selectable reference voltage (1.024V to VDD)
 *      - Hardware averaging (up to 16 samples)
 *      - Configurable prescaler for ADC clock
 *      - EEPROM configuration persistence
 *      - Display processor integration
 *
 * @note    Channels use fixed pins: PD4 (Average), PD5 (Instant)
 * @note    VREF must be initialized before ADC usage
 * @warning Digital input buffers are disabled on ADC pins for accuracy
 * @warning Blocking reads - conversion time depends on prescaler setting
 *
 * @see Menu_ADC.h for user interface
 * @see EEPRom.h for configuration storage
 * @see Display_Processor.h for data display
 */ 

#pragma once

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include "Common/Common.h"
#include "Common/DataStructures.h"

#define ADCSystemSize 3			// Total number of ADC systems

enum ADC_CHANNELNAME{
	ADC_CHANNELNAME_INSTANT,
	ADC_CHANNELNAME_AVERAGE,
	ADC_CHANNELNAME_DIFFERENTIAL
};

enum ADC_STATE {
	ADC_STATE_IDLE,
	ADC_STATE_ACTIVE,
	ADC_STATE_INITIALIZE,
	ADC_STATE_READ,
	ADC_STATE_DISPLAY,
	ADC_STATE_COMPLETE,
};

/**
 * @struct ADCConfiguration
 * @brief Holds runtime configuration values for the ADC subsystem.
 *
 * Used by Menu_ADC and driver initialization routines.
 */
typedef struct ADCConfiguration
{
	enum SIGNATURE_BYTES signature;
	enum ADC_CHANNELNAME channel;
	bool enabled;
	uint8_t refRange;
	uint8_t sampNum;
	uint8_t sampleDly;
	uint8_t noDataBits;
	uint8_t prescale;
} ADCConfiguration;


typedef struct ADCProcessEngine
{
	ADCConfiguration config;
	uint16_t ADC_Results;
	float displayValue;
	const char *displayMsg;
	void (*funcptr_Initialize)(struct ADCProcessEngine* config);
	bool (*funcptr_Read)(struct ADCProcessEngine* config);
	void (*funcptr_Display)(struct ADCProcessEngine* config);
} ADCProcessEngine;

void ADC_Display(void *ptrData, const char *msg);
void ADC_Initialize_Average(ADCProcessEngine* ptr);
bool ADC_Read_Average(ADCProcessEngine* ptr);
void ADC_Display_Average(ADCProcessEngine* ptr);

void ADC_Initialize_Instant(ADCProcessEngine* ptr);
bool ADC_Read_Instant(ADCProcessEngine* ptr);
void ADC_Display_Instant(ADCProcessEngine* ptr);

void ADC_Initialize_Differential(ADCProcessEngine* ptr);
bool ADC_Read_Differential(ADCProcessEngine* ptr);
void ADC_Display_Differential(ADCProcessEngine* ptr);

void ADC_Initialize(void);
void ADC_Monitor(void);


void *GetPtrToADCData(uint8_t index);

char *ADCPresScaleDecode(void *ptr);
char *ADCSampleDelayDecode(void *ptr);
char *ADC_BitsDecode(void *ptr);
char *ADCSampleNumberDecode(void *ptr);
char *ADCRefDecode(void *ptr);

void ADC_Save_Config(uint8_t channel);
void ADC_Load_Config(uint8_t channel);
float VoltsPerTick(ADCProcessEngine *ptr);
bool GetADCEnabledState(void);