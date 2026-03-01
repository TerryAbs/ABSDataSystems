/**
 * @file    DAC.h
 * @brief   Digital-to-Analog Converter driver interface for AVR64DD32
 * @details Provides configurable DAC output on PD6 with multiple waveform
 *          generation modes including DC, square wave, triangle, sine, and
 *          sawtooth. Supports both manual and automatic waveform generation.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Hardware
 *      - Output Pin: PD6 (TP12 on FTDI demo board)
 *      - Resolution: 10-bit (1024 levels)
 *      - Reference: Internal 4.096V or VDD
 *      - Output Range: 0V to VREF
 *
 * @par Waveform Modes
 *      - DC: Constant voltage output
 *      - Square: 50% duty cycle, configurable frequency
 *      - Triangle: Linear rise and fall
 *      - Sine: Smooth sine wave via lookup table
 *      - Sawtooth: Linear rise, instant fall
 *
 * @note    Run mode enables automatic waveform generation
 * @note    Frequency depends on main loop execution rate
 * @warning Output drive capability limited - use buffer for heavy loads
 *
 * @see Menu_DAC.h for user configuration interface
 */

#pragma once
#include "Common/Common.h"
#define DACSystemSize 1

#define PD6_DAC_Pin 0x40 
/*
	 here are my notes for the menus
	 
	 1. Enable/Disable
	 2. Reference voltages source
	 
	 3. Set a dac value.
	 4. Triangle wave 
		peak
		offset
		Steps
	5. Sawtooth
		peak
		offset
		Steps
	6. sine wave
		peak
		offset
		steps.
*/		


enum DAC_DISPLAY_MODES{
	DAC_DISPLAY_MODES_VALUE,
	DAC_DISPLAY_MODES_SINEWAVE,
	DAC_DISPLAY_MODES_SAWTOOTH,
	DAC_DISPLAY_MODES_TRIANGLE
};

typedef struct DACConfiguration
{
	enum SIGNATURE_BYTES signature;
	bool enabled;
	uint8_t refRange;
	enum DAC_DISPLAY_MODES mode;
	float peak;
	float Offset;
	uint16_t steps;

} DACConfiguration;


// DAC Resolution
#define DAC_Resolution 10	// Bits
#define DAC_MaxResolution 1023.0
#define MAX_DISPLAYSAMPLES 500
	
void DAC_FloatWrite(float value);
void DAC_Uint16_tWrite(uint16_t value);
uint16_t VoltsToDAC(float value);
float DACToVolts(uint16_t value);

void DAC_Initialize(void);
void DAC_ReInitialize(void);
void DAC_Monitor(void);

char *DACModeDecode(void *ptr);
void *GetPtrToDACData(void);
void DAC_Save_Config(uint8_t channel);
void DAC_Load_Config(void);
float GetMaxStepSize(void);
float GetMaxPeak(float defaultValue);

void GetDACPeakValue(const char *msg);
void SetDACPeakValue(float value);
void GetDACOffsetValue(const char *msg);
void SetDACOffsetValue(float value);
void GetDACStepValue(const char *msg);
void SetDACStepValue(float value);

enum DACSTATEMACHINE{
	DACSTATEMACHINE_INITIALIZE,
	DACSTATEMACHINE_DISPLAYVALUE,
	DACSTATEMACHINE_COMPUTENXEXTVALUE,
	DACSTATEMACHINE_COMPLETE
};

void DAC_Display(void *ptrData, const char *msg);
void DACValue_Monitor(void);
void DAC_Triangle_Wave_Monitor(void);
void DAC_SawTooth_Monitor(void);
void DAC_SineWave_Monitor(void);
