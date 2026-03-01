/*
 * Menu_ADC.c
 *
 * Created: 11/28/2025 9:23:25 AM
 *  Author: terry
 */ 
#include "CommonStrings/CommonStrings.h"

// Turn each makeStringsWithSize(name,"text") line into:
//   const char name[] = "text";
#define makeStringsWithSize(name, text) \
const char name[] = text;

#include "CommonMsgs_list.h"
#undef makeStringsWithSize
