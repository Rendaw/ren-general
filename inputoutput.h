#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

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
		virtual OutputStream &operator <<(bool const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(int const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(long int const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(long unsigned int const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(unsigned int const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(float const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(double const &Data) throw(Error::System &) = 0;
		virtual OutputStream &operator <<(String const &Data) throw(Error::System &) = 0;
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
		virtual InputStream &operator >>(String &Data) throw(Error::System &) = 0; // Reads a line
};

class StandardStreamTag : public OutputStream, public InputStream
{
	public:
		OutputStream &operator <<(FlushToken const &Data) throw(Error::System &);
		OutputStream &operator <<(RawToken const &Data) throw(Error::System &);
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
	private:
		void CheckOutput(void) throw(Error::System &);
		void CheckInput(void) throw(Error::System &);
} StandardStream;

class StandardErrorStreamTag : public OutputStream
{
	public:
		OutputStream &operator <<(FlushToken const &Data) throw(Error::System &);
		OutputStream &operator <<(RawToken const &Data) throw(Error::System &);
		OutputStream &operator <<(bool const &Data) throw(Error::System &);
		OutputStream &operator <<(int const &Data) throw(Error::System &);
		OutputStream &operator <<(long int const &Data) throw(Error::System &);
		OutputStream &operator <<(long unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(unsigned int const &Data) throw(Error::System &);
		OutputStream &operator <<(float const &Data) throw(Error::System &);
		OutputStream &operator <<(double const &Data) throw(Error::System &);
		OutputStream &operator <<(String const &Data) throw(Error::System &);
		OutputStream &operator <<(HexToken const &Data) throw(Error::System &);
	private:
		void CheckOutput(void) throw(Error::System &);
} StandardErrorStream;

class FileOutput : public OutputStream
{
	public:
		enum Modes
		{
			MustCreate = 1 << 0,
			NeverCreate = 1 << 1,
			Erase = 1 << 2,
			Append = 1 << 3
		};
		FileOutput(String const &Filename, unsigned int Mode = 0) throw(Error::System &);
		FileOutput(FileOutput &&Other) throw();
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

#endif
