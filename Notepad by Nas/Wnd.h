#pragma once
#include "NasWin.h"
#include <functional>
#include <vector>
#include <fstream>
#include "MenuItem.h"

struct TopLevelWnd;

// forward declaration
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern std::vector<TopLevelWnd> windows;
extern std::unordered_map<HWND, TopLevelWnd*> hWndToTopLevelWndMap;

struct Wnd
{
	Wnd(int x, int y, int ncX, int ncY,	int width, int height, int ncWidth, int ncHeight, DWORD styles, DWORD exStyles, const wchar_t* className, const wchar_t* name, HINSTANCE hInstance = GetModuleHandleW(nullptr), HWND hWnd = nullptr, HWND hWndParent = nullptr, HMENU hMenu = nullptr)
		:
		x(x),
		y(y),
		ncX(ncX),
		ncY(ncY),
		width(width),
		height(height),
		ncWidth(ncWidth),
		ncHeight(ncHeight),
		styles(styles), 
		exStyles(exStyles),
		className(className),
		name(name),
		hInstance(hInstance),
		hWnd(hWnd),
		hWndParent(hWndParent)
	{}

	Wnd() = default;

	int x;
	int y;
	int ncX;
	int ncY;

	int width;
	int height;
	int ncWidth;
	int ncHeight;

	bool wordWrapOn = false;

	bool newFile = true; // whether the stream member is associated with a file or not
	std::vector<HWND> children;

	DWORD styles;
	DWORD exStyles;
	const wchar_t* className;
	const wchar_t* name;
	HWND hWnd;
	HWND hWndParent;
	//std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> wndProc;
	HINSTANCE hInstance;
	// a data structure to store hMenus (tree?)
	LPVOID lpParam = nullptr;
};

struct TopLevelWnd : public Wnd
{
	TopLevelWnd(int x, int y, int ncX, int ncY, int width, int height, int ncWidth, int ncHeight, DWORD styles, DWORD exStyles, const wchar_t* className, const wchar_t* name, HINSTANCE hInstance = GetModuleHandleW(nullptr), HWND hWnd = nullptr, HWND hWndParent = nullptr, HMENU hMenu = nullptr)
		: Wnd(x, y, ncX, ncY, width, height, ncWidth, ncHeight, styles, exStyles, className, name, hInstance ,hWnd, hWndParent),
		hMenu(hMenu)
	{
		// Class Specifications
		{
			WNDCLASSEXW wc = { 0 };
			// if the class doesn't already exist
			if (!GetClassInfoExW(hInstance, className, &wc))
			{
				wc.cbSize = sizeof wc;
				wc.hInstance = hInstance;
				wc.lpszClassName = className;
				wc.lpfnWndProc = WndProc;
				wc.style = CS_DBLCLKS | CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
				RegisterClassExW(&wc);
			}
		}

		// create window
		hWnd = CreateWindowExW(
			0,
			className,
			name,
			styles,
			ncX, ncY,
			ncWidth,
			ncHeight,
			hWndParent,
			hMenu,
			hInstance,
			nullptr
		);

		// Check if Window Creation Failed
		if (!hWnd)
		{
			exit(-1);
		}

		// Menu Creation
		{
			// Create Menu
			hMenu = CreateMenu(); // Main Menu Bar
			// File SubMenu
			HMENU hFileMenu = CreateMenu();
			AppendMenuW(hMenu, MF_POPUP, UINT_PTR(hFileMenu), L"File"); // Pop-Up Menu
			AppendMenuW(hFileMenu, MF_STRING, MenuItem::File::NEW, L"New");
			AppendMenuW(hFileMenu, MF_STRING, MenuItem::File::OPEN, L"Open...");
			AppendMenuW(hFileMenu, MF_STRING, MenuItem::File::CLOSE, L"Close");
			AppendMenuW(hFileMenu, MF_STRING, MenuItem::File::SAVE, L"Save");
			AppendMenuW(hFileMenu, MF_STRING, MenuItem::File::SAVEAS, L"Save As...");
			AppendMenuW(hFileMenu, MF_SEPARATOR, NULL, nullptr); // line separator
			AppendMenuW(hFileMenu, MF_STRING, MenuItem::File::EXIT, L"Exit");
			// Edit SubMenu
			HMENU hEditMenu = CreateMenu();
			AppendMenuW(hMenu, MF_POPUP, UINT_PTR(hEditMenu), L"Edit");
			AppendMenuW(hEditMenu, MF_STRING, MenuItem::Edit::CUT, L"Cut");
			AppendMenuW(hEditMenu, MF_STRING, MenuItem::Edit::COPY, L"Copy");
			AppendMenuW(hEditMenu, MF_STRING, MenuItem::Edit::PASTE, L"Paste");
			AppendMenuW(hEditMenu, MF_STRING, MenuItem::Edit::UNDO, L"Undo");
			// Format SubMenu
			HMENU hFormatMenu = CreateMenu();
			AppendMenuW(hMenu, MF_POPUP, UINT_PTR(hFormatMenu), L"Format");
			AppendMenuW(hFormatMenu, MF_STRING | MF_UNCHECKED, MenuItem::Format::WORD_WRAP, L"Word Wrap");
			// View SubMenu
			HMENU hViewMenu = CreateMenu();
			AppendMenuW(hMenu, MF_POPUP, UINT_PTR(hViewMenu), L"View");
			AppendMenuW(hMenu, MF_POPUP, 0, L"View");
			AppendMenuW(hMenu, MF_STRING, MenuItem::Help::VIEW_HELP, L"Help"); // No Pop-Up; Sends WM_COMMAND msg with wParam = the enum we pass here

			SetMenu(hWnd, hMenu);
		}

		// create Edit control child window for text editing
		editControl = CreateWindowExW(0, L"Edit", L"",
			WS_CHILD | WS_BORDER | (wordWrapOn ? ES_AUTOHSCROLL : 0) | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL,
			0, 0,
			width, height,
			hWnd, nullptr, nullptr, nullptr);

		if (!editControl)
		{
			exit(-1);
		}

		ShowWindow(hWnd, SW_SHOW);
		ShowWindow(editControl, SW_SHOW); // could've added WS_VISIBLE or other way

		// update the global map
		hWndToTopLevelWndMap[hWnd] = this;
	}

	TopLevelWnd() = default;

	// delete copy members because fstream cannot be copied
	TopLevelWnd& operator=(const TopLevelWnd&) = delete;
	TopLevelWnd(const TopLevelWnd&) = delete;
	
	// move members
	TopLevelWnd(TopLevelWnd&& source)
		: Wnd(std::move(source)),
		file(std::move(source.file))
	{
	}
	TopLevelWnd& operator=(TopLevelWnd&& right)
	{
		Wnd::operator=(std::move(right));
		file = std::move(right.file);
		return *this;
	}

	HMENU hMenu;
	HWND editControl;
	std::wfstream file;
	std::wstring filePath;
};