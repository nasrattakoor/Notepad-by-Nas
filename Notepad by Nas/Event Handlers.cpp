#include "NasWin.h"
#include "Wnd.h"
#include "Header.h"
#include <fstream>
#include <cassert>

// saves with Unix line endings (\n) by default
void OnSaveAs(TopLevelWnd& wnd)
{
	// Show Save-Dialog to user
	HRESULT hr = SaveFileByDialog(wnd.filePath);
	// TODO: handle user hitting Cancel
	if (FAILED(hr))
	{
		return;
	}

	// get the length of text in edit control
	size_t fileLength = GetWindowTextLengthW(wnd.editControl);
	
	// allocate a wide array large enough to fit edit control contents
	// NOTE: you cannot use sizeof on a new[] array
	int size = fileLength + 1; // +1 for the \0
	//wchar_t* contents = new wchar_t[size]; 
	wchar_t contents_unix[1024 * 100];
	wchar_t contents_windows[1024 * 100];
	// copy text from edit control to array
	GetWindowTextW(wnd.editControl, contents_windows, size);
	ConvertWindowsToUnixLineEndings(contents_unix, contents_windows, size);
	// open file stream
	wnd.file.open(wnd.filePath, std::ios_base::out | std::ios_base::trunc | std::ios::binary);
	// if stream opened successfully
	if (wnd.file.is_open())
	{
		// write text from array to file
		wnd.file << contents_unix;
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
		wnd.file.open(wnd.filePath, std::ios::out | std::ios::trunc | std::ios::binary);
		size_t fileLength = GetWindowTextLengthW(wnd.editControl);
		int buffer_size = fileLength + 1;
		//wchar_t* contents = new wchar_t[fileLength + 1];
		wchar_t contents_unix[1024 * 100];
		wchar_t contents_windows[1024 * 100];
		GetWindowTextW(wnd.editControl, contents_windows, fileLength + 1);
		ConvertWindowsToUnixLineEndings(contents_unix, contents_windows, buffer_size);
		wnd.file << contents_unix;

		//delete[] contents;
	}
}

void ConvertWindowsToUnixLineEndings(wchar_t dest[], const wchar_t source[], long long int nMax)
{
	for(int srcIndex = 0, destIndex = 0; srcIndex < nMax; srcIndex++, destIndex++)
	{
		if (source[srcIndex] == '\r' && source[srcIndex + 1] == '\n')
		{
			dest[destIndex] = '\n';
			srcIndex++;
		}
		else
		{
			dest[destIndex] = source[srcIndex];
		}
	}
}