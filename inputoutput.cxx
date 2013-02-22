#include "inputoutput.h"

#include <iostream>
#include <iomanip>
#include <cassert>
#include <cstring>

#ifdef WINDOWS
#include <wchar.h>
#include <string.h>
#endif

#include "filesystem.h"

OutputStream::~OutputStream(void) {}
		
OutputStream &OutputStream::operator <<(StringHexToken const &Data)
{
	std::stringstream Out;
	if (Data.PadToCount > 0)
	{
		Out.width(Data.PadToCount);
		Out.fill('0');
	}
	Out << std::hex << Data.Value;
	*this << Out.str();
	return *this;
}

OutputStream &OutputStream::operator <<(FloatToken const &Data)
{
	std::stringstream Out;

	if (Data.FractionalCount >= 0)
	{
		switch (Data.FractionalCountType)
		{
			case FloatToken::Exact:
				Out.setf(std::ios::fixed);
				break;
			default: break;
		}
		Out.precision(Data.FractionalCount);
	}

	Out << Data.Value;
	*this << Out.str();
	return *this;
}

OutputStream &OutputStream::operator <<(Path const &Data)
	{ *this << Data.AsAbsoluteString(); return *this; }
		
OutputStream::operator String(void) const { assert(false); return ""; }
			
InputStream::~InputStream(void) {}

InputStream &InputStream::operator >>(int &Data)
{
	String Temp;
	*this >> Temp;
	std::stringstream Convert(Temp);
	Convert >> Data;
	return *this;
}

InputStream &InputStream::operator >>(unsigned int &Data)
{
	String Temp;
	*this >> Temp;
	std::stringstream Convert(Temp);
	Convert >> Data;
	return *this;
}
		
InputStream &InputStream::operator >>(float &Data)
{
	String Temp;
	*this >> Temp;
	std::stringstream Convert(Temp);
	Convert >> Data;
	return *this;
}

#ifdef WINDOWS
template <typename WriteType> void WriteSimpleData(bool const &IsConsole, HANDLE OutputHandle, WriteType const &Data)
{
	std::stringstream StringConversion;
	StringConversion << Data;
	DWORD Unused;
	if (IsConsole)
	{
		NativeString EncodingConversion(AsNativeString(StringConversion.str()));
		WriteConsoleW(OutputHandle, EncodingConversion.c_str(), EncodingConversion.length(), &Unused, nullptr);
	}
	else
		WriteFile(OutputHandle, StringConversion.str().c_str(), StringConversion.str().length(), &Unused, nullptr);
}
#endif

StandardStreamTag::StandardStreamTag(void) 
{
#ifdef WINDOWS
	DWORD Unused;
	OutputIsConsole = GetConsoleMode(stdout, &Unused);
	OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	InputIsConsole = GetConsoleMode(stdin, &Unused);
	InputHandle = GetStdHandle(STD_INPUT_HANDLE);
#endif
}

OutputStream &StandardStreamTag::operator <<(OutputStream::FlushToken const &)
	{ CheckOutput(); std::cout << std::flush; return *this; }
	
OutputStream &StandardStreamTag::operator <<(OutputStream::RawToken const &Data)
	{ CheckOutput(); std::cout.write(reinterpret_cast<char const *>(Data.Data), Data.Length); return *this; }
	
/*OutputStream &StandardStreamTag::operator <<(bool const &Data)
{ 
	CheckOutput();
#ifdef WINDOWS
	fwprintf(stdout, L"%d", Data);
#else
	std::cout << Data; 
#endif
	return *this;
}*/

