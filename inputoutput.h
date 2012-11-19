#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include "exception.h"

class Path;

class OutputStream
{
	public:
		struct FlushToken {};
		static FlushToken Flush(void) { return FlushToken(); }
		
		struct RawToken
		{
			void const *const Data;
			unsigned int const Length;
		};
		template <typename DataType> static RawToken Raw(DataType const &Data)
			{ return RawToken {&Data, sizeof(Data)}; }
		
		struct HexToken
		{
			void const *const Data;
			unsigned int const Length;
		};  
		template <typename DataType> static HexToken Hex(DataType const &Data) 
			{ return HexToken {&Data, sizeof(Data)}; }

		struct FloatToken
		{
			float Value;

			int FractionalCount;
			enum { Exact, Max } FractionalCountType;

			FloatToken &FractionalDigits(unsigned int Digits)
				{ FractionalCount = Digits; FractionalCountType = Exact; return *this; }

			FloatToken &MaxFractionalDigits(unsigned int Digits)
				{ FractionalCount = Digits; FractionalCountType = Max; return *this; }
		};
		static FloatToken Float(float const &Data)
			{ return FloatToken {Data, -1, FloatToken::Exact}; }
		
		virtual ~OutputStream(void);
		virtual OutputStream &operator <<(FlushToken const &Data) = 0;
		virtual OutputStream &operator <<(RawToken const &Data) = 0;
		//virtual OutputStream &operator <<(bool const &Data) = 0;
		virtual OutputStream &operator <<(int const &Data) = 0;
		virtual OutputStream &operator <<(long int const &Data) = 0;
		virtual OutputStream &operator <<(long unsigned int const &Data) = 0;
		virtual OutputStream &operator <<(unsigned int const &Data) = 0;
		virtual OutputStream &operator <<(float const &Data) = 0;
		virtual OutputStream &operator <<(FloatToken const &Data);
		virtual OutputStream &operator <<(double const &Data) = 0;
		virtual OutputStream &operator <<(char const *Data) = 0;
		virtual OutputStream &operator <<(String const &Data) = 0;
		virtual OutputStream &operator <<(Path const &Data);
		virtual OutputStream &operator <<(HexToken const &Data) = 0;
		virtual operator String(void) const;
};

class InputStream
{
	public:
		struct RawToken
		{
			void *const Data;
			unsigned int const Length;
		};
		template <typename DataType> static RawToken Raw(DataType &Data)
			{ return RawToken {&Data, sizeof(Data)}; }
			
		virtual ~InputStream(void);

		virtual InputStream &operator >>(RawToken &Data) = 0;
		virtual InputStream &operator >>(int &Data);
		virtual InputStream &operator >>(unsigned int &Data);
		virtual InputStream &operator >>(float &Data);
		virtual InputStream &operator >>(String &Data) = 0; // Reads a line
		virtual operator bool(void) const = 0;
};

class StandardStreamTag : public OutputStream, public InputStream
{
	public:
		using OutputStream::operator <<;
		using InputStream::operator >>;

		StandardStreamTag(void);

		OutputStream &operator <<(OutputStream::FlushToken const &Data);
		OutputStream &operator <<(OutputStream::RawToken const &Data);
		//OutputStream &operator <<(bool const &Data);
		OutputStream &operator <<(int const &Data);
		OutputStream &operator <<(long int const &Data);
		OutputStream &operator <<(long unsigned int const &Data);
		OutputStream &operator <<(unsigned int const &Data);
		OutputStream &operator <<(float const &Data);
		OutputStream &operator <<(double const &Data);
		inline OutputStream &operator <<(char const *Data)
			{ *this << String(Data); return *this; }
		OutputStream &operator <<(String const &Data);
		OutputStream &operator <<(OutputStream::HexToken const &Data);
		InputStream &operator >>(InputStream::RawToken &Data);
		InputStream &operator >>(String &Data); // Reads a line
		operator bool(void) const;
	private:
		void CheckOutput(void);
		void CheckInput(void);

#ifdef WINDOWS
		bool OutputIsConsole;
		HANDLE OutputHandle;
		bool InputIsConsole;
		HANDLE InputHandle;
#endif
};

