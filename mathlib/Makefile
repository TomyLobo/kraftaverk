WARNINGS = -W -Wall -Wextra -Wno-unused-parameter -Wconversion -Wwrite-strings -Wcast-qual -pedantic
PROFILE = 
#-pg -DPROFILE
INCLUDEPATH = 
LIBPATH = 
LIBS = 
CFLAGS = -std=c++98 $(WARNINGS) -O3 $(PROFILE) $(INCLUDEPATH) -Wno-long-long -U__STRICT_ANSI__
LFLAGS = $(LIBPATH) $(LIBS) $(PROFILE)

OBJS = test.o

all: test
run: test
	./test
	
clean:
	-rm *.o test.exe

.PHONY: clean all run


test: $(OBJS)
	g++ -o $@ $^ $(LFLAGS)

test.o: test.cpp quaternion.h vector2.h vector3.h vector4.h matrix3.h matrix4.h mathlib.h angle.h
	g++ $(CFLAGS) -c -o $@ $<
