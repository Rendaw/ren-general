#include "string.h"

#ifdef WINDOWS
template <> String AsString<NativeString>(Nativestring const &Convertee)
{
	const int Length = WideCharToMultiByte(CP_UTF8, 0, Convertee.c_str(), Convertee.length(), nullptr, 0, nullptr, nullptr);
	assert(Length > 0);
	std::vector<char> ConversionBuffer;
	ConversionBuffer.resize(Length);
	MultiByteToWideChar(CP_UTF8, 0, Convertee.c_str(), Convertee.length(), &ConversionBuffer[0], Length, nullptr, nullptr);
	return String(&ConversionBuffer[0], Length);
}
#else
template <> String AsString<String>(String const &Convertee)
	{ return Convertee; }
#endif

