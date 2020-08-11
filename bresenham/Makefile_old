
GLLIBS = -lGL -lGLU -lglut

PROGRAMS = bresenham bresenham_all


.SUFFIXES: .c .cpp .h .hpp

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS)
	rm -f *.o
	rm -f *~

bresenham: bresenham.cpp
	g++ bresenham.cpp -o $@ $(GLLIBS)

bresenham_all: bresenham_all.cpp
	g++ bresenham_all.cpp -o $@ $(GLLIBS)


