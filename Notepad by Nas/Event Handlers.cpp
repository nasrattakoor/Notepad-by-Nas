#include "NasWin.h"
#include "Wnd.h"
#include "Header.h"
#include <fstream>
#include <cassert>

void OnSaveAs(TopLevelWnd& wnd)
{
	// Show Save-Dialog to user
	HRESULT hr = SaveFileByDialog(wnd.filePath);
	// TODO: handle user hitting Cancel
	if (FAILED(hr))
	{
		exit(-1);
	}

	// get the length of text in edit control
	size_t fileLength = GetWindowTextLengthW(wnd.editControl);
	
	// allocate a wide array large enough to fit edit control contents
	wchar_t* contents = new wchar_t[fileLength + 1]; // +1 for the \0
	// copy text from edit control to array
	GetWindowTextW(wnd.editControl, contents, sizeof contents / sizeof contents[0] - 1);
	// open file stream
	wnd.file.open(L"file.txt", std::ios_base::out);
	// if stream opened successfully
	if (wnd.file.is_open())
	{
		// write text from array to file
		wnd.file << contents;
	}
	// otherwise, exit program with error
	else
	{
		exit(-1);
	}
}

void OnSave(TopLevelWnd& wnd)
{
	if (wnd.newFile)
	{
		assert(wnd.filePath == L""); // if it's a new file, path shouldn't be defined
		OnSaveAs(wnd);
	}
	else
	{
		assert(wnd.filePath != L""); // if it's not a new file, then path should already be defined
		wnd.file.close();
		wnd.file.open(wnd.filePath, std::ios::out | std::ios::trunc);
		size_t fileLength = GetWindowTextLengthW(wnd.hWnd);
		wchar_t* contents = new wchar_t[fileLength + 1];
		GetWindowTextW(wnd.hWnd, contents, fileLength + 1);
		wnd.file << contents;
	}
}

