
INCLUDES = \
    -I. \
    -I/usr/include/SDL


LIBS = \
	-lGL -lGLU -lSDL -lSDL_mixer -lSDL_image

GPPFLAGS = \
    $(INCLUDES) \


ICCFLAGS = \
    $(INCLUDES) 


CPP = g++ $(GPPFLAGS)
#CPP = g++ $(IPPFLAGS)


.cpp.o:
	$(CPP) -g -c -o $@ $<
.c.o:
	$(CC) -g -c -o $@ $<

OBJS = \
		glee/GLee.o \
		shader.o \
		a2.o \
		algebra.o \
		skydome.o \
		scene.o \
		primitive.o \
		material.o \
		light.o \
		world.o \
		particle.o \
    main.o \
    SDL_OGL.o \
		glm.o \
		SoundManager.o


compileobjs: $(OBJS)

HEADERS = \
		glee/GLee.h \
    SDL_OGL.h \
		SoundManager.h



lib_tutorial: $(HEADERS) $(OBJS) Makefile
	ar rvs libsm $(OBJS)
	mv libsm libsm.a

project: $(HEADERS) $(OBJS) Makefile
	$(CPP) -o surreal $(OBJS) $(LIBS)

clean:
	-rm *.o core