extern StandardStreamTag StandardStream;

class StandardErrorStreamTag : public OutputStream
{
	public:
		StandardErrorStreamTag(void);

		using OutputStream::operator <<;
		OutputStream &operator <<(OutputStream::FlushToken const &Data);
		OutputStream &operator <<(OutputStream::RawToken const &Data);
		//OutputStream &operator <<(bool const &Data);
		OutputStream &operator <<(int const &Data);
		OutputStream &operator <<(long int const &Data);
		OutputStream &operator <<(long unsigned int const &Data);
		OutputStream &operator <<(unsigned int const &Data);
		OutputStream &operator <<(float const &Data);
		OutputStream &operator <<(double const &Data);
		inline OutputStream &operator <<(char const *Data)
			{ *this << String(Data); return *this; }
		OutputStream &operator <<(String const &Data);
		OutputStream &operator <<(OutputStream::HexToken const &Data);
	private:
		void CheckOutput(void);
#ifdef WINDOWS
		bool OutputIsConsole;
		HANDLE OutputHandle;
#endif
};

extern StandardErrorStreamTag StandardErrorStream;

class FileOutput : public OutputStream
{
	public:
		using OutputStream::operator <<;

		enum Modes
		{
			Erase = 1 << 0,
			Append = 1 << 1 
		};
		FileOutput(String const &Filename, unsigned int Mode = 0);
		FileOutput(FileOutput &&Other);
		FileOutput &operator =(FileOutput &&Other);
		~FileOutput(void);
		OutputStream &operator <<(OutputStream::FlushToken const &Data);
		OutputStream &operator <<(OutputStream::RawToken const &Data);
		OutputStream &operator <<(char const &Data);
		//OutputStream &operator <<(bool const &Data);
		OutputStream &operator <<(int const &Data);
		OutputStream &operator <<(long int const &Data);
		OutputStream &operator <<(long unsigned int const &Data);
		OutputStream &operator <<(unsigned int const &Data);
		OutputStream &operator <<(float const &Data);
		OutputStream &operator <<(double const &Data);
		inline OutputStream &operator <<(char const *Data)
			{ *this << String(Data); return *this; }
		OutputStream &operator <<(String const &Data);
		OutputStream &operator <<(OutputStream::HexToken const &Data);
	private:
		void CheckOutput(void);
		void CheckWriteResult(size_t Result);

		FILE *File;
};

class FileInput : public InputStream
{
	public:
		using InputStream::operator >>;

		FileInput(String const &Filename);
		FileInput(FileInput &&Other);
		FileInput &operator =(FileInput &&Other);
		InputStream &operator >>(InputStream::RawToken &Data);
		InputStream &operator >>(String &Data);
		operator bool(void) const;
	private:
		void CheckInput(void);
		void CheckReadResult(size_t Result);
		FILE *File;
};

class MemoryStream : public OutputStream, public InputStream
{
	public:
		using OutputStream::operator <<;
		using InputStream::operator >>;
		
		MemoryStream(void);
		MemoryStream(String const &InitialData);
		OutputStream &operator <<(OutputStream::FlushToken const &Data);
		OutputStream &operator <<(OutputStream::RawToken const &Data);
		OutputStream &operator <<(char const &Data);
		//OutputStream &operator <<(bool const &Data);
		OutputStream &operator <<(int const &Data);
		OutputStream &operator <<(unsigned int const &Data);
		OutputStream &operator <<(long int const &Data);
		OutputStream &operator <<(long unsigned int const &Data);
		OutputStream &operator <<(float const &Data);
		OutputStream &operator <<(double const &Data);
		inline OutputStream &operator <<(char const *Data)
			{ *this << String(Data); return *this; }
		OutputStream &operator <<(String const &Data);
		OutputStream &operator <<(OutputStream::HexToken const &Data);
		operator String(void) const;
		InputStream &operator >>(InputStream::RawToken &Data);
		InputStream &operator >>(String &Data); // Reads a line
		operator bool(void) const;
	private:
		std::stringstream Buffer;
};

#endif
