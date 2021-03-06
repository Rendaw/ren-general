#include "filesystem.h"

#include <cassert>
#include <unistd.h>
#include <cstring>

#ifdef WINDOWS
#include <windows.h>
#include <wchar.h>
#include <direct.h>
#include <shlobj.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

#include "exception.h"
#include "arrangement.h"

// My policy on case insensitivity on Windows: pretend it doesn't exist.  If two paths with different cases are compared, subsetted, whatever, they will be considered inequivalent.

static bool IsAbsolute(String const &RawPath)
{
#ifdef WINDOWS
	if ((RawPath.size() < 2) || (RawPath[1] != ':'))
		return false;
#else
	if (RawPath[0] != '/')
		return false;
#endif
	return true;
}

class PathStringIterator
{
	public:
		PathStringIterator(String const &PathString) :
			PathString(PathString), PreviousMarker(0), Marker(0)
			{}

		bool FindNext(void)
		{
			if (Marker == String::npos) return false;

			PreviousMarker = Marker;

			Marker = PathString.find(u8"/", PreviousMarker);
#ifdef WINDOWS
			if (Marker == String::npos)
				Marker = PathString.find(u8"\\", PreviousMarker);
#endif
			if (Marker != String::npos) Marker += 1;
			return true;
		}

		String Read(void) const
		{
			assert(Marker > 0); // FindNext must be called first
			if (Marker - 1 - PreviousMarker == 0) return String();
			return PathString.substr(PreviousMarker, Marker - 1 - PreviousMarker);
		}

	private:
		String const &PathString;
		size_t PreviousMarker, Marker;
};

Path::Path(String const &Absolute)
{
	if (Absolute.empty())
		throw Error::Construction("Absolute paths must not be empty.");

	if (!IsAbsolute(Absolute))
		throw Error::Construction("Base paths must be constructed with absolute paths.");
	
	PathStringIterator AbsoluteIterator(Absolute);

	while (AbsoluteIterator.FindNext())
	{
		String Part = AbsoluteIterator.Read();
		if (Part == u8"")
			continue;
		else if (Part == u8"..")
		{
			if (Parts.empty()) throw Error::Construction(".. directory specified at root level!");
#ifdef WINDOWS
			if (Parts.size() == 1) throw Error::Construction(".. directory specified at root level!");
#endif
			Parts.pop_back();
		}
		else if (Part == u8".")
			continue;
		else Parts.push_back(Part);
	}
}

Path::Path(Path const &Other) : Parts(Other.Parts) {}

Path::~Path(void) {}

String Path::AsAbsoluteString(char const *Separator) const
{
	MemoryStream Out;
#ifdef WINDOWS
	bool First = true;
#else
	if (Parts.empty())
		Out << Separator;
#endif

	for (auto Part : Parts)
	{
#ifdef WINDOWS
		if (First) First = false;
		else Out << Separator;
		Out << Part;
#else
		Out << Separator << Part;
#endif
	}

#ifdef WINDOWS
	if (Parts.size() == 1)
		Out << Separator;
#endif
	return Out;
}

Path::operator String(void) const
	{ return AsAbsoluteString(); }

String Path::AsRelativeString(DirectoryPath const &From) const
{
	MemoryStream Out;
	bool First = true;
	auto AppendPart = [&Out, &First](String const &Part)
	{
		if (First) First = false;
		else Out << u8"/";
		Out << Part;
	};

	PartCollection::const_iterator HerePart = Parts.begin(), FromPart = From.Parts.begin();
	FindCommonRoot(From.Parts, HerePart, FromPart);
#ifdef WINDOWS
	if (FromPart == From.Parts.begin())
		return AsAbsoluteString();
#endif
	for (; FromPart != From.Parts.end(); FromPart++)
		AppendPart(u8"..");
	for (; HerePart != Parts.end(); HerePart++)
		AppendPart(*HerePart);

	return Out;
}

bool Path::IsRoot(void) const
{
#ifdef WINDOWS
	assert(!Parts.empty());
	return Parts.size() <= 1;
#else
	return Parts.empty();
#endif
}

