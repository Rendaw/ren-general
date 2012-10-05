#ifndef string_h
#define string_h

#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <list>
#include <fstream>
#ifdef WINDOWS
#include <windows.h>
#include <cassert>
#endif

typedef unsigned char Byte;
typedef std::vector<Byte> ByteBuffer;

typedef std::string String; // Always UTF-8, use u8"" to specify literals

#ifdef WINDOWS
typedef std::u16string NativeString;

inline NativeString AsNativeString(String const &Input)
{
	static_assert(sizeof(wchar_t) == sizeof(char16_t), "Assumption that all Windows systems use 16-bit wide characters failed!");
	const int Length = MultiByteToWideChar(CP_UTF8, 0, Input.c_str(), Input.length(), nullptr, 0);
	assert(Length > 0);
	std::vector<char16_t> ConversionBuffer;
	ConversionBuffer.resize(Length);
	MultiByteToWideChar(CP_UTF8, 0, Input.c_str(), Input.length(), (LPWSTR)&ConversionBuffer[0], Length);
	return NativeString(&ConversionBuffer[0], Length);
}
#else
typedef std::string NativeString;

inline String AsNativeString(String const &Input)
	{ return Input; }
#endif

template <typename Base> String AsString(const Base &Convertee)
{
	std::stringstream Out;
	Out << Convertee;
	return Out.str();
}

template <> String AsString<NativeString>(NativeString const &Convertee);

#endif
