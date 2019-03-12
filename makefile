CC = gcc
CFLAGS = -g -Wall
SOURCES = src/main.c src/server.c src/client.c
HEADERS = src/*.h
OBJECTS = $(SOURCES:.c=.o)
TARGET = bin/spider-cat

default: build

build: $(TARGET)
	@echo $(TARGET) compiled

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) *~ $(TARGET)

rebuild: clean build

runserver:
	$(TARGET)

runclient:
	$(TARGET) username
