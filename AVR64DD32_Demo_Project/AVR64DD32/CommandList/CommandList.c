/**
	\file
*/ 

#include "CommandList.h"
#include "Common/Common.h"
#include "Common/DataStructures.h"
#include "CommonStrings/CommonStrings.h"
#include "Themes/Themes.h"
#include "Delay/Delay.h"
#include <stdio.h>
#include <ctype.h>


// TODO: Need to rethink this code
// the value of ten represent
#define PROGMEMBUFFERSIZE 20
static char ProgmemBuffer[PROGMEMBUFFERSIZE + 1];

const char *ReadFromPROGMEM(PGM_P msg)
{
	memset(ProgmemBuffer, 0, PROGMEMBUFFERSIZE);
	memcpy_P(ProgmemBuffer, msg, strlen_P(msg));
	return ProgmemBuffer;
}


static enum COMMANDMODE commandMode;

void SetCommandMode(enum COMMANDMODE mode)
{
	commandMode = mode;
};

void ProcessCommandFromMenus(char *buf)
{
	int index;
	uint8_t value = 0;
	bool processed = false;
	const struct MenuSystem *activeMenu = GetActiveMenu();
	
	MenuBody mb;
	DataEntry de;
	
	for(int i = 0; i < activeMenu->menuBodySize; i++)
	{
		MenuBody_Read(&mb, activeMenu->menuBody, i);
		index = strncasecmp_P(buf, mb.cmd_Msg_P, mb.cmdSize);
		// Test to make sure at least the first x match.
		if(index == 0 )
		{
			switch(activeMenu->dataEntryMode)
			{
				case DATAENTRYMODE_MENU:
				case DATAENTRYMODE_DISPLAYDATA:
					if(mb.funcptr != NULL)
					{
						//mb.funcptr(mb.cmd_Msg_P);
						mb.funcptr(ReadFromPROGMEM(mb.cmd_Msg_P));
					}
					break;
				case DATAENTRYMODE_DROPDOWN:			
				case DATAENTRYMODE_ENTERVALUE:			// This is to make the compiler happy. We are by passing this code 
					// This is where the menu system gets complicate.
					// the Body and the Data Entries must match 1:1 with row numbers
					// The row number provides the index match to the corresponding row in the data table
					// 
					// The Data entry contains the value from the drop down that we want to 
					// get and place at the provided memory location.
					
					// Setup up calling menu or the return menu
					if(activeMenu->callingMenu != NULL)
					{
						// Return back to the calling menu
						SetActiveMenu(activeMenu->callingMenu);	
					}
					
					// DropDowns have a common exit key = 'x'
					// The X tell the dropdown to cancel.
					
					if(toupper(buf[0]) != 'X')
					{
						index = mb.row;
						// This menu is a little more complicated as we need to look up the call from the DataEntryMenu
						for(int j = 0; i < activeMenu->dataEntrySize; j++)
						{
							MenuDataEntry_Read(&de, activeMenu->dataEntry, j);
							// Search for matching record
							if(de.row == index)
							{
								// This location contains the value.
								value = de.offset;

								// Got it. 
								// Get the value from the list
								// The calling menu contains the location
								// of where to write the results
								switch(de.fieldType)
								{
									case FIELDTYPE_BOOL:
										*(bool *)activeMenu->dataptr = (bool)value;
										break;
									case FIELDTYPE_UINT8:
										*(uint8_t *)activeMenu->dataptr = value;
										break;
									case FIELDTYPE_UINT16:
										*(uint16_t *)activeMenu->dataptr = value;
										break;
									case FIELDTYPE_FLOAT:
										*(float *)activeMenu->dataptr = value;
										break;
								}	
								// Call the save function
								if(activeMenu->saveConfig.Savefuncptr != NULL)							
								{
									activeMenu->saveConfig.Savefuncptr(activeMenu->saveConfig.index);
								}
								break;					
							}						
						}
					}
					break;
			}
			processed = true;
			break;
		}
	}
	if(!processed)
	{
		Menu_DisplayMenu();
	}
}

// I'm still using the menu structure, 
// just a little differently that originally designed.
// I should probably refactor the code.
// But for now, I want to complete this project.
// TODO: Rethink this method

void ProcessCommandFromValueEntered(char *buf)
{
	float value = 0.0;
	struct EnterValueByHand *enterValue = GetEnterValuePtr();
	
	if(sscanf(buf, "%f", &value) == 0)
	{
		LogErrorToScreen(msg_InvalidEntry);
	}
	else
	{
		if((value >= enterValue->Min) &&  (value <= enterValue->Max))
		{
			if(enterValue->ptrSaveFunction != NULL)
			{
				enterValue->ptrSaveFunction(value);
			}
		}
		else
		{
			LogErrorToScreen(msg_InvalidRange);
		}
	}
	Menu_DisplayMenu();
}

void Command_Process_Monitor(void)
{
	// Make sure we are not still printing the menu
	if(Menu_Processor_Status())
	{
		// Is there a command to be processed
		if(GetCmdReady_Status())
		{
			switch(commandMode)
			{
				case COMMANDMODE_MENU:
					ProcessCommandFromMenus(GetReceiveBuffer());
					break;
				case COMMANDMODE_VALUE:
					ProcessCommandFromValueEntered(GetReceiveBuffer());
					break;
			}
			Flush_ReceiveBuffer();
		}
	}
}
