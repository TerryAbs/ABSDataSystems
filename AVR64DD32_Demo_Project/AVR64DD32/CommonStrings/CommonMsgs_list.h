/**
 * @file    CommonMsgs_list.h
 * @brief   Comprehensive list of PROGMEM string declarations
 * @details Master list of all common strings used throughout the project.
 *          Organized by category for easy reference and maintenance.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par String Categories
 *      - ADC_Messages: ADC channel labels and units
 *      - DAC_Messages: DAC waveform types and settings
 *      - Timer_Messages: Timer mode labels
 *      - SPI_Messages: Flash device names and operations
 *      - TWI_Messages: I2C device types and operations
 *      - Menu_Messages: Menu prompts and navigation
 *      - Error_Messages: Error and warning text
 *      - Status_Messages: Success and info messages
 *
 * @note    Declarations only - definitions in CommonStrings.c
 * @note    All strings use PROGMEM attribute
 * @see CommonStrings.c for string definitions
 */

// This file was originally name with a .def extension.
// In doing so, Intellisense stopped working.
// The work around is to change this file to a h file
// and do not create a #pragma once 

// All Strings appear here.

makeStringsWithSize(msg_TitleBar, "===========================================================")
makeStringsWithSize(msg_Plus, "+")
makeStringsWithSize(msg_MainMenu, "Main Menu")
makeStringsWithSize(msg_EQUAL, "=")
makeStringsWithSize(msg_EQUALLONG, "===========================")

makeStringsWithSize(msg_Main,			"Main")
makeStringsWithSize(msg_INSTANT,		"Instant")
makeStringsWithSize(msg_AVERAGE,		"Average")
makeStringsWithSize(msg_DIFFERENTIAL,	"Differential")


makeStringsWithSize(msg_Cancel,					"Cancel")

makeStringsWithSize(msg_ExitMenu,	"Exit Menu")
makeStringsWithSize(msg_ADCMenu,	"ADC Menu")
makeStringsWithSize(msg_ADCConfigurationMenu,	"ADC Configuration Menu")
makeStringsWithSize(msg_CCLMenu,	"CCL Menu")
makeStringsWithSize(msg_DACMenu,	"DAC Menu")
makeStringsWithSize(msg_EventsMenu,	"Events Menu")
makeStringsWithSize(msg_PortsMenu,	"Ports Menu")
makeStringsWithSize(msg_RTCMenu,	"RTC Menu")
makeStringsWithSize(msg_SPIMenu,	"SPI Menu")
makeStringsWithSize(msg_TimersMenu,	"Timers Menu")
makeStringsWithSize(msg_TWIMenu,	"TWI Menu")
makeStringsWithSize(msg_USARTMenu,	"USART Menu")
makeStringsWithSize(msg_ZCDMenu,	"ZCD Menu")

makeStringsWithSize(msg_ADC,	"ADC")
makeStringsWithSize(msg_CCL,	"CCL")
makeStringsWithSize(msg_DAC,	"DAC")
makeStringsWithSize(msg_Events,	"Events")
makeStringsWithSize(msg_Ports,	"Ports")
makeStringsWithSize(msg_RTC,	"RTC")
makeStringsWithSize(msg_SPI,	"SPI")
makeStringsWithSize(msg_Timers,	"Timers")
makeStringsWithSize(msg_TWI,	"TWI")
makeStringsWithSize(msg_USART,	"USART")
makeStringsWithSize(msg_ZCD,	"ZCD")

makeStringsWithSize(msg_Num_1,	"1")
makeStringsWithSize(msg_Num_2,	"2")
makeStringsWithSize(msg_Num_3,	"3")
makeStringsWithSize(msg_Num_4,	"4")
makeStringsWithSize(msg_Num_5,	"5")
makeStringsWithSize(msg_Num_6,	"6")
makeStringsWithSize(msg_Num_7,	"7")
makeStringsWithSize(msg_Num_8,	"8")
makeStringsWithSize(msg_Num_9,	"9")

