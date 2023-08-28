#pragma once

#include <string>

inline void StringToWString(const char* str, std::wstring& wstr) {
	size_t size = strlen(str) + 1;
	wchar_t* convert = new wchar_t[size];

	size_t outSize;
	mbstowcs_s(&outSize, convert, size, str, size - 1);

	wstr = convert;
	delete[] convert;
}

inline void WStringToString(const wchar_t* wstr, std::string& str) {
	size_t size = wcslen(wstr) + 1;
	char* convert = new char[size];

	size_t outSize;
	wcstombs_s(&outSize, convert, size, wstr, size - 1);

	str = convert;
	delete[] convert;
}