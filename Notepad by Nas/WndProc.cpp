#define _CRT_SECURE_NO_WARNINGS

#include "NasWin.h"			// must be first include
#include "Wnd.h"
#include "MenuItem.h"
#include "resource.h"
#include "Header.h"

#include <cassert>
#include <sstream>

#include <combaseapi.h>		// for CoCreateInstance
#include <shobjidl_core.h>	// for IFileDialog

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_DESTROY: // ?
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case MenuItem::File::NEW:
			// if the edit control contains text 
			if (GetWindowTextLengthW(hWndToTopLevelWndMap[hWnd]->editControl) > 0)
			{
				// prompt the user to either save, don't save, or cancel
				int ret = DialogBoxW(hWndToTopLevelWndMap[hWnd]->hInstance, MAKEINTRESOURCEW(IDD_DIALOG1), hWnd, SaveDlgProc);
				// if user chooses to save
				if (ret == IDC_SAVE)
				{
					// if no associated file
					if (hWndToTopLevelWndMap[hWnd]->newFile)
					{
						// trigger Save dialog and handle it
						PostMessageW(hWndToTopLevelWndMap[hWnd]->hWnd, WM_COMMAND, MenuItem::File::SAVEAS, 0);
					}
					else
					{
						// overwrite the file already associated with window
						PostMessageW(hWnd, WM_COMMAND, MenuItem::File::SAVE, 0);
					}
				}
				// or if user chooses not to save
				else if (ret == IDC_DONTSAVE)
				{
					// simply erase whatever text is in the edit control
					SetWindowTextW(hWndToTopLevelWndMap[hWnd]->editControl, L"");
				}
				// or if the user cancels
				else if (ret == IDCANCEL)
				{
					// do nothing
				}
			}
			break;
		case MenuItem::File::OPEN:
		{
			// wstring to hold file path chosen by user in Open-Dialog
			std::wstring fileName;

			// get the name of the file chosen by the user in the open-dialog
			// if user presses cancel or close button, cancel the open operation
			if (GetFileByDialog(fileName) == HRESULT_FROM_WIN32(ERROR_CANCELLED))
			{
				break;
			}

			// create a file stream associated with the file path
			hWndToTopLevelWndMap[hWnd]->file.open(fileName);

			// read the entire file into a wstringstream
			// TODO: create new window when opening a file
			std::wstringstream contents;
			while (!hWndToTopLevelWndMap[hWnd]->file.eof())
			{
				std::wstring line;
				std::getline(hWndToTopLevelWndMap[hWnd]->file, line);
				contents << line << "\r\n";
			}

			// set the contents of the edit control to the contents of the wstringstream
			// this window's now associated with an existing file, so set newFile to false
			SetWindowTextW(hWndToTopLevelWndMap[hWnd]->editControl, contents.str().c_str());
			hWndToTopLevelWndMap[hWnd]->newFile = false;
		}
		break;
		case MenuItem::File::SAVE:
		{
			// if user presses Save and there's no file associated with this window
			if (hWndToTopLevelWndMap[hWnd]->newFile)
			{
				// turn the Save into a Save-As...
				PostMessageW(hWnd, WM_COMMAND, MenuItem::File::SAVEAS, 0);
			}
			// otherwise
			else
			{
				OnSave(*hWndToTopLevelWndMap[hWnd]);
			}
		}
			break;
		case MenuItem::File::SAVEAS:
			OnSaveAs(*hWndToTopLevelWndMap[hWnd]);
			break;
		case MenuItem::File::CLOSE:
			//
			break;
		case MenuItem::File::EXIT:
			PostQuitMessage(0);
			break;

		case MenuItem::Edit::CUT:
			//
			break;
		case MenuItem::Edit::COPY:
			//
			break;
		case MenuItem::Edit::PASTE:
			//
			break;
		case MenuItem::Edit::UNDO:
			//
			break;

		case MenuItem::Format::WORD_WRAP:
		{
			// find out if menu item is checked or not, and toggle it
			MENUITEMINFOW mii = { 0 };
			mii.cbSize = sizeof MENUITEMINFOW; // required

			// get the HMENU of the drop down menu associated with the Format menu item
			HMENU formatMenu = GetSubMenu(hWndToTopLevelWndMap[hWnd]->hMenu, MenuItem::Position::POS_FORMAT);

			// required to use fState member
			mii.fMask |= MIIM_STATE; 
			// get the info of the Word Wrap menu item, checking if the call succeeded
			if (!GetMenuItemInfoW(formatMenu, LOWORD(wParam), FALSE, &mii))
			{
				PostQuitMessage(-900);
				break;
			}

			DWORD editWinStyles = GetWindowLongPtrW(hWndToTopLevelWndMap[hWnd]->editControl, GWL_STYLE);
			// check whether option is checked or unchecked, and toggle it
			if (hWndToTopLevelWndMap[hWnd]->wordWrapOn)
			{
				mii.fState ^= MFS_CHECKED;
				mii.fState ^= MFS_UNCHECKED;
				hWndToTopLevelWndMap[hWnd]->wordWrapOn = false;

				SetWindowLongPtrW(hWndToTopLevelWndMap[hWnd]->editControl, GWL_STYLE, editWinStyles | ~ES_AUTOHSCROLL);
			}
			else
			{
				mii.fState ^= MFS_UNCHECKED;
				mii.fState |= MFS_CHECKED;
				hWndToTopLevelWndMap[hWnd]->wordWrapOn = true;

				SetWindowLongPtrW(hWndToTopLevelWndMap[hWnd]->editControl, GWL_STYLE, editWinStyles | ES_AUTOHSCROLL);
			}

			// Update menu
			SetMenuItemInfoW(formatMenu, LOWORD(wParam), FALSE, &mii);
			DrawMenuBar(hWnd);

		}
		break;
		case MenuItem::Format::FONT:
			//
			break;

		case MenuItem::View::Zoom_In:
			//
			break;
		case MenuItem::View::Zoom_Out:
			// 
			break;
		case MenuItem::View::FullScreen:
			// 
			break;
		}
		break;
	}
	case WM_KEYDOWN:
		break;
	}
	return DefWindowProcW(hWnd, msg, wParam, lParam);
}