makeStringsWithSize(msg_LTR_A,	"A")
makeStringsWithSize(msg_LTR_B,	"B")
makeStringsWithSize(msg_LTR_C,	"C")
makeStringsWithSize(msg_LTR_D,	"D")
makeStringsWithSize(msg_LTR_E,	"E")
makeStringsWithSize(msg_LTR_F,	"F")
makeStringsWithSize(msg_LTR_G,	"G")
makeStringsWithSize(msg_LTR_H,	"H")
makeStringsWithSize(msg_LTR_I,	"I")
makeStringsWithSize(msg_LTR_J,	"J")
makeStringsWithSize(msg_LTR_K,	"K")
makeStringsWithSize(msg_LTR_L,	"L")
makeStringsWithSize(msg_LTR_M,	"M")
makeStringsWithSize(msg_LTR_N,	"N")
makeStringsWithSize(msg_LTR_O,	"O")
makeStringsWithSize(msg_LTR_P,	"P")
makeStringsWithSize(msg_LTR_X,	"X")

makeStringsWithSize(msg_ADCPresScaleMenu, "ADC Pre-scale Select")
makeStringsWithSize(msg_ADC_PRESC_DIV2_gc,		"Div 2")
makeStringsWithSize(msg_ADC_PRESC_DIV4_gc,		"Div 4") 
makeStringsWithSize(msg_ADC_PRESC_DIV8_gc,		"Div 8") 
makeStringsWithSize(msg_ADC_PRESC_DIV12_gc,		"Div 12")
makeStringsWithSize(msg_ADC_PRESC_DIV16_gc,		"Div 16")
makeStringsWithSize(msg_ADC_PRESC_DIV20_gc,		"Div 20")
makeStringsWithSize(msg_ADC_PRESC_DIV24_gc,		"Div 24")
makeStringsWithSize(msg_ADC_PRESC_DIV28_gc,		"Div 28")
makeStringsWithSize(msg_ADC_PRESC_DIV32_gc,		"Div 32")
makeStringsWithSize(msg_ADC_PRESC_DIV48_gc,		"Div 48")
makeStringsWithSize(msg_ADC_PRESC_DIV64_gc,		"Div 64")
makeStringsWithSize(msg_ADC_PRESC_DIV96_gc,		"Div 96")
makeStringsWithSize(msg_ADC_PRESC_DIV128_gc,	"Div 128")
makeStringsWithSize(msg_ADC_PRESC_DIV256_gc,	"Div 256")

makeStringsWithSize(msg_ADCSampleDelayMenu, "ADC Sample Delay Select")
makeStringsWithSize(msg_ADC_SAMPDLY_DLY0_gc,	"Delay 0")
makeStringsWithSize(msg_ADC_SAMPDLY_DLY1_gc,	"Delay 1")
makeStringsWithSize(msg_ADC_SAMPDLY_DLY2_gc,	"Delay 2")
makeStringsWithSize(msg_ADC_SAMPDLY_DLY3_gc,	"Delay 3") 
makeStringsWithSize(msg_ADC_SAMPDLY_DLY4_gc,	"Delay 4") 
makeStringsWithSize(msg_ADC_SAMPDLY_DLY5_gc,	"Delay 5") 
makeStringsWithSize(msg_ADC_SAMPDLY_DLY6_gc,	"Delay 6") 
makeStringsWithSize(msg_ADC_SAMPDLY_DLY7_gc,	"Delay 7") 
makeStringsWithSize(msg_ADC_SAMPDLY_DLY8_gc,	"Delay 8") 
makeStringsWithSize(msg_ADC_SAMPDLY_DLY9_gc,	"Delay 9") 
makeStringsWithSize(msg_ADC_SAMPDLY_DLY10_gc,	"Delay 10")
makeStringsWithSize(msg_ADC_SAMPDLY_DLY11_gc,	"Delay 11")
makeStringsWithSize(msg_ADC_SAMPDLY_DLY12_gc,	"Delay 12")
makeStringsWithSize(msg_ADC_SAMPDLY_DLY13_gc,	"Delay 13")
makeStringsWithSize(msg_ADC_SAMPDLY_DLY14_gc,	"Delay 14")
makeStringsWithSize(msg_ADC_SAMPDLY_DLY15_gc,	"Delay 15")


makeStringsWithSize(msg_ADC_DataBitsMenu,	"ADC Data Bit Select")
makeStringsWithSize(msg_ADC_RESSEL_10BIT_gc,	"ADC 10 Bits")
makeStringsWithSize(msg_ADC_RESSEL_12BIT_gc,	"ADC 12 Bits")

