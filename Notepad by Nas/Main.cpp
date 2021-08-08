#include "NasWin.h"
#include "Wnd.h"
#include "MenuItem.h"
#include "Header.h"
#include <unordered_map>

// mainWnd is the window created when the application is run
std::vector<TopLevelWnd> windows(1);
// a global map of all TopLevelWnds, indexed by their HWNDs
// unfortunately, I think values here must be pointer type 
std::unordered_map<HWND, TopLevelWnd*> hWndToTopLevelWndMap;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine,int nCmdShow)
{
	// for compatibility issues, I made mainWnd an alias of the first window
	auto& mainWnd = windows[0];

	// calculate entire window size based on desired CLIENT region size
	RECT wr;
	wr.left = 100;
	wr.right = wr.left + 1280;
	wr.top = 100;
	wr.bottom = wr.top + 720;
	if (FAILED(AdjustWindowRect(&wr, mainWnd.styles, FALSE)))
	{
		OutputDebugStringW(L"AdjustWindowRect Failed!");
		exit(-1);
	}
	mainWnd.ncX = wr.left;
	mainWnd.ncY = wr.top;
	mainWnd.ncWidth = wr.right - wr.left;
	mainWnd.ncHeight = wr.bottom - wr.top;

	mainWnd = TopLevelWnd(wr.left, wr.right, wr.left, wr.top, 1280, 720, wr.right - wr.left, wr.bottom - wr.top, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, L"Main Window Class", L"Main Window", hInstance);

	

	// Message Loop
	BOOL gResult;
	MSG msg;
	while ((gResult = GetMessageW(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return msg.wParam;
}
