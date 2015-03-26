#pragma once
#include <Windows.h>
#include <string>

class IniFile
{
public:

	IniFile(const WCHAR* fileName);

	~IniFile();

	int GetInt(WCHAR* section, WCHAR* item);

	std::wstring GetString(WCHAR* section, WCHAR* item);

	BOOL SetString(WCHAR* section, WCHAR* item, WCHAR* value);

	BOOL SetInt(WCHAR* section, WCHAR* item, int value);

private:

	WCHAR m_fileName[MAX_PATH];
};