makeStringsWithSize(msg_ADCSampleNumberMenu, "ADC Sample Number Select")
makeStringsWithSize(msg_ADC_SAMPNUM_NONE_gc,	"Number 0")
makeStringsWithSize(msg_ADC_SAMPNUM_ACC2_gc,	"Number 2")
makeStringsWithSize(msg_ADC_SAMPNUM_ACC4_gc,	"Number 4")
makeStringsWithSize(msg_ADC_SAMPNUM_ACC8_gc,	"Number 8")
makeStringsWithSize(msg_ADC_SAMPNUM_ACC16_gc,	"Number 16")
makeStringsWithSize(msg_ADC_SAMPNUM_ACC32_gc,	"Number 32")
makeStringsWithSize(msg_ADC_SAMPNUM_ACC64_gc,	"Number 64")
makeStringsWithSize(msg_ADC_SAMPNUM_ACC128_gc,	"Number 128")

makeStringsWithSize(msg_VrefMenu, "VRef Select")
makeStringsWithSize(msg_VREF_REFSEL_1V024_gc,	"Vref 1.024V")
makeStringsWithSize(msg_VREF_REFSEL_2V048_gc,	"Vref 2.048V")
makeStringsWithSize(msg_VREF_REFSEL_4V096_gc,	"Vref 4.096V")
makeStringsWithSize(msg_VREF_REFSEL_2V500_gc,	"Vref 2.500V")
makeStringsWithSize(msg_VREF_REFSEL_VDD_gc,		"Vref VDD")
makeStringsWithSize(msg_VREF_REFSEL_VREFA_gc,	"Vref External")

makeStringsWithSize(msg_ENABLE,					"Enable")
makeStringsWithSize(msg_REFRANGE,				"Ref Range")
makeStringsWithSize(msg_SAMPNUM,				"Sample No")
makeStringsWithSize(msg_SAMPLEDLY,				"Sample Delay")
makeStringsWithSize(msg_BITS,					"No of Bits")
makeStringsWithSize(msg_PRESCALE,				"Prescale")


makeStringsWithSize(msg_DACMODEMenu,	"DAC Mode Select")
makeStringsWithSize(msg_DACMODE,				"DAC Mode")
makeStringsWithSize(msg_SETVALUE,				"Set Value")
makeStringsWithSize(msg_SINEWAVE,				"Sine Wave")
makeStringsWithSize(msg_SAWTOOHWAVE,			"Saw Tooth Wave")
makeStringsWithSize(msg_TRIANGLEWAVE,			"Triangle Wave")
makeStringsWithSize(msg_PEAK,					"Peak")
makeStringsWithSize(msg_OFFSET,					"Offset")
makeStringsWithSize(msg_STEPS,					"Steps")

makeStringsWithSize(msg_EEPROM_NO_DATA,		"EEPROM No Data")
makeStringsWithSize(msg_EEPROM_WRITE_FAILED,"EEPROM Write Failed")
makeStringsWithSize(msg_EEPROM_TIMEDOUT,	"EEPROM TimedOut")
makeStringsWithSize(msg_EEPROM_INVALIDSIZE,	"EEPROM Invalid Size")
makeStringsWithSize(msg_EEPROM_ERASEFAILED,	"EEPROM Erase Failed")
makeStringsWithSize(msg_EEPROM_FAILED_STATUS,"EEPROM Failed Status Check")
makeStringsWithSize(msg_ENTERVALUE,				"Enter Value")

makeStringsWithSize(msg_InvalidEntry, "Value entered is invalid")
makeStringsWithSize(msg_InvalidRange, "Value entered is not with in Range")


// CCL
makeStringsWithSize(msg_CCL_INPUT,	"Select CCL Inputs")
makeStringsWithSize(msg_CCL_INPUT_A, "Input A")
makeStringsWithSize(msg_CCL_INPUT_A_B, "Input A + B")
makeStringsWithSize(msg_CCL_INPUT_A_B_C, "Input A + B + C")
makeStringsWithSize(msg_INPUTSOURCE,	"Input Source")
makeStringsWithSize(msg_PATTERN,	"Pattern")

makeStringsWithSize(msg_Passed, "Passed")
makeStringsWithSize(msg_Failed, "Failed")
		
makeStringsWithSize(msg_TWII2C_WriteError, "TWI Write Error")
makeStringsWithSize(msg_TWII2C_ACKError, "TWI Ack Error")

makeStringsWithSize(msgSPI_TimedOut, "SPI TimedOut")

