// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
// By Geelix School of Serious Games and Edutainment.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <crtdbg.h>

// DirectX 11 Header Files
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <dxerr.h>
#include <xnamath.h>
#include <xact3.h>

// TODO: reference additional headers your program requires here


//////////////////////////////////////////////////////////////////////
// Macros

#ifdef _DEBUG

// Asserts the expression. If the expression evaluates to false, the 
// user will get an assertion message
#define ASSERT(b) \
	do \
	{ \
	if (!(b) && _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) \
		_CrtDbgBreak(); \
	} while (0) \

// Outputs the message to the Visual Studio output window
#define TRACE(msg) ::OutputDebugString(msg)

#else // _DEBUG

// Dummy for release builds
#define ASSERT(b) ((void)0)

// Dummy for release builds
#define TRACE(msg) ((void)0)

#endif // _DEBUG
