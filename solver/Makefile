# Makefile

CC=gcc
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O3
LDFLAGS= -fsanitize=address -Wl,-V
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

SRC = solver.c ../nn/nn.c ../utils/pixel_operations.c 
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)
OUTPUTFILE = solver

.PHONY: all clean clean-output

all:	$(OBJ)
	$(CC) $(OBJ) -o $(OUTPUTFILE)

clean-output:
	${RM} *.bmp
	${RM} *.csv
	${RM} ./**/*.bmp
	${RM} ./**/*.csv

clean:
	${RM} solver.[do]
	${RM} ./**/*.o
	${RM} ./**/*.d
	${RM} solver
	${RM} *.result
# END
