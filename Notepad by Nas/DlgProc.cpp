#include "NasWin.h"
#include "resource.h"

INT_PTR CALLBACK SaveDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	// handle any processing done before dialog creation (instead of WM_NCCREATE)
	case WM_INITDIALOG:
		break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_SAVE:
			EndDialog(hWnd, IDC_SAVE);
			break;
		case IDC_DONTSAVE:
			EndDialog(hWnd, IDC_DONTSAVE);
			break;
		case IDCANCEL:
			EndDialog(hWnd, IDCANCEL);
			break;
		}
	}

	default:
		return FALSE;
	}
	return TRUE;
}