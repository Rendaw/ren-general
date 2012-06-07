#ifndef filesystem_h
#define filesystem_h

#include <list>

#include "string.h"

class Path;
class DirectoryPath;
class Path
{
	public:
		Path(String const &Absolute);
		Path(Path const &Other);
		virtual ~Path(void);

		virtual String AsAbsoluteString(void) const;
		virtual String AsRelativeString(DirectoryPath const &From) const;
		bool IsRoot(void) const;
		unsigned int Depth(void) const;
		
	protected:
		typedef std::list<String> PartCollection;

		Path(PartCollection const &Parts);
		PartCollection FindCommonRoot(PartCollection const &OtherParts, PartCollection::const_iterator &LocalDivergence, PartCollection::const_iterator &OtherDivergence) const;

		PartCollection Parts;
};

class FilePath : public Path
{
	public:
		FilePath(String const &AbsolutePath);

		String File(void) const;
		DirectoryPath Directory(void) const;

		InputStream &&Read(void) const;
		OutputStream &&Write(void) const;
		operator InputStream&&(void) const;
		operator OutputStream&&(void) const;

		void Delete(void) const;
	private:
		friend class DirectoryPath;
		FilePath(Path::PartCollection const &Parts, String const &Filename);
};

class DirectoryPath : public Path
{
	public:
		DirectoryPath(String const &AbsolutePath);

		DirectoryPath Exit(void) const;
		DirectoryPath Enter(String const &Directory) const;
		FilePath Select(String const &File) const;

		DirectoryPath FindCommonRoot(DirectoryPath const &Other) const;
	private:
		friend class FilePath;
		DirectoryPath(Path::PartCollection const &Parts);
};

DirectoryPath LocateWorkingDirectory(void);
FilePath LocateUserConfigFile(String const &Filename);
FilePath LocateUserConfigFile(String const &Project, String const &Filename);
FilePath LocateGlobalConfigFile(String const &Filename);
FilePath LocateGlobalConfigFile(String const &Project, String const &Filename);
DirectoryPath LocateDocumentDirectory(void);
DirectoryPath LocateDocumentDirectory(String const &Project);

#endif

