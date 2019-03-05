CC = gcc
FILES = src/*.c
OUT_EXE = bin/spider-cat

build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES)

run:
	./$(OUT_EXE)

clean:
	rm -f *.o core

rebuild: clean build
