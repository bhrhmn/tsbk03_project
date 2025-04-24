# set this variable to the director in which you saved the common files
commondir = ../common2/

all : shadowmap

shadowmap : shadowmap.c $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(commondir)VectorUtils3.c $(commondir)loadobj.c
	gcc -Wall -o shadowmap -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES shadowmap.c $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm -lstdc++ 
clean :
	rm shadowmap

