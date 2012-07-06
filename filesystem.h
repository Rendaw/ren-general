#ifndef filesystem_h
#define filesystem_h

#include <list>
#include <functional>

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
		operator char const *(void) const;
		operator String(void) const;
		//operator NativeString(void) const;

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

		bool Exists(void) const;

		FileInput &&Read(void) const;
		FileOutput &&Write(bool Append = false, bool Truncate = false) const;
		operator FileInput&&(void) const;
		operator FileOutput&&(void) const;

		bool Delete(void) const;
	private:
		friend class DirectoryPath;
		FilePath(Path::PartCollection const &Parts, String const &Filename);
};

class DirectoryPath : public Path
{
	public:
		DirectoryPath(void);
		DirectoryPath(String const &AbsolutePath);

		bool Create(bool EnsureAncestors) const;

		DirectoryPath &Exit(void);
		DirectoryPath &Enter(String const &Directory);
		FilePath Select(String const &File) const;

		std::list<String> ListFiles(void) const;
		std::list<String> ListDirectories(void) const;
		void Walk(std::function<void(FilePath const &File)> const &Handler) const;
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
DirectoryPath LocateTemporaryDirectory(void);

FilePath CreateTemporaryFile(DirectoryPath &TempDirectory, FileOutput &Output);

#endif