// Timer ClkSel
makeStringsWithSize(msg_ClkSelMode,			"CLKSel")
makeStringsWithSize(msg_ClkSelMenu, "Clock Select Menu")
makeStringsWithSize(msg_ClkSEL_DIV1_gc,		"Div 1")
makeStringsWithSize(msg_ClkSEL_DIV2_gc,		"Div 2")
makeStringsWithSize(msg_ClkSEL_DIV4_gc,		"Div 4")
makeStringsWithSize(msg_ClkSEL_DIV8_gc,		"Div 8")
makeStringsWithSize(msg_ClkSEL_DIV16_gc,	"Div 16")
makeStringsWithSize(msg_ClkSEL_DIV64_gc,	"Div 64")
makeStringsWithSize(msg_ClkSEL_DIV256_gc,	"Div 256")
makeStringsWithSize(msg_ClkSEL_DIV1024_gc,	"Div 1024")

makeStringsWithSize(msg_WaveformMode,			"Waveform")
makeStringsWithSize(msg_WaveformMenu,			"Waveform Select Menu")
makeStringsWithSize(msg_Waveform_Normal,		"Normal")
makeStringsWithSize(msg_Waveform_Frequency,		"Frequency")
makeStringsWithSize(msg_Waveform_SingleSlope,		"Single Slope PWM")
makeStringsWithSize(msg_Waveform_DualPWMTop,		"Dual Slope PWM Top")
makeStringsWithSize(msg_Waveform_DualPWMTopBottom,	"Dual Slope PWM Top/Bottom")
makeStringsWithSize(msg_Waveform_DualPWMBottom,		"Dual Slope PWM Bottom")
makeStringsWithSize(msg_PeriodMode, "Period")
makeStringsWithSize(msg_CompareMode, "Compare")

makeStringsWithSize(msg_ADC_Description,	"Analog to Digital Conversion")
makeStringsWithSize(msg_CCL_Description,	"Configurable Custom Logic Look-up Table")
makeStringsWithSize(msg_DAC_Description,	"Digital to Analog Conversion")
makeStringsWithSize(msg_Events_Description,	"Events Systems")
makeStringsWithSize(msg_Ports_Description,	"Ports")
makeStringsWithSize(msg_RTC_Description,	"Real Time Clock")
makeStringsWithSize(msg_SPI_Description,	"Serial Peripheral Interface")
makeStringsWithSize(msg_Timers_Description,	"System Timer (TCA 16 Bit)")
makeStringsWithSize(msg_TWI_Description,	"Two Wire Interface (I2C)")
makeStringsWithSize(msg_USART_Description,	"Universal Synchronous and Asynchronous Rec and Xmit")
makeStringsWithSize(msg_ZCD_Description,	"Zero-Crossing Detector")

makeStringsWithSize(msg_FrequencyMode,			"Frequency")
makeStringsWithSize(msg_FrequencyMenu,			"Frequency Menu")


makeStringsWithSize(msg_DutyCycleMode,			"Duty Cycle")
makeStringsWithSize(msg_DutyCycleMenu,			"Duty Cycle Menu")
makeStringsWithSize(msg_DutyCycleList_10,		"10%")
makeStringsWithSize(msg_DutyCycleList_20,		"20%")
makeStringsWithSize(msg_DutyCycleList_30,		"30%")
makeStringsWithSize(msg_DutyCycleList_40,		"40%")
makeStringsWithSize(msg_DutyCycleList_50,		"50%")
makeStringsWithSize(msg_DutyCycleList_60,		"60%")
makeStringsWithSize(msg_DutyCycleList_70,		"70%")
makeStringsWithSize(msg_DutyCycleList_80,		"80%")
makeStringsWithSize(msg_DutyCycleList_90,		"90%")


makeStringsWithSize(msg_BaudRate,	"Baud Rate")
makeStringsWithSize(msg_Parity,		"Parity")
makeStringsWithSize(msg_StopBits,	"Stop Bits")
makeStringsWithSize(msg_DataBits,	"Data Bits")

makeStringsWithSize(msg_BaudRateMenu,	"Select Baud Rate")
makeStringsWithSize(msg_ParityMenu,		"Select Parity")
makeStringsWithSize(msg_StopBitsMenu,	"Select Stop Bits")
makeStringsWithSize(msg_DataBitsMenu,	"Select Data Bits")

makeStringsWithSize(msg_None,		"None")
makeStringsWithSize(msg_Even,		"Even")
makeStringsWithSize(msg_Odd,		"Odd")

