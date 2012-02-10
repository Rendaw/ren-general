ifeq ($(PLATFORM),windows)
	PLATFORMFLAGS = -DWINDOWS
	EXTENSION = .exe
else
	PLATFORMFLAGS = -DLINUX
	EXTENSION = 
endif

COMPILEDEBUG = g++ \
	-pedantic -Wno-long-long -Wall -std=c++0x -ggdb -D DEBUG \
	$(CFLAGS) $(PLATFORMFLAGS) \
	-o

test: testcollection testarrangement
	objects/testcollection$(EXTENSION)
	objects/testarrangement$(EXTENSION)
	
testcollection: testcollection.cxx
	$(COMPILEDEBUG) objects/testcollection$(EXTENSION) $^

testarrangement: testarrangement.cxx
	$(COMPILEDEBUG) objects/testarrangement$(EXTENSION) $^

clean:
	rm objects/*
