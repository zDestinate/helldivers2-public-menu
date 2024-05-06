#pragma once

#include <Windows.h>
#include <string>
#include <codecvt>


static std::wstring utf_to_wstring(const std::string& str)
{
    // Convert UTF-8 encoded string to wstring
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

static std::string wstring_to_utf8(const std::wstring& str)
{
    // Convert wstring to narrow string (UTF-8 encoded)
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(str);
}