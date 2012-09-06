#include "inputoutput.h"

#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

OutputStream::FlushToken OutputStream::Flush(void)
	{ return FlushToken(); }
		
OutputStream::~OutputStream(void) {}
			
InputStream::~InputStream(void) {}

OutputStream &StandardStreamTag::operator <<(FlushToken const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout << std::flush; return *this; }
	
OutputStream &StandardStreamTag::operator <<(RawToken const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout.write(reinterpret_cast<char const *>(Data.Data), Data.Size); return *this; }
	
OutputStream &StandardStreamTag::operator <<(bool const &Data) throw(Error::System &)
{ 
	CheckOutput();
#ifdef WINDOWS
	fwprintf(stdout, L"%d", Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(int const &Data) throw(Error::System &)
{ 
	CheckOutput();
#ifdef WINDOWS
	fwprintf(stdout, L"%d", Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(long int const &Data) throw(Error::System &)
{ 
	CheckOutput();
#ifdef WINDOWS
	fwprintf(stdout, L"%ld", Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(unsigned int const &Data) throw(Error::System &)
{ 
	CheckOutput();
#ifdef WINDOWS
	fwprintf(stdout, L"%u", Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(long unsigned int const &Data) throw(Error::System &)
{ 
	CheckOutput();
#ifdef WINDOWS
	fwprintf(stdout, L"%lu", Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(float const &Data) throw(Error::System &)
{ 
	CheckOutput();
#ifdef WINDOWS
	fwprintf(stdout, L"%f", Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(double const &Data) throw(Error::System &)
{ 
	CheckOutput();
#ifdef WINDOWS
	fwprintf(stdout, L"%f", Data);
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(String const &Data) throw(Error::System &)
{ 
	CheckOutput();
#ifdef WINDOWS
	NativeString NativeData(AsNativeString(Data));
	fwprintf(stdout, L"%s", NativeData.c_str());
#else
	std::cout << Data; 
#endif
	return *this;
}

OutputStream &StandardStreamTag::operator <<(HexToken const &Data) throw(Error::System &)
{
	CheckOutput(); 
#ifdef WINDOWS
	for (unsigned int CurrentPosition = 0; CurrentPosition < Data.Size; CurrentPosition++)
		fwprintf(stdout, L"%02x", *((unsigned char *)Data.Data + CurrentPosition));
#else
	std::cout << std::hex << std::setfill('0') << std::setw(2);
	for (unsigned int CurrentPosition = 0; CurrentPosition < Data.Size; CurrentPosition++)
		std::cout << *((unsigned char *)Data.Data + CurrentPosition); 
	std::cout << std::dec;
#endif
}

InputStream &StandardStreamTag::operator >>(RawToken &Data) throw(Error::System &)
{ 
	CheckInput(); 
	std::cin.read(reinterpret_cast<char *>(Data.Data), Data.Size); 
	return *this; 
}

InputStream &StandardStreamTag::operator >>(String &Data) throw(Error::System &)
	{ CheckInput(); std::getline(std::cin, Data); return *this; }

void StandardStreamTag::CheckOutput(void) throw(Error::System &)
	{ if (!std::cout.good()) throw Error::System("Standard output has failed!"); }
	
void StandardStreamTag::CheckOutput(void) throw(Error::System &)
	{ if (!std::coin.good()) throw Error::System("Standard input has failed!"); }

OutputStream &StandardErrorStreamTag::operator <<(FlushToken const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout << std::flush; return *this; }
	
OutputStream &StandardErrorStreamTag::operator <<(RawToken const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout.write(reinterpret_cast<char const *>(Data.Data), Data.Size); return *this; }
	
OutputStream &StandardErrorStreamTag::operator <<(char const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout << Data; return *this; }
	
OutputStream &StandardErrorStreamTag::operator <<(bool const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout << Data; return *this; }

OutputStream &StandardErrorStreamTag::operator <<(int const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout << Data; return *this; }

OutputStream &StandardErrorStreamTag::operator <<(long int const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout << Data; return *this; }

OutputStream &StandardErrorStreamTag::operator <<(long unsigned int const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout << Data; return *this; }

OutputStream &StandardErrorStreamTag::operator <<(unsigned int const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout << Data; return *this; }

OutputStream &StandardErrorStreamTag::operator <<(float const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout << Data; return *this; }

OutputStream &StandardErrorStreamTag::operator <<(double const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout << Data; return *this; }

OutputStream &StandardErrorStreamTag::operator <<(String const &Data) throw(Error::System &)
	{ CheckOutput(); std::cout << Data; return *this; }	
	
void StandardErrorStreamTag::CheckOutput(void) throw(Error::System &)
	{ if (!std::cerr.good()) throw Error::System("Standard error output has failed!"); }

class FileOutput : public OutputStream
{
	public:
		enum Modes
		{
			Erase = 1 << 0,
			Append = 1 << 1 
		};
FileOutput::FileOutput(String const &Filename, unsigned int Mode = 0) throw(Error::System &)
#ifdef WINDOWS
	File(_wfopen(reinterpret_cast<wchar_t const *>(AsNativeString(Filename).c_str()), 
		Mode & Erase ?
			Mode & Append ? L"wa" :
			L"w" :
		L"a"));
{ 
	if (File == nullptr) throw Error::System("Couldn't open file " + Filename);
FileOutput::FileOutput(FileOutput &&Other) throw();
OutputStream &FileOutput::operator <<(FlushToken const &Data) throw(Error::System &);
OutputStream &FileOutput::operator <<(RawToken const &Data) throw(Error::System &);
OutputStream &FileOutput::operator <<(char const &Data) throw(Error::System &);
OutputStream &FileOutput::operator <<(bool const &Data) throw(Error::System &);
OutputStream &FileOutput::operator <<(int const &Data) throw(Error::System &);
OutputStream &FileOutput::operator <<(long int const &Data) throw(Error::System &);
OutputStream &FileOutput::operator <<(long unsigned int const &Data) throw(Error::System &);
OutputStream &FileOutput::operator <<(unsigned int const &Data) throw(Error::System &);
OutputStream &FileOutput::operator <<(float const &Data) throw(Error::System &);
OutputStream &FileOutput::operator <<(double const &Data) throw(Error::System &);
OutputStream &FileOutput::operator <<(String const &Data) throw(Error::System &);
OutputStream &FileOutput::operator <<(HexToken const &Data) throw(Error::System &);
		
};

class FileInput : public InputStream
{
	public:
		FileInput(String const &Filename, unsigned int Mode = 0) throw(Error::System &);
		FileInput(FileInput &&Other) throw();
		InputStream &operator >>(RawToken &Data) throw(Error::System &);
		InputStream &operator >>(String &Data) throw(Error::System &);
	private:
#ifdef WINDOWS
		FILE *File;
#else
		std::ifstream File;
#endif
};

class MemoryStream : public OutputStream, public InputStream
{
	public:
		MemoryStream(void) throw();
		OutputStream &operator <<(FlushToken const &Data) throw(Error::System &);
		OutputStream &operator <<(RawToken const &Data) throw(Error::System &);
		OutputStream &operator <<(char const &Data) throw(Error::System &);
		OutputStream &operator <<(bool const &Data) throw(Error::System &);
		OutputStream &operator <<(int const &Data) throw(Error::System &);
		OutputStream &operator <<(long int const &Data) throw(Error::System &);
		OutputStream &operator <<(long unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(float const &Data) throw(Error::System &);
		OutputStream &operator <<(double const &Data) throw(Error::System &);
		OutputStream &operator <<(String const &Data) throw(Error::System &);
		OutputStream &operator <<(HexToken const &Data) throw(Error::System &);
		InputStream &operator >>(RawToken &Data) throw(Error::System &);
		InputStream &operator >>(String &Data) throw(Error::System &); // Reads a line
};
