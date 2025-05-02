# set this variable to the director in which you saved the common files
commondir = ../common/

all : shadowmap

shadowmap: shadowmap.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o shadowmap -I$(commondir) -I$(commondir)Linux -DGL_GLEXT_PROTOTYPES shadowmap.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lX11 -lGL -lm

clean :
	rm shadowmap

