#
#	Makefile for OpenGL example from Chapter 1 in Red Book
#

CC = g++ 
CFLAGS = -D_DEBUG
CPPFLAGS = -std=c++0x -g

.cpp.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@  $<

#LIBS = -framework OpenGL -framework GLUT -lglew 

LIBS = -lGL -lGLU -lglut -lm -ljpeg -Wall -lGLEW

SRCS = viewer.cpp LoadShaders.cpp
OBJS = viewer.o LoadShaders.o 

example1: $(OBJS) LoadShaders.h
	g++ -g -o viewer $(OBJS) $(LIBS)

clean:
	rm -f viewer *.o
