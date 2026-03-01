/*
 * CommonMenus.h
 *
 * Created: 11/28/2025 9:22:41 AM
 *  Author: terry
 */

#pragma once
#include "ADC/ADC.h"
#include "Menus/ADC/Menu_ADC.h"
#include "Menu_Report_Processor/Menu_Processor/Menu_Processor.h"
#include "Common/DataStructures.h"

#define LocalPrintBufferSize 25

void CommonMenus_Initialize(void);

// ADC
void ADC_SampleNumberMainMenu(const char *msg);
void ADC_SampleDelayMainMenu(const char *msg);
void ADC_DataBitsMainMenu(const char *msg);
void ADC_PreScaleMainMenu(const char *msg);

// Generic
void EnableDisabledMainMenu(const char *msg);
void PortDirectionMainMenu(const char *msg);
void PortInvertedMainMenu(const char *msg);
void PortResistorsMainMenu(const char *msg);
void OutputHighLowMainMenu(const char *msg);
void PortInputLevelMainMenu(const char *msg);

void VRefMainMenu(const char *msg);

// DAC
void DACModeMainMenu(const char *msg);

bool Menu_Report_Processor_Status(void);

/*
 * Generic dropdown lookup (existing)
 */
char *LookupDropDownByValue(MenuSystem *menuSystem, uint16_t value);
uint16_t LookupDropDownString(MenuSystem *menuSystem, char *msg);

/*
 * Option B: DropdownId-only table-driven access
 */
typedef enum
{
	DROPDOWN_EnableDisabled = 0,
	DROPDOWN_PreScale,
	DROPDOWN_SampleDelay,
	DROPDOWN_Bits,
	DROPDOWN_SampleNumber,
	DROPDOWN_VRef,
	DROPDOWN_DACMode,

	DROPDOWN_CCLInputSource,
	DROPDOWN_ClkSelect,
	DROPDOWN_Waveform,
	DROPDOWN_DutyCycle,

	DROPDOWN_BaudRate,
	DROPDOWN_Parity,
	DROPDOWN_StopBits,
	DROPDOWN_DataBits,
	
	DROPDOWN_InterruptEdges,
	DROPDOWN_EventSystemModes,
	
	DROPDOWN_PortDirection,
	DROPDOWN_PortInverted,
	DROPDOWN_PortResistor,

	DROPDOWN_HighLowValue, 
	DROPDOWN_PortInputLevel,
		
	DROPDOWN_Count
} DropDownId;

char *LookupDropDownByValueId(DropDownId id, uint16_t value);
uint16_t LookupDropDownStringId(DropDownId id, char *msg);

/*
 * Compatibility macros (so you don't have to touch call-sites yet)
 * These replace the old wrapper FUNCTIONS with lightweight macros.
 */
#define LookupEnableDisabledByValue(v)    LookupDropDownByValueId(DROPDOWN_EnableDisabled, (uint16_t)(v))
#define LookupEnableDisabledByString(s)   LookupDropDownStringId(DROPDOWN_EnableDisabled, (s))

#define LookupPreScaleByValue(v)          LookupDropDownByValueId(DROPDOWN_PreScale, (uint16_t)(v))
#define LookupPreScaleByString(s)         LookupDropDownStringId(DROPDOWN_PreScale, (s))

#define LookupSampleDelayByValue(v)       LookupDropDownByValueId(DROPDOWN_SampleDelay, (uint16_t)(v))
#define LookupSampleDelayByString(s)      LookupDropDownStringId(DROPDOWN_SampleDelay, (s))

#define LookupBitsByValue(v)              LookupDropDownByValueId(DROPDOWN_Bits, (uint16_t)(v))
#define LookupBitsByString(s)             LookupDropDownStringId(DROPDOWN_Bits, (s))

#define LookupSampleNumberByValue(v)      LookupDropDownByValueId(DROPDOWN_SampleNumber, (uint16_t)(v))
#define LookupSampleNumberByString(s)     LookupDropDownStringId(DROPDOWN_SampleNumber, (s))

#define LookupVRefByValue(v)              LookupDropDownByValueId(DROPDOWN_VRef, (uint16_t)(v))
#define LookupVRefByString(s)             LookupDropDownStringId(DROPDOWN_VRef, (s))

#define LookupDACModeByValue(v)           LookupDropDownByValueId(DROPDOWN_DACMode, (uint16_t)(v))
#define LookupDACModeByString(s)          LookupDropDownStringId(DROPDOWN_DACMode, (s))

