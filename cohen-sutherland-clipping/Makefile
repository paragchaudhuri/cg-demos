
GLLIBS = -lGL -lGLU -lglut

PROGRAMS=clipping

.SUFFIXES: .c .cpp

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS)
	rm -f *.o
	rm -f *~

clipping: clipping.cpp
	g++ -Wall -g clipping.cpp -o $@ $(GLLIBS)
