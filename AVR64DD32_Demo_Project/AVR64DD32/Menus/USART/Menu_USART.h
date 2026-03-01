/*
 * Menu_USART.h
 *
 * Created: 11/28/2025 9:22:41 AM
 *  Author: terry
 */ 

#pragma once

#include "Common/Common.h"
#include "Common/DataStructures.h"

void USART_Menu(const char *msg);
void BaudRateMainMenu(const char *msg);
void ParityMainMenu(const char *msg);
void StopBitsMainMenu(const char *msg);
void DataBitsMainMenu(const char *msg);

MenuSystem BaudRateMainMenuSystem;
MenuSystem ParityMainMenuSystem;
MenuSystem StopBitsMainMenuSystem;
MenuSystem DataBitsMainMenuSystem;