// CCL
#define LookupInputSourceByValue(v)       LookupDropDownByValueId(DROPDOWN_CCLInputSource, (uint16_t)(v))

//TODO: Should convert to Macros
char *CCLInputSourceDecode(void *ptr);
void CCLInputSourceMainMenu(const char *msg);

// clksel
#define LookupClkselModeByValue(v)        LookupDropDownByValueId(DROPDOWN_ClkSelect, (uint16_t)(v))
#define LookupClkselModeByString(s)       LookupDropDownStringId(DROPDOWN_ClkSelect, (s))


//TODO: Should convert to Macros
char *ClkselModeDecode(void *ptr);
void ClkSelectMainMenu(const char *msg);

//TODO: Should convert to Macros
char *WaveformModeDecode(void *ptr);
void WaveformMainMenu(const char *msg);

//TODO: Should convert to Macros
char *DutyCycleModeDecode(void *ptr);
void DutyCycleMainMenu(const char *msg);

//TODO: Should convert to Macros
char *InterruptEdgeDecode(void *ptr);
void InterruptEdgeMainMenu(const char *msg);
#define LookupInterruptEdgeByValue(v)        LookupDropDownByValueId(DROPDOWN_InterruptEdges, (uint16_t)(v))

//TODO: Should convert to Macros
char *EventsSystemModeDecode(void *ptr);
void EventSystemModesMainMenu(const char *msg);
#define EventSystemModesByValue(v)        LookupDropDownByValueId(DROPDOWN_EventSystemModes, (uint16_t)(v))

// Baud Rate
#define LookupBaudRateByValue(v)          LookupDropDownByValueId(DROPDOWN_BaudRate, (uint16_t)(v))
#define LookupBaudRateByString(s)       LookupDropDownStringId(DROPDOWN_BaudRate, (s))

// Parity
#define LookupParityByValue(v)        LookupDropDownByValueId(DROPDOWN_Parity, (uint16_t)(v))
#define LookupParityByString(s)       LookupDropDownStringId(DROPDOWN_Parity, (s))

// Stop Bits
#define LookupStopBitsByValue(v)        LookupDropDownByValueId(DROPDOWN_StopBits, (uint16_t)(v))
#define LookupStopBitsByString(s)       LookupDropDownStringId(DROPDOWN_StopBits, (s))

// Data Bits
#define LookupDataBitsByValue(v)        LookupDropDownByValueId(DROPDOWN_DataBits, (uint16_t)(v))
#define LookupDataBitsByString(s)       LookupDropDownStringId(DROPDOWN_DataBits, (s))

#define LookupPortDirectionByValue(v)   LookupDropDownByValueId(DROPDOWN_PortDirection, (uint16_t)(v))
#define LookupPortDirectionByString(s)  LookupDropDownStringId(DROPDOWN_PortDirection, (s))

#define LookupPortInvertedByValue(v)    LookupDropDownByValueId(DROPDOWN_PortInverted, (uint16_t)(v))
#define LookupPortInvertedByString(s)   LookupDropDownStringId(DROPDOWN_PortInverted, (s))

#define LookupPortResistorByValue(v)    LookupDropDownByValueId(DROPDOWN_PortResistor, (uint16_t)(v))
#define LookupPortResistorByString(s)   LookupDropDownStringId(DROPDOWN_PortResistor, (s))

#define LookupHighLowByValue(v)			LookupDropDownByValueId(DROPDOWN_HighLowValue, (uint16_t)(v))
#define LookupHighLowByString(s)		LookupDropDownStringId(DROPDOWN_HighLowValue, (s))

#define LookupPortInputLevelByValue(v)  LookupDropDownByValueId(DROPDOWN_PortInputLevel, (uint16_t)(v))
#define LookupPortInputLevelByString(s) LookupDropDownStringId(DROPDOWN_PortInputLevel, (s))


void ApplySaveConfigFromParentMenu(struct MenuSystem *editMenu);

void Clear_LocalPrintBuffer(void);


#define LookupClkselModeByValue(v)        LookupDropDownByValueId(DROPDOWN_ClkSelect, (uint16_t)(v))
#define LookupClkselModeByString(s)       LookupDropDownStringId(DROPDOWN_ClkSelect, (s))

// I decided that I want to send limits to the input values.
// Therefore I created specific function for managing the values.
struct EnterValueByHand *GetEnterValuePtr(void);
void EnterNumberValue(void);
char *DecodeFloat(void *ptr);
char *DecodeUint8_t(void *ptr);
char *DecodeUint16_t(void *ptr);