unsigned int Path::Depth(void) const
{
#ifdef WINDOWS
	assert(!Parts.empty());
	return Parts.size() - 1;
#else
	return Parts.size();
#endif
}

Path::Path(PartCollection const &Parts) : Parts(Parts) {}

Path::PartCollection Path::FindCommonRoot(PartCollection const &OtherParts, PartCollection::const_iterator &LocalDivergence, PartCollection::const_iterator &OtherDivergence) const
{
	PartCollection::const_iterator &LocalPart = LocalDivergence, &OtherPart = OtherDivergence;
	PartCollection Out;
	while ((LocalPart != Parts.end()) && (OtherPart != OtherParts.end()))
	{
		if (*LocalPart != *OtherPart) break;
		Out.push_back(*LocalPart);
		++LocalPart;
		++OtherPart;
	}
	return std::move(Out);
}

FilePath::FilePath(String const &Absolute) : Path(Absolute) {}
		
FilePath FilePath::Qualify(String const &RawPath)
{
	if (IsAbsolute(RawPath))
		return FilePath(RawPath);
	return FilePath(LocateWorkingDirectory().AsAbsoluteString() + "/" + RawPath);
}

String FilePath::File(void) const { return Parts.back(); }

DirectoryPath FilePath::Directory(void) const { return DirectoryPath(PartCollection(Parts.begin(), --Parts.end())); }
#include <iostream>
#include <iomanip>
bool FilePath::Exists(void) const
{
#ifdef WINDOWS
	//DWORD Attributes = GetFileAttributesW(reinterpret_cast<wchar_t const *>(AsNativeString("\\\\?\\" + AsAbsoluteString()).c_str())); // Doesn't work for some reason -- mixed slashes?
	DWORD Attributes = GetFileAttributesW(reinterpret_cast<wchar_t const *>(AsNativeString(AsAbsoluteString()).c_str()));
        return !Set<DWORD>({0xFFFFFFFF, 0x10}).Contains(Attributes);
#else
	struct stat StatResultBuffer;
	int Result = stat(AsAbsoluteString().c_str(), &StatResultBuffer);
	if (Result != 0) return false;
	return S_ISREG(StatResultBuffer.st_mode);
#endif
}

FileInput FilePath::Read(void) const 
	{ return FileInput(AsAbsoluteString()); }

FileOutput FilePath::Write(bool Append, bool Truncate) const
	{ return FileOutput(AsAbsoluteString(), (Append ? FileOutput::Append : 0) | (Truncate ? FileOutput::Erase : 0)); }

FilePath::operator FileInput(void) const { return Read(); }

FilePath::operator FileOutput(void) const { return Write(); }

bool FilePath::Delete(void) const
{
#ifdef WINDOWS
	return _wunlink(reinterpret_cast<wchar_t const *>(AsNativeString(AsAbsoluteString()).c_str())) == 0;
#else
	return unlink(AsAbsoluteString().c_str()) == 0;
#endif
}

FilePath::FilePath(Path::PartCollection const &Parts, String const &Filename) : Path(Parts)
	{ this->Parts.push_back(Filename); }

DirectoryPath::DirectoryPath(void) : Path(Path::PartCollection()) {}

DirectoryPath DirectoryPath::Qualify(String const &RawPath)
{
	if (IsAbsolute(RawPath))
		return DirectoryPath(RawPath);
	return DirectoryPath(LocateWorkingDirectory().AsAbsoluteString() + "/" + RawPath);
}

DirectoryPath::DirectoryPath(String const &Absolute) : Path(Absolute) {}
		
bool DirectoryPath::Exists(void) const
{
#ifdef WINDOWS
        return GetFileAttributesW(reinterpret_cast<wchar_t const *>(AsNativeString("\\\\?\\" + AsAbsoluteString()).c_str())) & 0x10;
#else
	struct stat StatResultBuffer;
	int Result = stat(AsAbsoluteString().c_str(), &StatResultBuffer);
	if (Result != 0) return false;
	return S_ISDIR(StatResultBuffer.st_mode);
#endif
}

