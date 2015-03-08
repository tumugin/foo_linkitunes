#include "prochelper.h"

// 現在起動しているプロセス名のリストを返す
ModuleNameList getModuleList() {
	ModuleNameList module_name_list;
	// スナップショットを取得
	HANDLE snapshot = ::CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS, 0
		);
	if (snapshot == INVALID_HANDLE_VALUE)
		throw std::exception();

	// プロセスの情報を走査
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(entry);
	BOOL has_entry = ::Process32FirstW(snapshot, &entry);
	while (has_entry) {
		module_name_list.push_back(entry.szExeFile);
		has_entry = ::Process32NextW(snapshot, &entry);
	}
	::CloseHandle(snapshot);

	return module_name_list;
}