makeStringsWithSize(msgBaudRate_300,		"300")
makeStringsWithSize(msgBaudRate_600, 		"600")
makeStringsWithSize(msgBaudRate_1200, 		"1200") 
makeStringsWithSize(msgBaudRate_2400, 		"2400")
makeStringsWithSize(msgBaudRate_4800, 		"4800")
makeStringsWithSize(msgBaudRate_9600, 		"9600")
makeStringsWithSize(msgBaudRate_14400, 		"14400") 
makeStringsWithSize(msgBaudRate_19200,		"19200")
makeStringsWithSize(msgBaudRate_28800, 		"28800")
makeStringsWithSize(msgBaudRate_38400, 		"38400")
makeStringsWithSize(msgBaudRate_57600, 		"57600")
makeStringsWithSize(msgBaudRate_115200, 	"115200")
makeStringsWithSize(msgBaudRate_230400, 	"230400")
makeStringsWithSize(msgBaudRate_460800, 	"460800")
makeStringsWithSize(msgBaudRate_921600,		"921600")

makeStringsWithSize(msg_LoopDelay,		"Loop Delay")

makeStringsWithSize(msg_InterruptEdge,			"Interrupt Edge")
makeStringsWithSize(msg_InterruptEdgeMenu,		"Interrupt Edge Menu")
makeStringsWithSize(msg_EdgesRising,	"Rising Edge")
makeStringsWithSize(msg_EdgesFalling,	"Falling Edge")
makeStringsWithSize(msg_EdgesBoth,		"Both Edges")

makeStringsWithSize(msg_EventSystemModes,		"Events System")
makeStringsWithSize(msg_EventSystemModesMenu,	"Events System Menu")
makeStringsWithSize(msg_EventModeADC,	"ADC 0")
makeStringsWithSize(msg_EventModeCCL,	"CCL")
makeStringsWithSize(msg_EventModeRTC,	"RTC")
makeStringsWithSize(msg_EventModeZCD,	"ZCD")

makeStringsWithSize(msg_EventModeADCError,	"ADC 0")
makeStringsWithSize(msg_EventModeCCLCError,	"CCL")
makeStringsWithSize(msg_EventModeRTCCError,	"RTC")
makeStringsWithSize(msg_EventModeZCDCError,	"ZCD")


makeStringsWithSize(msg_ENABLEDMenu,	"Enabled Select")
makeStringsWithSize(msg_ENABLED,		"Enabled")
makeStringsWithSize(msg_DISABLED,		"Disabled")

makeStringsWithSize(msg_PortDirectionMenu,	"Port Direction")
makeStringsWithSize(msg_PortDirectionPC3,		"PC3 Direction")
makeStringsWithSize(msg_PortDirection_Input,		"Input")
makeStringsWithSize(msg_PortDirection_Output,		"Output")

makeStringsWithSize(msg_PortInvertedMenu,	"Port Inverted")
makeStringsWithSize(msg_PortInvertedPC3,	"Port PC3 Inverted")
makeStringsWithSize(msg_PortInvertedPF6,	"Port PF6 Inverted")
makeStringsWithSize(msg_PortInvertedInverted,	"Inverted")
makeStringsWithSize(msg_PortInvertedNotInveted,	"Not Inverted")

makeStringsWithSize(msg_PortPullUpResistorMenu,	"PullUp Resistor")
makeStringsWithSize(msg_PortPullUpResistoPC3,	"PC3 Pullup Resistor")
makeStringsWithSize(msg_PortPullUpResistoPF6,	"PF6 Pullup Resistor")
makeStringsWithSize(msg_PortPullUpResistorPullUpEnabled,	"Pullup Enabled")
makeStringsWithSize(msg_PortPullUpResistorPullUpDisabled,	"Pullup Disabled")

makeStringsWithSize(msg_PortInputLevelMenu,	"Input Level")
makeStringsWithSize(msg_PortInputLevelPC3,	"PC3 Input Level")
makeStringsWithSize(msg_PortInputLevelPF6,	"PF6 InputLevel")
makeStringsWithSize(msg_PortInputLevelSchmitt,	"Schmitt Trigger")
makeStringsWithSize(msg_PortInputLevelTTL,	"TTL")

makeStringsWithSize(msg_OutputLevelMenu,	"Output Level")
makeStringsWithSize(msg_OutputDefault,	"Default Output")
makeStringsWithSize(msg_OutputLevel_LOW,	"Low")
makeStringsWithSize(msg_OutputLevel_HIGH,	"High")