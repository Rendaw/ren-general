ifeq ($(PLATFORM),windows)
	PLATFORMFLAGS = -DWINDOWS
	EXTENSION = .exe
else
	PLATFORMFLAGS = -DLINUX
	EXTENSION = 
endif

COMPILEDEBUG = g++ \
	-pedantic -Wno-long-long -Wall -std=c++0x -ggdb -D DEBUG \
	$(CFLAGS) $(PLATFORMFLAGS) -I../../ \
	-o

test: testcollection testarrangement testfilesystem
	objects/testcollection$(EXTENSION)
	objects/testarrangement$(EXTENSION)
	objects/testfilesystem$(EXTENSION)
	
testcollection: testcollection.cxx
	$(COMPILEDEBUG) objects/testcollection$(EXTENSION) $^ ../objects/*.debug.o

testarrangement: testarrangement.cxx
	$(COMPILEDEBUG) objects/testarrangement$(EXTENSION) $^ ../objects/*.debug.o

testfilesystem: testfilesystem.cxx
	$(COMPILEDEBUG) objects/testfilesystem$(EXTENSION) $^ ../objects/*.debug.o

clean:
	rm objects/*
