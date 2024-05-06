#pragma once
#include <Windows.h>
#include <string>
#include "../libs/json/json.hpp"

using json = nlohmann::ordered_json;

class FileManager
{
private:
	void LoadConfig(std::wstring strConfigFileName);
	void LoadLanguage(std::wstring strLanguageFileName);

public:
	std::wstring ProcessPath;
	std::wstring ConfigPath;
	json Config;

	std::wstring LanguagePath;
	json Language;

	void Init();
	void SaveFile(std::wstring strFileName, json Data);
	std::string GetFontFile();
};
extern FileManager g_FileManager;