bool DirectoryPath::Create(bool EnsureAncestors) const
{
	auto MakeSingleDirectory = [](DirectoryPath const &Ancestor) -> bool
	{
#ifdef WINDOWS
		int Result = _wmkdir(reinterpret_cast<wchar_t const *>(AsNativeString(Ancestor.AsAbsoluteString()).c_str()));
#else
		int Result = mkdir(((String)Ancestor).c_str(), 0777);
#endif
		if (Result == -1 && errno != EEXIST)
			return false;
		return true;
	};

	if (EnsureAncestors)
	{
		DirectoryPath Ancestor;
		for (Path::PartCollection::const_iterator CurrentPart = Parts.begin(); CurrentPart != Parts.end(); CurrentPart++)
		{
			Ancestor.Enter(*CurrentPart);
			if (!MakeSingleDirectory(Ancestor))
				return false;
		}
		return true;
	}
	else
	{
		return MakeSingleDirectory(*this);
	}
}

DirectoryPath &DirectoryPath::Exit(void)
{
	assert(!IsRoot());
	Parts.pop_back();
	return *this;
}

DirectoryPath &DirectoryPath::Enter(String const &Directory)
{
	Parts.push_back(Directory);
	return *this;
}

FilePath DirectoryPath::Select(String const &File) const
	{ return FilePath(Parts, File); }

static void ProcessDirectoryContents(DirectoryPath const &DirectoryName, std::function<void(String const &Element, bool IsFile)> Process)
{
#ifdef WINDOWS
	WIN32_FIND_DATAW ElementInfo;
	HANDLE DirectoryResource;
	DirectoryResource = FindFirstFileW(reinterpret_cast<wchar_t const *>(AsNativeString(DirectoryName.AsAbsoluteString("\\") + "\\*").c_str()), &ElementInfo);
	if (DirectoryResource == INVALID_HANDLE_VALUE) return;
	do
	{
		String FindName = AsString(NativeString(reinterpret_cast<char16_t const *>(ElementInfo.cFileName)));
		if (Set<String>({".", ".."}).Contains(FindName)) continue;

		Process(FindName, ElementInfo.dwFileAttributes & ~FILE_ATTRIBUTE_DIRECTORY);
	} while (FindNextFileW(DirectoryResource, &ElementInfo) != 0);
	FindClose(DirectoryResource);
#else
	DIR *DirectoryResource = opendir(((String)DirectoryName).c_str());
	if (DirectoryResource == nullptr) return;

	dirent *ElementInfo;
	while ((ElementInfo = readdir(DirectoryResource)) != nullptr)
	{
		String ElementName(ElementInfo->d_name);
		if ((ElementName == ".") || (ElementName == "..")) continue;
		Process(std::move(ElementName), ElementInfo->d_type != DT_DIR);
	}

	closedir(DirectoryResource);
#endif
}

std::list<String> DirectoryPath::ListFiles(void) const
{
	std::list<String> Out;
	ProcessDirectoryContents(*this, [&](String const &Element, bool IsFile) { if (IsFile) Out.push_back(Element); });
	return std::move(Out);
}

std::list<String> DirectoryPath::ListDirectories(void) const
{
	std::list<String> Out;
	ProcessDirectoryContents(*this, [&](String const &Element, bool IsFile) { if (!IsFile) Out.push_back(Element); });
	return std::move(Out);
}

void DirectoryPath::Walk(std::function<void(FilePath const &File)> const &Process) const
{
	std::list<String> Transitions;
	DirectoryPath Marker(*this);

	auto ProcessFiles = [&]()
		{ for (auto &File : Marker.ListFiles()) Process(Marker.Select(File)); };

	auto QueueNewTransitions = [&]()
	{
		// Add new transitions from directories in current directory
		std::list<String> Directories = Marker.ListDirectories();
		Transitions.insert(Transitions.begin(), Directories.begin(), Directories.end());
	};

	// Perform a depth first exploration of the filesystem subtree.  Files are processed before directories.
	ProcessFiles();
	QueueNewTransitions();

	while (!Transitions.empty())
	{
		String NextTransition = Transitions.front();
		Transitions.pop_front();

		// If there's an empty transition, move back up
		if (NextTransition.empty())
		{
			Marker.Exit();
			continue;
		}

		// Otherwise, enter the next directory and process it.
		Marker.Enter(NextTransition);
		Transitions.push_front("");
		QueueNewTransitions();
		ProcessFiles();
	}
}

