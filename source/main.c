/*
	BonkClicker - a goofy clicker for WinAPI
	Made by Applehat. (github.com/Jakkret)
	BONK - for MarsVinyl1234
*/ 

// ## Trying to make it more readable and organized
// - ApplehatDot on 9.08.2025

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdbool.h>
#include "PredFunc.c"

#pragma comment(lib, "winmm.lib")

HWND hClicka, hText;
HBITMAP hBitmap;
bool IsSoundPlaying;
int points = 0;

void UpdatePointsText() {
    wchar_t buffer[64];
    swprintf(buffer, 64, L"Points: %d", points);
    SetWindowTextW(hText, buffer);
}

void MakeMenu(HWND hwnd){
	HMENU hMenuBar = CreateMenu();
	HMENU hQuit = CreateMenu();
	HMENU hAbout = CreateMenu();
	
	/* Tabbing cuz it makes stuff stand-out */
		AppendMenuW(hQuit, MF_STRING, 9001, L"Quit");
		AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hQuit, L"Quit");
		AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hAbout, L"About Game");
		AppendMenuW(hAbout, MF_STRING, 9002, L"About");
		
	SetMenu(hwnd, hMenuBar);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_CREATE:
			
			MakeMenu(hwnd);
		
			hText = CreateWindowExW(
				0,
				L"STATIC",
				L"Points: 0",
				WS_CHILD | WS_VISIBLE,
				70, 50, 130, 20,
				hwnd,
				NULL,
				GetModuleHandle(NULL),
				NULL
			);
			
			SetSystemThemeFont(hText);
			
			hClicka = CreateWindowExW(
				0, 
				L"BUTTON",
				L"bonk",
				WS_VISIBLE | WS_CHILD | BS_BITMAP,
				70, 100, 130, 130,
				hwnd,
				(HMENU)100, 
				GetModuleHandle(NULL), 
				NULL
			);
			
			hBitmap = (HBITMAP)LoadImageW(
                NULL,
                L"bonk.bmp",
                IMAGE_BITMAP,
                0, 0,
                LR_LOADFROMFILE
            );
			
            if (hBitmap){
                SendMessageW(hClicka, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
            } else if (!hBitmap){
				MessageBoxW(NULL, L"No bitmap detected - game cannot start", L"Error", MB_OK | MB_ICONEXCLAMATION);
				exit(1);
			}
            break;
			
		case WM_COMMAND:
		
			// Using switches is .. more readable ig :shrug:
			switch(LOWORD(wParam)){
				case 100: //if (LOWORD(wParam) == 100) {
					points++;
					UpdatePointsText();
					if (points % 100 == 0) {
						IsSoundPlaying = PlaySoundW(L"bonk.wav", NULL, SND_FILENAME | SND_ASYNC);
						if(!IsSoundPlaying){		
							MessageBoxW(NULL, L"Could not play sound - PlaySoundW Failed", L"Audio Error", MB_OK | MB_ICONEXCLAMATION);
						}
					}
				break;
				
				case 9001: //if (LOWORD(wParam) == 9001) {
					PostMessage(hwnd, WM_CLOSE, 0, 0);
					break;
					
				case 9002: //if (LOWORD(wParam) == 9002) {
					MessageBoxW(NULL,  L"BONK Clicker v1.0.1-Enhanced\n\nCreated by Applehat.\nDedicated to fellow bonkers:\nElif, MarsVinyl1234, Deborah (a.k.a. Reverse Name)", L"About", MB_OK);
					break;
			}	// <-- switch ends here
			
		default:
			return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}
	
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	const wchar_t CLASS_NAME[] = L"MainClickWnd";
	
	WNDCLASSW wc = {};
	
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	
	if(!RegisterClassW(&wc)){
		MessageBoxW(NULL, L"Window Registration Failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	
	HWND hwnd = CreateWindowExW(
		0,
		CLASS_NAME,
		L"BonkClicker",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		300, 300,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	
	MSG msg = {};
	while(GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}