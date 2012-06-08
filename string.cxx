#include "string.h"

#ifdef WINDOWS
template <> String AsString<NativeString>(Nativestring const &Convertee)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> Converter;
	return Converter.to_bytes(Convertee);
}
#else
template <> String AsString<String>(String const &Convertee)
	{ return Convertee; }
#endif

