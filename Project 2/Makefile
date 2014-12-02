###########################################################
# Project 2 Makefile

CC = g++
CFLAGS = -Wall -ggdb
INCLUDE = -I/lusr/X11/include -I/lusr/include
LIBDIR = -L/lusr/X11/lib -L/lusr/lib
# Libraries that use native graphics hardware --
# appropriate for Linux machines in Taylor basement
LIBS = -lglut -lGLU -lGL -lpthread -lm

###########################################################
# Options if compiling on Mac
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CC = g++
CFLAGS = -Wall -g -D__MAC__
INCLUDE = 
LIBDIR = -L/lusr/X11/lib
LIBS = -framework OpenGL -framework GLUT
endif

plant: drawplant.cpp drawplant.h matrix.cpp plant.cpp readppm.cpp
	${CC} ${CFLAGS} $(INCLUDE) -o plant ${LIBDIR} drawplant.cpp plant.cpp readppm.cpp matrix.cpp  ${LIBS}

clean:
	rm -f plant *.o core
