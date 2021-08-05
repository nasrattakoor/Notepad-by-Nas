#pragma once
#include "NasWin.h"
#include "Wnd.h"

// global variable
extern std::vector<TopLevelWnd> windows;
extern std::unordered_map<HWND, TopLevelWnd*> hWndToTopLevelWndMap;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HRESULT SaveFileByDialog(std::wstring& fileNameHolder);

// EventHandlers.cpp
void OnSaveAs(TopLevelWnd& wnd);
void OnSave(TopLevelWnd& wnd);