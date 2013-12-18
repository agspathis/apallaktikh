CC=g++
CFLAGS= -lGL -lGLU -lglut

all: main visuals controls model texture
	$(CC) main.o visuals.o controls.o model.o texture.o -o main $(CFLAGS)

main: main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)

visuals: visuals.cpp
	$(CC) -c visuals.cpp -o visuals.o $(CFLAGS)

controls: controls.cpp
	$(CC) -c controls.cpp -o controls.o $(CFLAGS)

model: model.cpp
	$(CC) -c model.cpp -o model.o $(CFLAGS)

texture: texture.cpp
	$(CC) -c texture.cpp -o texture.o $(CFLAGS)

clean:
	rm -rf *o main
