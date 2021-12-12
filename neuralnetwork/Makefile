# Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O3
LDFLAGS= #-fsanitize=address #-Wl,-V
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

SRC = network.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all: network

network: layer.o neuron.o network.o save.o bmptoarray.o ../utils/image.o ../utils/pixel_operations.o

clean:
	${RM} network.[do] 
	${RM} network
	${RM} *.d
	${RM} *.o
# END
