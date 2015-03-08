#pragma once
#include <iostream>
#include <vector>
#include <string>

#include <Windows.h>
#include <TlHelp32.h>
typedef std::vector<std::wstring> ModuleNameList;
ModuleNameList getModuleList();