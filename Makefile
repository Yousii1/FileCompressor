CC=gcc
CFLAGS=-c -Wall
SOURCE=proyecto5.c
OBJ=$(SOURCE:.c=.o)
EXE=proyecto5

all: $(SOURCE) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ -lm 

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@ -lm

clean:
	rm -rf $(OBJ) $(EXE)
