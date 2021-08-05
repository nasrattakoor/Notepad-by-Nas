#pragma once
namespace MenuItem
{
	enum Position
	{
		POS_FILE = 0,
		POS_EDIT = 1,
		POS_FORMAT = 2,
		POS_VIEW = 3,
		POS_HELP = 4
	};

	enum File
	{
		FILE = 0,
		NEW = 1,
		SAVE = 2,
		SAVEAS = 3,
		CLOSE = 4,
		OPEN = 5,
		EXIT = 6
	};

	enum Edit
	{
		EDIT = 100,
		CUT = 101,
		COPY = 102,
		PASTE = 103,
		UNDO = 104
	};

	enum Format
	{
		FORMAT = 200,
		WORD_WRAP = 201,
		FONT = 202
	};

	enum View
	{
		VIEW = 300,
		Zoom_In = 301,
		Zoom_Out = 302,
		FullScreen = 303
	};

	enum Help
	{
		HELP = 400,
		VIEW_HELP = 401,
		ABOUT_NOTEPAD = 402
	};
}