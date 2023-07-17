#pragma once

// target Windows 10 and later
#define _WIN32_WINNT 0x0A00 
#include <sdkddkver.h>

// disable unnecessary macro in Windows.h
#define NOMINMAX

#include <Windows.h>