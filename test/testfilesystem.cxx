#include <ren-general/filesystem.h>

#include <cassert>
#include <iostream>

inline void AssertEqual(String const &Got, String const &Expected)
{
	//std::cout << "Checking \"" << Got << "\" against \"" << Expected << "\"" << std::endl;
	assert(Got == Expected);
}

inline void AssertEqual(unsigned int Got, unsigned int Expected)
{
	//std::cout << "Checking " << Got << " against " << Expected << std::endl;
	assert(Got == Expected);
}

inline void AssertEqual(std::list<String> Got, std::list<String> Expected)
{
	Got.sort();
	Expected.sort();
	//std::cout << "Checking string lists:" << std::endl;
	//std::cout << "\tGot:";
	//for (auto &Element : Got) std::cout << " \"" << Element << "\"";
	//std::cout << std::endl;
	//std::cout << "\tExpected:";
	//for (auto &Element : Expected) std::cout << " \"" << Element << "\"";
	//std::cout << std::endl;
	assert(Got.size() == Expected.size());
	std::list<String>::const_iterator ExpectedElement = Expected.begin();
	for (auto &GotElement : Got)
	{
		assert(GotElement == *ExpectedElement);
		++ExpectedElement;
	}
}

inline void AssertTrue(bool Expression)
{
	assert(Expression);
}

int main(int argc, char **argv)
{
#ifdef WINDOWS
	String Prefix(u8"c:");
#else
	String Prefix(u8"");
#endif
	AssertEqual(Path(Prefix + u8"/").AsAbsoluteString(), Prefix + u8"/");
	AssertTrue(Path(Prefix + u8"/").IsRoot());
	AssertEqual(Path(Prefix + u8"/").Depth(), 0);
	AssertEqual(Path(Prefix + u8"/a").AsAbsoluteString(), Prefix + u8"/a");
	AssertTrue(!Path(Prefix + u8"/a").IsRoot());
	AssertEqual(Path(Prefix + u8"/a").Depth(), 1);
	AssertEqual(Path(Prefix + u8"/a/").AsAbsoluteString(), Prefix + u8"/a");
	AssertEqual(Path(Prefix + u8"/a/").Depth(), 1);
	AssertEqual(Path(Prefix + u8"/a/1").AsAbsoluteString(), Prefix + u8"/a/1");
	AssertEqual(Path(Prefix + u8"/a/1").Depth(), 2);
	AssertEqual(Path(Prefix + u8"/a/1/").AsAbsoluteString(), Prefix + u8"/a/1");
	AssertEqual(Path(Prefix + u8"/./").AsAbsoluteString(), Prefix + u8"/");
	AssertEqual(Path(Prefix + u8"/a/./").AsAbsoluteString(), Prefix + u8"/a");
	AssertEqual(Path(Prefix + u8"/a/./1").AsAbsoluteString(), Prefix + u8"/a/1");
	AssertEqual(Path(Prefix + u8"/a/..").AsAbsoluteString(), Prefix + u8"/");
	AssertEqual(Path(Prefix + u8"/a/../1").AsAbsoluteString(), Prefix + u8"/1");
	AssertEqual(Path(Prefix + u8"/").AsRelativeString(DirectoryPath(Prefix + u8"/")), Prefix + u8"");
	AssertEqual(Path(Prefix + u8"/a").AsRelativeString(DirectoryPath(Prefix + u8"/")), Prefix + u8"a");
	AssertEqual(Path(Prefix + u8"/").AsRelativeString(DirectoryPath(Prefix + u8"/b")), Prefix + u8"..");
	AssertEqual(Path(Prefix + u8"/a").AsRelativeString(DirectoryPath(Prefix + u8"/b")), Prefix + u8"../a");
	AssertEqual(Path(Prefix + u8"/a/1").AsRelativeString(DirectoryPath(Prefix + u8"/b")), Prefix + u8"../a/1");
	AssertEqual(Path(Prefix + u8"/a").AsRelativeString(DirectoryPath(Prefix + u8"/b/1")), Prefix + u8"../../a");
	AssertEqual(DirectoryPath(Prefix + u8"/").FindCommonRoot(Prefix + u8"/").AsAbsoluteString(), Prefix + u8"/");
	AssertEqual(DirectoryPath(Prefix + u8"/a").FindCommonRoot(Prefix + u8"/").AsAbsoluteString(), Prefix + u8"/");
	AssertEqual(DirectoryPath(Prefix + u8"/").FindCommonRoot(Prefix + u8"/a").AsAbsoluteString(), Prefix + u8"/");
	AssertEqual(DirectoryPath(Prefix + u8"/a").FindCommonRoot(Prefix + u8"/b").AsAbsoluteString(), Prefix + u8"/");
	AssertEqual(DirectoryPath(Prefix + u8"/a/1").FindCommonRoot(Prefix + u8"/a/2").AsAbsoluteString(), Prefix + u8"/a");
	AssertEqual(FilePath(Prefix + u8"/c.txt").File(), u8"c.txt");
	AssertEqual(FilePath(Prefix + u8"/c.txt").Directory().AsAbsoluteString(), Prefix + u8"/");
	AssertEqual(FilePath(Prefix + u8"/a/c.txt").File(), u8"c.txt");
	AssertEqual(FilePath(Prefix + u8"/a/c.txt").Directory().AsAbsoluteString(), Prefix + u8"/a");
	String Unicode1(u8"\xE5\xAD\x90\xE4\xBE\x9B"), Unicode2(u8"\xE5\xA4\xA7\x20\xE4\xBA\xBA"), Unicode3(u8"\xE3\x83\x95\xE3\x82\xA6\xE3\x83\x81\xE3\x83\xA7\xE3\x82\xA6\xE7\xA7\x91");
	FilePath UnicodePath(Prefix + u8"/" + Unicode1 + u8"/" + Unicode2 + u8"/" + Unicode3 + u8".txt");
	AssertEqual(UnicodePath.File(), Unicode3 + u8".txt");
	AssertEqual(DirectoryPath(u8"/").Enter(Unicode1).AsAbsoluteString(), Prefix + u8"/" + Unicode1);
	AssertEqual(LocateWorkingDirectory().Enter(u8"filesystemtesttree").ListDirectories(), {"a", "b"});
	AssertEqual(LocateWorkingDirectory().Enter(u8"filesystemtesttree").Enter(u8"a").ListFiles(), {"1.txt", "2.txt", "3.txt"});
	std::list<String> DiscoveredFiles;
	LocateWorkingDirectory().Enter(u8"filesystemtesttree").Walk([&](FilePath const &File) { DiscoveredFiles.push_back(File.File()); });
	AssertEqual(DiscoveredFiles, {"1.txt", "2.txt", "3.txt", "4.txt", "5.txt", "6.txt"});
	FilePath UnicodeFile = LocateWorkingDirectory().Select(Unicode1);
	{ 
		//FileOutput UnicodeFileOutput(UnicodeFile); // Not supported by GCC right now?
		FileOutput UnicodeFileOutput(AsNativeString(UnicodeFile)); 
		AssertTrue(!!UnicodeFileOutput);
	}
	//AssertTrue(UnicodeFile.Delete());
	//FilePath Unicode2File = LocateWorkingDirectory().Select(Unicode2);
	//FileOutput Unicode2Output(AsNativeString(Unicode2File));
}

