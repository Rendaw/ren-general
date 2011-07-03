#ifndef string_h
#define string_h

#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <list>
#include <fstream>

typedef std::string Unstrung;
typedef std::string String;
typedef std::ifstream InputStream;
typedef std::ofstream OutputStream;
typedef std::stringstream StringStream;

inline String Left(const String &Base, size_t Size)
	{ return Base.substr(0, Size); }

inline String Right(const String &Base, size_t Size)
{
	if (Base.size() <= Size) return Base;
	return Base.substr(Base.size() - Size, Size);
}

template <typename Base> String AsString(const Base &Convertee)
{
	StringStream Out;
	Out << Convertee;
	return Out.str();
}

#endif