DirectoryPath DirectoryPath::FindCommonRoot(DirectoryPath const &Other) const
{
	Path::PartCollection::const_iterator Part(Parts.begin()), OtherPart(Other.Parts.begin());
	return DirectoryPath(Path::FindCommonRoot(Other.Parts, Part, OtherPart));
}

DirectoryPath::DirectoryPath(Path::PartCollection const &Parts) : Path(Parts) {}

DirectoryPath LocateWorkingDirectory(void)
{
	 char WorkingDirectoryBuffer[FILENAME_MAX];
	 if (!getcwd(WorkingDirectoryBuffer, sizeof(WorkingDirectoryBuffer)))
		 throw Error::System("Couldn't obtain working directory!");
	 return DirectoryPath(WorkingDirectoryBuffer);
}

void ChangeWorkingDirectory(DirectoryPath const &Target)
{
#ifdef WINDOWS
	if (!SetCurrentDirectoryW(reinterpret_cast<wchar_t const *>(AsNativeString(Target).c_str())))
		throw Error::System("Couldn't change working directory!");
#else
	if (chdir(Target.AsAbsoluteString().c_str()) == -1)
		throw Error::System(String("Couldn't change working directory to \"") + Target.AsAbsoluteString() + "\": " + strerror(errno));
#endif
}

static String GetUserConfigDirectory(void)
{
#ifdef WINDOWS
	wchar_t PathResult[MAX_PATH];
	HRESULT Result = SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, PathResult);
	if (Result != S_OK)
		throw Error::System("Couldn't find user config directory!  Received error " + AsString(Result));
	return AsString(NativeString(reinterpret_cast<char16_t const *>(PathResult)));
#else
	char *HomePath = getenv("XDG_CONFIG_HOME");
	if (HomePath != nullptr)
		return HomePath;
	HomePath = getenv("HOME");
	if (HomePath != nullptr)
		return String(HomePath) + "/.config";

	throw Error::System("User's local config directory and home directory are undefined!");
	return String();
#endif
}

static String GetGlobalConfigDirectory(void)
{
#ifdef WINDOWS
	wchar_t PathResult[MAX_PATH];
	HRESULT Result = SHGetFolderPathW(nullptr, CSIDL_COMMON_APPDATA, nullptr, 0, PathResult);
	if (Result != S_OK)
		throw Error::System("Couldn't find global config directory!  Received error " + AsString(Result));
	return AsString(NativeString(reinterpret_cast<char16_t const *>(PathResult)));
#else
	return String(u8"/etc");
#endif
}

FilePath LocateUserConfigFile(String const &Filename)
	{ return DirectoryPath(GetUserConfigDirectory()).Select(Filename); }

FilePath LocateUserConfigFile(String const &Project, String const &Filename)
	{ return DirectoryPath(GetUserConfigDirectory()).Enter(Project).Select(Filename); }

FilePath LocateGlobalConfigFile(String const &Filename)
	{ return DirectoryPath(GetGlobalConfigDirectory()).Select(Filename); }

FilePath LocateGlobalConfigFile(String const &Project, String const &Filename)
	{ return DirectoryPath(GetGlobalConfigDirectory()).Enter(Project).Select(Filename); }

