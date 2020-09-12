CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -I./

BIN=modeling-transforms
SRCS=modeling-transforms.cpp gl_framework.cpp ../common/*.cpp ../common/imgui/*.cpp
INCLUDES=gl_framework.hpp ../common/*.hpp ../common/imgui/*.h

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN)
