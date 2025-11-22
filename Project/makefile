# Minimal modular Makefile
commondir = ../common/

all: scene

scene: scene.cpp camera.cpp models.cpp rendering.cpp lighting.cpp shadows.cpp rain.cpp
	g++ -Wall -o scene -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES \
	scene.cpp camera.cpp models.cpp rendering.cpp lighting.cpp shadows.cpp tree.cpp rain.cpp \
	$(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(commondir)lodepng.cpp \
	-lXt -lX11 -lGL -lm -lstdc++ \
	-lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

fedora: scene.cpp camera.cpp models.cpp rendering.cpp lighting.cpp shadows.cpp rain.cpp
	g++ -Wall -o scene -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES \
	scene.cpp camera.cpp models.cpp rendering.cpp lighting.cpp shadows.cpp tree.cpp rain.cpp \
	$(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(commondir)lodepng.cpp \
	-lX11 -lGL -lm -lstdc++

clean:
	rm -f scene

.PHONY: all clean fedora