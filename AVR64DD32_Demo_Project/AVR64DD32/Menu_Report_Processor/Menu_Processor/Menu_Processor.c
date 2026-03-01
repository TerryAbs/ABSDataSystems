/*
 * Menu_Processor.c
 *
 * Created: 12/06/2025 8:46:05 AM
 *  Author: terry
 */ 
#include <avr/pgmspace.h>
#include <string.h>
#include "Menu_Processor.h"
#include "CommandList/CommandList.h"

static struct SystemMenuProcessor systemMenuProcessor;
static struct MenuSystem *activeMenu;

void Menu_Processor_Initialize(void)
{
	systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_IDLE;
}

void SetActiveMenu(MenuSystem *menu)
{
	// DropDown are special cases.
	// They only have a valid selection option
	// The way the return back to the host menu
	// is by setting the calling menu value
	if(menu->dataEntryMode == DATAENTRYMODE_DROPDOWN)
	{
		menu->callingMenu = activeMenu;
	}

	activeMenu = menu;
	Menu_DisplayMenu();
}

void Menu_DisplayMenu(void)
{
	SetCommandMode(COMMANDMODE_MENU);
	if(activeMenu != NULL)
	{
		systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_START;
	}
}

void *GetDataPtrForMenuItem(const char *msg)
{
	void *results = NULL;
	int index = 0;
	MenuBody mb;
	DataEntry de;
	
	for(int i = 0; i < activeMenu->menuBodySize; i++)
	{
		MenuBody_Read(&mb, activeMenu->menuBody, i);
		index = strncasecmp_P(msg, mb.cmd_Msg_P, mb.cmdSize);
		// Test to make sure at least the first x match.
		if(index == 0 )
		{	
			index = mb.row;
			for(int j = 0; j < activeMenu->dataEntrySize; j++)
			{
				MenuDataEntry_Read(&de, activeMenu->dataEntry, j);
				if(de.row == index)
				{
					results = activeMenu->dataptr + de.offset;
					break;
				}
			}
			break;			
		}
	}
	return results;
}

const struct MenuSystem *GetActiveMenu(void)
{
	return activeMenu;
}

// Are we still printing the menu?
bool Menu_Processor_Status(void)
{
	bool results = true;
	if (systemMenuProcessor.StateMachine != SYSTEMMENU_PROCESSING_STATE_IDLE)
	{
		results = false;
	}
	return results;
}

void MenuSystem_Processor_Monitor(void)
{
	uint8_t centeredCol = 0;
	MenuBody mb;
	MenuHeader mh;
	DataEntry de;
	const char *msg; 
	
	switch(systemMenuProcessor.StateMachine)
	{
		case SYSTEMMENU_PROCESSING_STATE_IDLE:
			break;
		case SYSTEMMENU_PROCESSING_STATE_START:
			if(activeMenu->menuHeader == NULL)
			{
				systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_BODY;
			}
			else
			{
				//
				CursorOff();
				switch(activeMenu->dataEntryMode)
				{
					
					case DATAENTRYMODE_DROPDOWN:
					case DATAENTRYMODE_ENTERVALUE:
						SetColor(activeMenu->screenArea.theme->foreColor, activeMenu->screenArea.theme->backGroundColor);
						ClearArea(activeMenu->screenArea.sRow, activeMenu->screenArea.eRow, activeMenu->screenArea.sCol, activeMenu->screenArea.eCol);
						break;
					default:
						ClearTerminal();					
							break;
				}
				systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_HEADER;
				systemMenuProcessor.currentRow = 0;			
			}
			break;
		case SYSTEMMENU_PROCESSING_STATE_HEADER:
			MenuHeader_Read(&mh, activeMenu->menuHeader, systemMenuProcessor.currentRow);
			SetColor(mh.theme->foreColor, mh.theme->backGroundColor);
			// Special rules only apply if the starting col = 0;
			// Automatically DrawLines
			// if the title has 1 character, its assumed to draw the max number of characters
			if(mh.col == 0)
			{
				if(mh.cmd_MsgSize == 1) 
				{
					PrintAt(mh.row, 0);
					for(int i = 0; i < TerminalMaxCols; i++)
					{
						printf_P(PSTR("%S"), mh.cmd_Msg_P);	
					}					
				}
				else
				{
					// Automatically Center Column Text
					centeredCol = TerminalColCenter - (mh.cmd_MsgSize /2);
					PrintAt(mh.row, centeredCol);

					printf_P(PSTR("%S"), mh.cmd_Msg_P);	
				}
			}
			else
			{
				PrintAt(mh.row, mh.col);
				printf_P(PSTR("%S"), mh.cmd_Msg_P);
			}
			
			systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_HEADER_TEST;
			break;
			
		case SYSTEMMENU_PROCESSING_STATE_HEADER_TEST:
			systemMenuProcessor.currentRow++;
			if(systemMenuProcessor.currentRow < activeMenu->menuHeaderSize)
			{
				systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_HEADER;
			}
			else
			{
				if(activeMenu->menuBody != NULL)
				{
					systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_BODY;
					systemMenuProcessor.currentRow = 0;
				}
				else
				{
					systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_DISPLAYPROMPT;	
				}
			}
			break;
		case SYSTEMMENU_PROCESSING_STATE_BODY:
			MenuBody_Read(&mb, activeMenu->menuBody, systemMenuProcessor.currentRow);
			SetColor(mb.theme->foreColor, mb.theme->backGroundColor);
			PrintAt(mb.row, mb.col);
			printf_P(PSTR("%S\t"), mb.cmd_Msg_P);
			printf_P(PSTR("(%S)"), mb.description_P);
			systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_BODY_TEST;
			break;
		case SYSTEMMENU_PROCESSING_STATE_BODY_TEST:
			systemMenuProcessor.currentRow++;
			if(systemMenuProcessor.currentRow < activeMenu->menuBodySize)
			{
				systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_BODY;
			}
			else
			{
				if(activeMenu->dataEntryMode == DATAENTRYMODE_MENU)
				{
					systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_DISPLAYPROMPT;
				}
				else
				{
					systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_DATAENTRY;
					systemMenuProcessor.currentRow = 0;
				}			
			}
			break;
		case SYSTEMMENU_PROCESSING_STATE_DATAENTRY:
			MenuDataEntry_Read(&de, activeMenu->dataEntry, systemMenuProcessor.currentRow);
			SetColor(de.theme->foreColor, de.theme->backGroundColor);
			PrintAt(de.row, de.col);
			// The Decodefunctptr points to a function that understand how to display the data.
			// Make sure there is a function prior to called.
			if(de.Decodefuncptr != NULL)
			{
				msg = de.Decodefuncptr(activeMenu->dataptr + de.offset);
				if(msg != NULL)
				{
					// Compute the data location and pass it on to the function
			 		printf_P(PSTR("%S"), msg);
				}
			}
			
			systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_DATAENTRY_TEST;
			break;
		case SYSTEMMENU_PROCESSING_STATE_DATAENTRY_TEST:
			systemMenuProcessor.currentRow++;
			if(systemMenuProcessor.currentRow < activeMenu->dataEntrySize)
			{
				systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_DATAENTRY;
			}
			else
			{
				systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_DISPLAYPROMPT;
			}
			break;
		case SYSTEMMENU_PROCESSING_STATE_DISPLAYPROMPT:
			CommandPrompt();
			// All done. Return to the idle state until someone else
			// what a menu displayed.
			systemMenuProcessor.StateMachine = SYSTEMMENU_PROCESSING_STATE_IDLE;
			break;
	}
}