OutputStream &StandardStreamTag::operator <<(int const &Data)
{ 
	CheckOutput();
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(long int const &Data)
{ 
	CheckOutput();
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(unsigned int const &Data)
{ 
	CheckOutput();
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(long unsigned int const &Data)
{ 
	CheckOutput();
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(float const &Data)
{ 
	CheckOutput();
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(double const &Data)
{ 
	CheckOutput();
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(String const &Data)
{ 
	CheckOutput();
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(OutputStream::HexToken const &Data)
{
	CheckOutput(); 
#ifdef WINDOWS
	std::stringstream Conversion;
	Conversion << std::hex << std::setfill('0') << std::setw(2);
	for (unsigned int CurrentPosition = 0; CurrentPosition < Data.Length; CurrentPosition++)
		Conversion << *((unsigned char *)Data.Data + CurrentPosition);
	WriteSimpleData(OutputIsConsole, OutputHandle, Conversion.str());
#else
	std::cout << std::setfill('0');
	for (unsigned int CurrentPosition = 0; CurrentPosition < Data.Length; CurrentPosition++)
		std::cout << std::setw(2) << std::hex << static_cast<int>(((uint8_t *)Data.Data)[CurrentPosition]); 
#endif
	return *this;
}

InputStream &StandardStreamTag::operator >>(InputStream::RawToken &Data)
{ 
	CheckInput(); 
	std::cin.read(reinterpret_cast<char *>(Data.Data), Data.Length); 
	return *this; 
}

InputStream &StandardStreamTag::operator >>(String &Data)
{
	CheckInput(); 
#ifdef WINDOWS	
	if (InputIsConsole)
	{
		DWORD ReadCount;
		wchar_t Read[2]; // Windows bug I read about
		std::vector<char16_t> Line;
		while (ReadConsoleW(InputHandle, Read, 1, &ReadCount, nullptr) && (ReadCount == 1))
		{
			if (Read[0] == L'\n') break;
			Line.push_back(Read[0]);
		}
		Data = AsString(NativeString(&Line[0], Line.size()));
	}
	else std::getline(std::cin, Data); 
#else
	std::getline(std::cin, Data); 
#endif
	return *this; 
}

StandardStreamTag::operator bool(void) const { return std::cin.good(); }

void StandardStreamTag::CheckOutput(void)
	{ if (!std::cout.good()) throw Error::System("Standard output has failed!"); }
	
void StandardStreamTag::CheckInput(void)
	{ if (!std::cin.good()) throw Error::System("Standard input has failed!"); }

StandardStreamTag StandardStream;
		
StandardErrorStreamTag::StandardErrorStreamTag(void)
{
#ifdef WINDOWS
	DWORD Unused;
	OutputIsConsole = !GetConsoleMode(stdout, &Unused);
	OutputHandle = GetStdHandle(STD_ERROR_HANDLE);
#endif
}

OutputStream &StandardErrorStreamTag::operator <<(OutputStream::FlushToken const &)
	{ CheckOutput(); std::cerr << std::flush; return *this; }
	
OutputStream &StandardErrorStreamTag::operator <<(OutputStream::RawToken const &Data)
	{ CheckOutput(); std::cerr.write(reinterpret_cast<char const *>(Data.Data), Data.Length); return *this; }
	
/*OutputStream &StandardErrorStreamTag::operator <<(bool const &Data)
	{ CheckOutput(); std::cerr << Data; return *this; }*/

OutputStream &StandardErrorStreamTag::operator <<(int const &Data)
{ 
	CheckOutput(); 
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cerr << Data; 
#endif
	return *this; 
}

OutputStream &StandardErrorStreamTag::operator <<(long int const &Data)
{ 
	CheckOutput(); 
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cerr << Data; 
#endif
	return *this; 
}

OutputStream &StandardErrorStreamTag::operator <<(long unsigned int const &Data)
{ 
	CheckOutput(); 
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cerr << Data; 
#endif
	return *this; 
}

OutputStream &StandardErrorStreamTag::operator <<(unsigned int const &Data)
{ 
	CheckOutput(); 
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cerr << Data; 
#endif
	return *this; 
}

OutputStream &StandardErrorStreamTag::operator <<(float const &Data)
{ 
	CheckOutput(); 
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cerr << Data; 
#endif
	return *this; 
}

OutputStream &StandardErrorStreamTag::operator <<(double const &Data)
{ 
	CheckOutput(); 
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cerr << Data; 
#endif
	return *this; 
}

OutputStream &StandardErrorStreamTag::operator <<(String const &Data)
{ 
	CheckOutput(); 
#ifdef WINDOWS
	WriteSimpleData(OutputIsConsole, OutputHandle, Data);
#else
	std::cerr << Data; 
#endif
	return *this; 
}

OutputStream &StandardErrorStreamTag::operator <<(OutputStream::HexToken const &Data)
{
	CheckOutput(); 
#ifdef WINDOWS
	std::stringstream Conversion;
	Conversion << std::hex << std::setfill('0') << std::setw(2);
	for (unsigned int CurrentPosition = 0; CurrentPosition < Data.Length; CurrentPosition++)
		Conversion << *((unsigned char *)Data.Data + CurrentPosition);
	WriteSimpleData(OutputIsConsole, OutputHandle, Conversion.str());
#else
	std::cerr << std::setfill('0');
	for (unsigned int CurrentPosition = 0; CurrentPosition < Data.Length; CurrentPosition++)
		std::cerr << std::setw(2) << std::hex << static_cast<int>(((uint8_t *)Data.Data)[CurrentPosition]); 
#endif
	return *this;
}
	
void StandardErrorStreamTag::CheckOutput(void)
	{ if (!std::cerr.good()) throw Error::System("Standard error output has failed!"); }

StandardErrorStreamTag StandardErrorStream;

FileOutput::FileOutput(String const &Filename, unsigned int Mode) :
#ifdef WINDOWS
	File(_wfopen(reinterpret_cast<wchar_t const *>(AsNativeString(Filename).c_str()), 
		Mode & Erase ?
			Mode & Append ? L"wab" :
			L"wb" :
		L"ab"))
#else
	File(fopen(Filename.c_str(),
		Mode & Erase ?
			Mode & Append ? "wa" :
			"w" :
		"a"))
#endif
{
	if (File == nullptr) throw Error::System("Couldn't open file " + Filename);
}

FileOutput::FileOutput(FileOutput &&Other) : File(Other.File)
	{ Other.File = nullptr; }

FileOutput &FileOutput::operator =(FileOutput &&Other)
	{ File = Other.File; Other.File = nullptr; return *this; }

FileOutput::~FileOutput(void)
	{ if (File != nullptr) fclose(File); }

OutputStream &FileOutput::operator <<(OutputStream::FlushToken const &)
	{ CheckOutput(); fflush(File); return *this; }

OutputStream &FileOutput::operator <<(OutputStream::RawToken const &Data)
	{ CheckOutput(); size_t Result = fwrite(Data.Data, Data.Length, 1, File); CheckWriteResult(Result); return *this; }

OutputStream &FileOutput::operator <<(char const &Data)
	{ CheckOutput(); size_t Result = fprintf(File, "%c", Data); CheckWriteResult(Result); return *this; }

/*OutputStream &FileOutput::operator <<(bool const &Data)
	{ CheckOutput(); size_t Result = fprintf(File, Data ? "true" : "false"); CheckWriteResult(Result); return *this; }*/

OutputStream &FileOutput::operator <<(int const &Data)
	{ CheckOutput(); size_t Result = fprintf(File, "%d", Data); CheckWriteResult(Result); return *this; }

OutputStream &FileOutput::operator <<(long int const &Data)
	{ CheckOutput(); size_t Result = fprintf(File, "%ld", Data); CheckWriteResult(Result); return *this; }

OutputStream &FileOutput::operator <<(long unsigned int const &Data)
	{ CheckOutput(); size_t Result = fprintf(File, "%lu", Data); CheckWriteResult(Result); return *this; }

OutputStream &FileOutput::operator <<(unsigned int const &Data)
	{ CheckOutput(); size_t Result = fprintf(File, "%u", Data); CheckWriteResult(Result); return *this; }

OutputStream &FileOutput::operator <<(float const &Data)
	{ CheckOutput(); size_t Result = fprintf(File, "%f", Data); CheckWriteResult(Result); return *this; }

OutputStream &FileOutput::operator <<(double const &Data)
	{ CheckOutput(); size_t Result = fprintf(File, "%f", Data); CheckWriteResult(Result); return *this; }
		
OutputStream &FileOutput::operator <<(String const &Data)
	{ CheckOutput(); size_t Result = fprintf(File, "%s", Data.c_str()); CheckWriteResult(Result); return *this; }

OutputStream &FileOutput::operator <<(OutputStream::HexToken const &Data)
{
	int Result = 0;
	for (unsigned int CurrentPosition = 0; CurrentPosition < Data.Length; CurrentPosition++)
	{
		int NewResult = fprintf(stdout, "%02x", *((unsigned char *)Data.Data + CurrentPosition));
		if ((NewResult < 0) && (Result >= 0)) Result = NewResult;
	}
	CheckWriteResult(Result);
	return *this;
}

void FileOutput::CheckOutput(void)
{
	assert(File != nullptr);
	if (feof(File)) throw Error::System("Received end-of-file while writing to file.");
	if (ferror(File)) throw Error::System("File is in the error state.");
}

void FileOutput::CheckWriteResult(size_t Result)
{
	if (Result <= 0)
	{
		if (ferror(File)) throw Error::System("Encountered error while writing; write failed.");
		else if (feof(File)) throw Error::System("Received end-of-file while writing; write failed.");
	}
}

FileInput::FileInput(String const &Filename) :
#ifdef WINDOWS
	File(_wfopen(reinterpret_cast<wchar_t const *>(AsNativeString(Filename).c_str()), L"rb"))
#else
	File(fopen(Filename.c_str(), "rb"))
#endif
	{ if (File == nullptr) throw Error::System("Couldn't open file " + Filename); }

FileInput::FileInput(FileInput &&Other)
	{ Other.File = nullptr; }
		
FileInput &FileInput::operator =(FileInput &&Other)
	{ File = Other.File; Other.File = nullptr; return *this; }

InputStream &FileInput::operator >>(InputStream::RawToken &Data)
{ 
	size_t Result = fread(Data.Data, Data.Length, 1, File); 
	if (Result <= 0)
	{
		if (ferror(File)) throw Error::System("Encountered error while writing; write failed.");
		else if (feof(File)) throw Error::System("Received end-of-file while writing; write failed.");
	}

	return *this;
}

InputStream &FileInput::operator >>(String &Data)
{
	size_t BufferSize = 1024 * 4;
	char *Buffer = new char[BufferSize];
	while (BufferSize < 1024 * 1024 * 10) 
	{
		delete [] Buffer;
		Buffer = new char[BufferSize];

		char *Result = fgets(Buffer, BufferSize, File);
		if (Result != nullptr)
		{ 
			for (unsigned int BufferIndex = 1; BufferIndex < BufferSize; BufferIndex++)
				if (Buffer[BufferIndex] == '\0')
				{
					Buffer[BufferIndex - 1] = '\0';
					break;
				}
			Buffer[BufferSize - 1] = '\0';
			Data = Buffer; 
			break; 
		}

		if (errno != EINVAL) // EOF?
			break;

		BufferSize *= 2;
	}
	delete [] Buffer;

	return *this;
}
		
FileInput::operator bool(void) const { return !feof(File) && !ferror(File); }

MemoryStream::MemoryStream(unsigned int Reserve) { Buffer.str().reserve(Reserve); }

MemoryStream::MemoryStream(String const &InitialData) : Buffer(InitialData) {}

OutputStream &MemoryStream::operator <<(OutputStream::FlushToken const &)
	{ Buffer << std::flush; return *this; }

OutputStream &MemoryStream::operator <<(OutputStream::RawToken const &Data)
	{ Buffer.write((char const *)Data.Data, Data.Length); return *this; }

OutputStream &MemoryStream::operator <<(char const &Data)
	{ Buffer << Data; return *this; }

/*OutputStream &MemoryStream::operator <<(bool const &Data)
	{ Buffer << Data; return *this; }*/

OutputStream &MemoryStream::operator <<(int const &Data)
	{ Buffer << Data; return *this; }

OutputStream &MemoryStream::operator <<(long int const &Data)
	{ Buffer << Data; return *this; }

OutputStream &MemoryStream::operator <<(long unsigned int const &Data)
	{ Buffer << Data; return *this; }

OutputStream &MemoryStream::operator <<(unsigned int const &Data)
	{ Buffer << Data; return *this; }

OutputStream &MemoryStream::operator <<(float const &Data)
	{ Buffer << Data; return *this; }

OutputStream &MemoryStream::operator <<(double const &Data)
	{ Buffer << Data; return *this; }
		
OutputStream &MemoryStream::operator <<(String const &Data)
	{ Buffer << Data; return *this; }

OutputStream &MemoryStream::operator <<(OutputStream::HexToken const &Data)
{
	Buffer << std::setfill('0');
	for (unsigned int CurrentPosition = 0; CurrentPosition < Data.Length; CurrentPosition++)
		Buffer << std::setw(2) << std::hex << static_cast<int>(((uint8_t *)Data.Data)[CurrentPosition]); 
	return *this;
}

MemoryStream::operator String(void) const 
	{ return Buffer.str(); }

InputStream &MemoryStream::operator >>(InputStream::RawToken &Data)
	{ Buffer.read((char *)Data.Data, Data.Length); return *this; }

InputStream &MemoryStream::operator >>(String &Data)
	{ getline(Buffer, Data); return *this; }

MemoryStream::operator bool(void) const { return Buffer.good(); }

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
template <> String AsString<NativeString>(NativeString const &Convertee)
	{ return Convertee; }
#endif

