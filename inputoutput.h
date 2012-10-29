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
		
		virtual ~OutputStream(void);
		virtual OutputStream &operator <<(FlushToken const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(RawToken const &Data) throw(Error::System &) = 0;
		//virtual OutputStream &operator <<(bool const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(int const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(long int const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(long unsigned int const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(unsigned int const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(float const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(double const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(char const *Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(String const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(Path const &Data) throw(Error::System &);
		virtual OutputStream &operator <<(HexToken const &Data) throw(Error::System &) = 0;
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
		virtual InputStream &operator >>(RawToken &Data) throw(Error::System &) = 0;
		virtual InputStream &operator >>(int &Data) throw(Error::System &);
		virtual InputStream &operator >>(unsigned int &Data) throw(Error::System &);
		virtual InputStream &operator >>(String &Data) throw(Error::System &) = 0; // Reads a line
};

class StandardStreamTag : public OutputStream, public InputStream
{
	public:
		using OutputStream::operator <<;
		using InputStream::operator >>;
		OutputStream &operator <<(OutputStream::FlushToken const &Data) throw(Error::System &);
		OutputStream &operator <<(OutputStream::RawToken const &Data) throw(Error::System &);
		//OutputStream &operator <<(bool const &Data) throw(Error::System &);
		OutputStream &operator <<(int const &Data) throw(Error::System &);
		OutputStream &operator <<(long int const &Data) throw(Error::System &);
		OutputStream &operator <<(long unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(float const &Data) throw(Error::System &);
		OutputStream &operator <<(double const &Data) throw(Error::System &);
		inline OutputStream &operator <<(char const *Data) throw(Error::System &)
			{ *this << String(Data); return *this; }
		OutputStream &operator <<(String const &Data) throw(Error::System &);
		OutputStream &operator <<(OutputStream::HexToken const &Data) throw(Error::System &);
		InputStream &operator >>(InputStream::RawToken &Data) throw(Error::System &);
		InputStream &operator >>(String &Data) throw(Error::System &); // Reads a line
	private:
		void CheckOutput(void) throw(Error::System &);
		void CheckInput(void) throw(Error::System &);
};

extern StandardStreamTag StandardStream;

class StandardErrorStreamTag : public OutputStream
{
	public:
		using OutputStream::operator <<;
		OutputStream &operator <<(OutputStream::FlushToken const &Data) throw(Error::System &);
		OutputStream &operator <<(OutputStream::RawToken const &Data) throw(Error::System &);
		//OutputStream &operator <<(bool const &Data) throw(Error::System &);
		OutputStream &operator <<(int const &Data) throw(Error::System &);
		OutputStream &operator <<(long int const &Data) throw(Error::System &);
		OutputStream &operator <<(long unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(float const &Data) throw(Error::System &);
		OutputStream &operator <<(double const &Data) throw(Error::System &);
		inline OutputStream &operator <<(char const *Data) throw(Error::System &)
			{ *this << String(Data); return *this; }
		OutputStream &operator <<(String const &Data) throw(Error::System &);
		OutputStream &operator <<(OutputStream::HexToken const &Data) throw(Error::System &);
	private:
		void CheckOutput(void) throw(Error::System &);
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
		FileOutput(String const &Filename, unsigned int Mode = 0) throw(Error::System &);
		FileOutput(FileOutput &&Other) throw();
		FileOutput &operator =(FileOutput &&Other) throw();
		~FileOutput(void) throw();
		OutputStream &operator <<(OutputStream::FlushToken const &Data) throw(Error::System &);
		OutputStream &operator <<(OutputStream::RawToken const &Data) throw(Error::System &);
		OutputStream &operator <<(char const &Data) throw(Error::System &);
		//OutputStream &operator <<(bool const &Data) throw(Error::System &);
		OutputStream &operator <<(int const &Data) throw(Error::System &);
		OutputStream &operator <<(long int const &Data) throw(Error::System &);
		OutputStream &operator <<(long unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(float const &Data) throw(Error::System &);
		OutputStream &operator <<(double const &Data) throw(Error::System &);
		inline OutputStream &operator <<(char const *Data) throw(Error::System &)
			{ *this << String(Data); return *this; }
		OutputStream &operator <<(String const &Data) throw(Error::System &);
		OutputStream &operator <<(OutputStream::HexToken const &Data) throw(Error::System &);
	private:
		void CheckOutput(void) throw(Error::System &);
		void CheckWriteResult(size_t Result) throw(Error::System &);

		FILE *File;
};

class FileInput : public InputStream
{
	public:
		using InputStream::operator >>;

		FileInput(String const &Filename) throw(Error::System &);
		FileInput(FileInput &&Other) throw();
		FileInput &operator =(FileInput &&Other) throw();
		InputStream &operator >>(InputStream::RawToken &Data) throw(Error::System &);
		InputStream &operator >>(String &Data) throw(Error::System &);
	private:
		void CheckInput(void) throw(Error::System &);
		void CheckReadResult(size_t Result) throw(Error::System &);
		FILE *File;
};

class MemoryStream : public OutputStream, public InputStream
{
	public:
		using OutputStream::operator <<;
		using InputStream::operator >>;

		MemoryStream(void);
		MemoryStream(String const &InitialData);
		OutputStream &operator <<(OutputStream::FlushToken const &Data) throw(Error::System &);
		OutputStream &operator <<(OutputStream::RawToken const &Data) throw(Error::System &);
		OutputStream &operator <<(char const &Data) throw(Error::System &);
		//OutputStream &operator <<(bool const &Data) throw(Error::System &);
		OutputStream &operator <<(int const &Data) throw(Error::System &);
		OutputStream &operator <<(unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(long int const &Data) throw(Error::System &);
		OutputStream &operator <<(long unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(float const &Data) throw(Error::System &);
		OutputStream &operator <<(double const &Data) throw(Error::System &);
		inline OutputStream &operator <<(char const *Data) throw(Error::System &)
			{ *this << String(Data); return *this; }
		OutputStream &operator <<(String const &Data) throw(Error::System &);
		OutputStream &operator <<(OutputStream::HexToken const &Data) throw(Error::System &);
		InputStream &operator >>(InputStream::RawToken &Data) throw(Error::System &);
		InputStream &operator >>(String &Data) throw(Error::System &); // Reads a line
		operator String(void) const throw();
	private:
		std::stringstream Buffer;
};

#endif
