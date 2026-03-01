/**
 * @file    Themes.h
 * @brief   Color theme definitions for terminal interface
 * @details Provides predefined color schemes for consistent visual appearance
 *          across menus and data displays. Themes specify foreground and
 *          background color combinations.
 *
 * @author  Terry (ABS Data Systems)
 * @date    November 25, 2025
 * @version 1.0
 *
 * @par Predefined Themes
 *      - Header theme: Menu titles and section headers
 *      - Body theme: Menu items and descriptions
 *      - Data theme: Real-time data display
 *      - Prompt theme: User input areas
 *      - Error theme: Error messages and warnings
 *
 * @par Theme Structure
 *      Each theme contains:
 *      - Foreground color (text)
 *      - Background color (behind text)
 *
 * @note    Themes are const structures in PROGMEM
 * @note    Color values map to VT102 color codes
 * @see Terminal_Commands.h for color constants
 */


#pragma once

extern const Theme themeWhiteBlack[];
extern const Theme themeYellowBlack[];
extern const Theme themeBlackYellow[];
extern const Theme themeRedBlack[];
extern const Theme themeRedYellow[];
extern const Theme themeCyanBlack[];
extern const Theme themeBlueWhite[];
extern const Theme themeRedWhite[];
extern const Theme themeBlackRed[];