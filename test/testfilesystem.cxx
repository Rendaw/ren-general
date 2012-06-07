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
	std::cout << "Checking " << Got << " against " << Expected << std::endl;
	assert(Got == Expected);
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
	String Unicode1(u8"\x5B\x50\x4F\x9B"), Unicode2(u8"\x59\x27\x4E\xBA"), Unicode3(u8"\x30\xD5\x30\xA6\x30\xC1\x30\xE7\x30\xA6\x79\xD1");
	FilePath UnicodePath(Prefix + u8"/" + Unicode1 + u8"/" + Unicode2 + u8"/" + Unicode3 + u8".txt");
	AssertEqual(UnicodePath.File(), Unicode3 + u8".txt");
	AssertEqual(DirectoryPath(u8"/").Enter(Unicode1).AsAbsoluteString(), Prefix + u8"/" + Unicode1);
}

