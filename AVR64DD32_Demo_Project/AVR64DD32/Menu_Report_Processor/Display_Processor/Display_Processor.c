/*
 * Display_Processor.c
 *
 * Created: 12/20/2025 8:11:04 AM
 *  Author: terry
 */ 

// The display processor for now is not going to be 
// anything fancy, just a list of call to make to 
// potential display routines

#include "Display_Processor/Display_Processor.h"
#include "Common/DataStructures.h"
#include "Menu_Report_Processor/Terminal_Commands/Terminal_Commands.h"

static DisplayProcessorMap displayProcessorMap[MAXNODISPLAYPROCESSORS];
static enum DISPLAYPROCESSORSTATES displayProcessor_StateMachine;
static uint8_t displayProcessor_Index;
static uint8_t displayProcessor_Slot;
static uint8_t displayProcessor_Row;

void DisplayProcessor_Initialize(void)
{
	displayProcessor_StateMachine = DISPLAYPROCESSORSTATES_PRINT;
	displayProcessor_Index = 0;
	displayProcessor_Slot = 0;
	displayProcessor_Row = 1;
}

void DisplayProcessor_Monitor(void)
{
	switch(displayProcessor_StateMachine)
	{
		case DISPLAYPROCESSORSTATES_PRINT:
			if((displayProcessorMap[displayProcessor_Index].funcptrPrint != NULL) && (displayProcessorMap[displayProcessor_Index].enabled))
			{
				PrintAt(displayProcessor_Row, DISPLAYPROCESSOR_DISPLAYCOL);
				displayProcessorMap[displayProcessor_Index].funcptrPrint(displayProcessorMap[displayProcessor_Index].ptrData, displayProcessorMap[displayProcessor_Index].msg);
				displayProcessor_Row++;
			}
			displayProcessor_StateMachine = DISPLAYPROCESSORSTATES_LOOP;
			break;
		case DISPLAYPROCESSORSTATES_LOOP:
			// Move to the next record
			displayProcessor_Index++;
			// Test for end of the list.
			if(displayProcessor_Index == MAXNODISPLAYPROCESSORS)
			{				
				displayProcessor_Index = 0;
				displayProcessor_Row = 1;
			}
			displayProcessor_StateMachine = DISPLAYPROCESSORSTATES_PRINT;
			break;
	}
}

void DisplayProcessor_Add(struct DisplayProcessorMap item)
{
	// We can only add if there is room.
	// Again, when we update the code, we can make this more dynamic.
	if(displayProcessor_Slot < MAXNODISPLAYPROCESSORS)
	{
		displayProcessorMap[displayProcessor_Slot].funcptrPrint = item.funcptrPrint;
		displayProcessorMap[displayProcessor_Slot].ptrData = item.ptrData;
		displayProcessorMap[displayProcessor_Slot].msg = item.msg;
		displayProcessorMap[displayProcessor_Slot].enabled = item.enabled;
		displayProcessor_Slot++;
	}
}


void DisplayProcessor_SetEnabledFlag(void *ptrData, bool enabled)
{
	for(int i = 0; i < displayProcessor_Slot; i++)
	{
		if(displayProcessorMap[i].ptrData == ptrData)
		{
			displayProcessorMap[i].enabled = enabled;
			break;
		}
	}
}


