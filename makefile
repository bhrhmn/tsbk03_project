# Minimal modular Makefile
commondir = common/

all: scene

scene: Project/scene.cpp Project/camera.cpp Project/models.cpp Project/rendering.cpp Project/lighting.cpp Project/shadows.cpp Project/tree.cpp Project/rain.cpp Project/animation.cpp
	g++ -Wall -o scene -I$(commondir) -I$(commondir)Linux -DGL_GLEXT_PROTOTYPES \
	Project/scene.cpp Project/camera.cpp Project/models.cpp Project/rendering.cpp Project/lighting.cpp Project/shadows.cpp Project/tree.cpp Project/rain.cpp Project/animation.cpp \
	$(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(commondir)lodepng.cpp \
	-lXt -lX11 -lGL -lm -lstdc++ \
	-lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

fedora: Project/scene.cpp Project/camera.cpp Project/models.cpp Project/rendering.cpp Project/lighting.cpp Project/shadows.cpp Project/tree.cpp Project/rain.cpp Project/animation.cpp
	g++ -Wall -o scene -I$(commondir) -I$(commondir)Linux -DGL_GLEXT_PROTOTYPES \
	Project/scene.cpp Project/camera.cpp Project/models.cpp Project/rendering.cpp Project/lighting.cpp Project/shadows.cpp Project/tree.cpp Project/rain.cpp Project/animation.cpp \
	$(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(commondir)lodepng.cpp \
	-lX11 -lGL -lm -lstdc++ \

clean:
	rm -f scene

.PHONY: all clean