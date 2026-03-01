/*
 * Menu_Processor.c
 *
 * Created: 12/06/2025 8:46:05 AM
 *  Author: terry
 */ 
#include "Menu_Report_Processor/Menu_Processor/Menu_Processor.h"
#include "Delay/Delay.h"
#include "Menu_Report_Processor/Themes/Themes.h"
#include "USART/USART.h"

void ClearTerminal(void)
{
	// clear screen and position the cursor a the home position
	printf("\x1b[2J");
	//MicroDelay(10);      // Give time for response
}

void ClearToEndOfLine(void)
{
	printf("\x1b[K");
	//MicroDelay(10);      // Give time for response
}

void CursorHome(void)
{
	printf("\x1b[H");
	//MicroDelay(10);      // Give time for response
}

void ClearArea(uint8_t sRow, uint8_t eRow, uint8_t sCol, uint8_t eCol)
{
	for(int i = sRow; i < eRow; i++)
	{
		PrintAt(i, sCol);
		for(int j = sCol; j < eCol; j++)
		{
			printf(" ");		
		}
	}	
}

void CursorOff(void)
{
	printf("\x1b[?25l");
}

void CursorOn(void)
{
	printf("\x1b[?25h");
}

void PrintAt(uint8_t row, uint8_t col)
{
	printf("\x1b[%u;%uH", row, col);
}

void SetColor(uint8_t foreground, uint8_t background)
{
	printf("\x1b[3%cm",foreground);
	printf("\x1b[4%cm", background);
}

void PrintNormal(void)
{
	printf("\x1b[0m");
}

void CommandPrompt(void)
{
	ClearArea(TerminalPrompt, TerminalPrompt, 0, TerminalMaxCols);
	SetColor(themeYellowBlack->foreColor, themeYellowBlack->backGroundColor);
	PrintAt(TerminalPrompt, 0);
	printf("-> %s", GetReceiveBuffer());
	CursorOn();
}

//TODO: Need to Test and restore cursor position
#define tempBufferSize 100
static char TempBuffer[tempBufferSize ];

void LogErrorToScreen(const char *msg)
{
	ClearArea(TerminalErrorLog, TerminalErrorLog, 0, TerminalMaxCols);
	SetColor(themeBlackRed->foreColor, themeBlackRed->backGroundColor);
	PrintAt(TerminalErrorLog, 0);
	memset(TempBuffer, 1, tempBufferSize);
	sprintf_P(TempBuffer, PSTR("Error: %S"), msg);
	printf("%s", TempBuffer);
	USART_0_Print_Immediate(TempBuffer);
}