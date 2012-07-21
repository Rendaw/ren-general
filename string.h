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

typedef std::string String; // Always UTF-8, use u8"" to specify literals
#ifdef WINDOWS
typedef std::u16string NativeString;
#else
typedef std::string NativeString;
#endif

typedef std::istream InputStream;
typedef std::ostream OutputStream;

typedef std::ifstream FileInput;
typedef std::ofstream FileOutput;
typedef std::stringstream StringStream;

/* TODO Support utf-8
inline String Left(const String &Base, size_t Size)
	{ return Base.substr(0, Size); }

inline String Right(const String &Base, size_t Size)
{
	if (Base.size() <= Size) return Base;
	return Base.substr(Base.size() - Size, Size);
}
*/

#ifdef WINDOWS
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
inline String AsNativeString(String const &Input)
	{ return Input; }
#endif

template <typename Base> String AsString(const Base &Convertee)
{
	StringStream Out;
	Out << Convertee;
	return Out.str();
}

template <> String AsString<NativeString>(NativeString const &Convertee);

#endif
