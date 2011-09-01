NAME = general
OBJECTS = objects/annals.o objects/time.o objects/statemachine.o objects/exception.o \
	objects/range.o objects/region.o objects/color.o objects/rotation.o objects/vector.o
COMPILE_EXTRA = 
LINK_EXTRA = 

include ../library-tools/Makefile.include

# Data types
objects/color.o objects/color.debug.o: color.cxx color.h
	$(COMPILE) $@ color.cxx

objects/vector.o objects/vector.debug.o: vector.cxx vector.h
	$(COMPILE) $@ vector.cxx

objects/rotation.o objects/rotation.debug.o: rotation.cxx rotation.h vector.h
	$(COMPILE) $@ rotation.cxx

objects/range.o objects/range.debug.o: range.cxx range.h
	$(COMPILE) $@ range.cxx

objects/region.o objects/region.debug.o: region.cxx region.h
	$(COMPILE) $@ region.cxx

objects/exception.o objects/exception.debug.o: exception.cxx exception.h
	$(COMPILE) $@ exception.cxx

objects/annals.o objects/annals.debug.o: annals.cxx annals.h
	$(COMPILE) $@ annals.cxx

objects/time.o objects/time.debug.o: time.cxx time.h
	$(COMPILE) $@ time.cxx

objects/statemachine.o objects/statemachine.debug.o: statemachine.cxx statemachine.h
	$(COMPILE) $@ statemachine.cxx