DirectoryPath LocateDataDirectory(void)
{
#ifdef RENGENERAL_FILESYSTEM_DATALOCATION
	return DirectoryPath::Qualify(RENGENERAL_FILESYSTEM_DATALOCATION);
#else
#ifdef WINDOWS
	wchar_t PathResult[MAX_PATH];
	DWORD Result = GetModuleFileNameW(nullptr, PathResult, MAX_PATH);
	if (Result == 0)
		throw Error::System("Couldn't locate executable directory!  Received error " + AsString(GetLastError()));
	return FilePath::Qualify(AsString(NativeString(reinterpret_cast<char16_t const *>(PathResult)))).Directory();
#else
	return DirectoryPath::Qualify(".");
#endif
#endif
}

DirectoryPath LocateDocumentDirectory(void)
{
#ifdef WINDOWS
	wchar_t PathResult[MAX_PATH];
	HRESULT Result = SHGetFolderPathW(nullptr, CSIDL_PROFILE, nullptr, 0, PathResult);
	if (Result != S_OK)
		throw Error::System("Couldn't find user document directory!  Received error " + AsString(Result));
	return DirectoryPath(AsString(NativeString(reinterpret_cast<char16_t const *>(PathResult))));
#else
	char *HomePath = getenv("HOME");
	if (HomePath == nullptr)
		throw Error::System("User's home directory is undefined!");
	return DirectoryPath(HomePath);
#endif
}

DirectoryPath LocateDocumentDirectory(String const &Project)
	{ return LocateDocumentDirectory().Enter(Project); }

DirectoryPath LocateTemporaryDirectory(void)
{
#ifdef WINDOWS
	wchar_t TemporaryPath[MAX_PATH];
	int Result = GetTempPathW(MAX_PATH, TemporaryPath);
	if (Result == 0)
		throw Error::System("Could not find the temporary file directory!");
	return DirectoryPath(AsString(NativeString(reinterpret_cast<char16_t const *>(TemporaryPath))));
#else
	char const *TemporaryPath = getenv("TMPDIR");
	if (TemporaryPath == nullptr) TemporaryPath = getenv("P_tmpdir");
	if (TemporaryPath == nullptr) TemporaryPath = "/tmp";
	return DirectoryPath(TemporaryPath);
#endif
}

std::tuple<FilePath, FileOutput> CreateTemporaryFile(DirectoryPath const &TempDirectory)
{
#ifdef WINDOWS
	NativeString TempDirectoryString = AsNativeString(TempDirectory.AsAbsoluteString("\\"));
	std::vector<wchar_t> TemporaryFilename;
	TemporaryFilename.resize(MAX_PATH);
	int Result = GetTempFileNameW(reinterpret_cast<wchar_t const *>(TempDirectoryString.c_str()), L"zar", 0, &TemporaryFilename[0]);
	if (Result == 0)
		throw Error::System("Could not determine a temporary filename!");
	Result = GetLongPathNameW(&TemporaryFilename[0], &TemporaryFilename[0], TemporaryFilename.size());
	assert(Result >= 0);
	if ((unsigned int)Result > TemporaryFilename.size())
	{
		TemporaryFilename.resize(Result);
		Result = GetLongPathNameW(&TemporaryFilename[0], &TemporaryFilename[0], TemporaryFilename.size());
		assert((unsigned int)Result <= TemporaryFilename.size());
	}
	if (Result == 0)
		throw Error::System("Could not qualify the temporary filename!");
	NativeString NativeTemporaryFilename(reinterpret_cast<char16_t const *>(&TemporaryFilename[0]));
	return std::make_tuple(FilePath(AsString(NativeTemporaryFilename)), FileOutput(AsString(NativeTemporaryFilename), FileOutput::Erase));
#else
	String Template = TempDirectory.AsAbsoluteString() + "/XXXXXX";
	std::vector<char> Filename(Template.size() + 1);
	std::copy(Template.begin(), Template.end(), Filename.begin());
	Filename[Template.size()] = '\0';
	int Result = mkstemp(&Filename[0]);
	if (Result == -1)
		throw Error::System("Failed to locate temporary file in " + TempDirectory.AsAbsoluteString() + "!");
	close(Result);
	return std::make_tuple(FilePath(&Filename[0]), FileOutput(&Filename[0], FileOutput::Erase));
#endif
}

