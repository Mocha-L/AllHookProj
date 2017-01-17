#pragma once

#include <string>

std::wstring  stringformat(const wchar_t* fmt,   ...);
std::wstring Path_GetCurrent(HMODULE hModule = NULL);