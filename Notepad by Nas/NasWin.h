#pragma once

#ifndef UNICODE
#define UNICODE
#endif

// target Windows 7 or later
#define _WIN32_WINNT 0x0601 
#include <sdkddkver.h>

// never want to include this one - will replace all our std::min's and std::max's with Windows macros - actual cancer
#define NOMINMAX

// these ones we can toggle
#ifndef LET_WINDOWS_POLLUTE_NAMESPACE

#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
//#define NOVIRTUALKEYCODE
//#define NOWINMESSAGES
//#define NOWINSTYLES
#define NOSYSMETRICS
//#define NOMENUS // need for CreateMenu
#define NOICONS
//#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
//#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
//#define NOCOLOR // need for GetSysColor
//#define NOCTLMGR // need this for buttons
#define NODRAWTEXT
//#define NOGDI
#define NOKERNEL
//#define NOUSER
#define NONLS
//#define NOMB
#define NOMEMMGR
#define NOMETAFILE
//#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
//#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE

#endif

#define STRICT

#include <Windows.h>