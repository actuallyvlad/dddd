SOURCES=$(wildcard src/*.cpp)
OBJS=$(SOURCES:.cpp=.o)

# compiler options : add debug information in debug mode
# optimize speed and size in release mode
ifneq (,$(findstring debug,$(MAKECMDGOALS)))
	CFLAGS=-g
else
	CFLAGS=-O2 -s
endif

# linker options : OS dependant
ifeq ($(shell sh -c 'uname -s'),Linux)
	LIBFLAGS=-L. -ltcod_debug -ltcodxx_debug -Wl,-rpath=.
else
	LIBFLAGS=-Llib -ltcod-mingw-debug -static-libgcc -static-libstdc++ -mwindows
endif

debug : dddd
release : dddd

dddd : $(OBJS)
	g++ $(OBJS) -o build/dddd -Wall $(LIBFLAGS) $(CFLAGS)

src/main.hpp.gch : src/*.hpp
	g++ src/main.hpp -Iinclude -Wall

src/%.o : src/%.cpp src/main.hpp.gch
	g++ $< -c -o $@ -Iinclude -Wall $(CFLAGS)

clean :
	rm -f src/main.hpp.gch $(OBJS)
