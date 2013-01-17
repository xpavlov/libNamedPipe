#pragma once
#ifdef _WIN32
	#include "WinNamedPipe.h"
	typedef WinNamedPipe NamedPipe;
#else
	#include <unistd.h>
	#include "PosixNamedPipe.h"
	typedef PosixNamedPipe NamedPipe;
#endif