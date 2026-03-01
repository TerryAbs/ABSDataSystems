/**
 * @file    Terminal_Commands.h
 * @brief   VT102 terminal control command library
 * @details Provides functions for VT102/ANSI terminal control including cursor
 *          positioning, screen clearing, color setting, and attribute control.
 *          Enables creation of sophisticated terminal-based user interfaces.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Supported Terminal Commands
 *      - Cursor positioning (absolute and relative)
 *      - Screen clearing (full, partial, line)
 *      - Foreground and background colors (16 colors)
 *      - Text attributes (bold, underline, reverse)
 *      - Cursor visibility control
 *      - Screen save/restore
 *
 * @par Color Palette
 *      - Black, Red, Green, Yellow, Blue, Magenta, Cyan, White
 *      - Bright variants available
 *
 * @par Usage Example
 *      @code
 *      ClearScreen();
 *      SetCursor(1, 1);
 *      SetColor(Color_White, Color_Blue);
 *      printf("Menu Title");
 *      @endcode
 *
 * @note    Requires VT102-compatible terminal (Minicom, PuTTY, etc.)
 * @note    All commands use ANSI escape sequences
 * @warning Non-VT102 terminals may display escape codes as garbage
 *
 * @see Menu_Processor.h for menu system usage
 */

#pragma once
#include "Common/Common.h"
#define TerminalMaxCols	80
#define TerminalMaxRows	24
#define TerminalColCenter TerminalMaxCols / 2
#define TerminalErrorLog TerminalMaxRows - 3
#define TerminalValueLine TerminalMaxRows - 2
#define TerminalPrompt TerminalMaxRows -1

#define Color_Black		'0'
#define Color_Red		'1'
#define Color_Green		'2'
#define Color_Yellow	'3'
#define Color_Blue		'4'
#define Color_Magenta	'5'
#define Color_Cyan		'6'
#define Color_White		'7'

// VT200 terminal emulation function
void ClearTerminal(void);
void ClearToEndOfLine(void);
void CursorHome(void);
void CursorOff(void);
void CursorOn(void);
void HomeCursor(void);
void PrintAt(uint8_t row, uint8_t col);
void SetColor(uint8_t foreground, uint8_t background);
void ClearArea(uint8_t sRow, uint8_t eRow, uint8_t sCol, uint8_t eCol);
void PrintNormal(void);
void CommandPrompt(void);
void LogErrorToScreen(const char *msg);