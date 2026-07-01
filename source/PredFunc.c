/*
 *	Pred(efined)Func(tions).c - Set of predefined functions made eazier
 * 	to use in code 
 */

#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

// ------------- WINAPI BATTERY FUNCTIONS ----------

LRESULT SetSystemThemeFont(HWND hwnd){
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	return SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);
}

