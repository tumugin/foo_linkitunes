#pragma once
#include <string>
using namespace std;
#include <windows.h>
std::string ConvertBSTRToMBS(BSTR bstr);
std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen);
BSTR ConvertMBSToBSTR(const std::string& str);