#include "string.h"

#ifdef WINDOWS
template <> String AsString<NativeString>(NativeString const &Convertee)
{
	const int Length = WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<wchar_t const *>(Convertee.c_str()), Convertee.length(), nullptr, 0, nullptr, nullptr);
	assert(Length > 0);
	std::vector<char> ConversionBuffer;
	ConversionBuffer.resize(Length);
	WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<wchar_t const *>(Convertee.c_str()), Convertee.length(), &ConversionBuffer[0], Length, nullptr, nullptr);
	return String(&ConversionBuffer[0], Length);
}
#else
template <> String AsString<String>(String const &Convertee)
	{ return Convertee; }
#endif

