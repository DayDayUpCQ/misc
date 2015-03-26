#include "Stdafx.h"
#include "IniFile.h"

IniFile::IniFile(const WCHAR* fileName)
{
	assert(fileName);
	memset(m_fileName, 0, MAX_PATH * sizeof(m_fileName[0]));
	memcpy_s(m_fileName, 
		MAX_PATH * sizeof(m_fileName[0]),
		fileName, wcslen(fileName) * sizeof(fileName[0]));
}

IniFile::~IniFile()
{
}

int IniFile::GetInt(WCHAR* section, WCHAR* item)
{
	return ::GetPrivateProfileInt(section, item, 0, m_fileName);
}

std::wstring  IniFile::GetString(WCHAR* section, WCHAR* item)
{
	std::wstring  strValue(L"");
	WCHAR swzValue[MAX_PATH];
	memset(swzValue, 0, MAX_PATH * sizeof(swzValue[0]));
	::GetPrivateProfileString(section, item, 0, swzValue, MAX_PATH, m_fileName);
	strValue = swzValue;
	return strValue;
}

BOOL IniFile::SetString(WCHAR* section, WCHAR* item, WCHAR* value)
{
	return ::WritePrivateProfileString(section, item, value, m_fileName);
}

BOOL IniFile::SetInt(WCHAR* section, WCHAR* item, int value)
{
	WCHAR swzValue[MAX_PATH] = {0};
	swprintf(swzValue, L"%d", value);
	return ::WritePrivateProfileString(section, item, swzValue, m_fileName);
}
