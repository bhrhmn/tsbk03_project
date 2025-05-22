# Your existing Makefile with SFML audio added
commondir = ../common/

all: scene

scene: scene.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(commondir)lodepng.cpp
	g++ -Wall -o scene -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES \
	scene.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c \
	$(commondir)Linux/MicroGlut.c $(commondir)lodepng.cpp \
	-lXt -lX11 -lGL -lm -lstdc++ \
	-lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f scene