# Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl gtk+-3.0` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O3 -g
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lSDL_gfx -lm

SRC = main-gui.c $(wildcard image-processing/*.c) $(wildcard utils/*.c) $(wildcard imagesplitting/*.c) $(wildcard solver/*.c) $(wildcard nn/*.c) 
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all: main

main: $(OBJ)
	${CC} ${CPPFLAGS} ${CFLAGS} ${LDFLAGS} ${LDLIBS} $^ -o $@

clean-output:
	${RM} ./**/*.bmp
	${RM} ./**/split/*.png
	${RM} output/final/grid_00
	${RM} output/final/grid_00.result

clean:
	${RM} main.[do]
	${RM} ./**/*.o
	${RM} ./**/*.d
	${RM} main
	${RM} *.d
	${RM} *.o
	${RM} ./*.d
	${RM} ./*.o
# END
