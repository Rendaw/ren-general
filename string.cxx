#include "string.h"

#ifdef WINDOWS
template <> String AsString<std::u16string>(std::u16string const &Convertee)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> Converter;
	return Converter.to_bytes(Convertee);
}
#else
template <> String AsString<String>(String const &Convertee)
	{ return Convertee; }
#endif

