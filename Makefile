CC=g++
CFLAGS= -lGL -lGLU -lglut

all: main.o visuals.o controls.o model.o\
	 vmodel.o texture.o mcubes.o angle.o\
     intersection.o
	$(CC) main.o visuals.o controls.o model.o\
		  vmodel.o texture.o mcubes.o angle.o\
          intersection.o\
		  -o main $(CFLAGS)

main.o: main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)

visuals.o: visuals.cpp vmodel.h
	$(CC) -c visuals.cpp -o visuals.o $(CFLAGS)

controls.o: controls.cpp
	$(CC) -c controls.cpp -o controls.o $(CFLAGS)

model.o: model.cpp
	$(CC) -c model.cpp -o model.o $(CFLAGS)

vmodel.o: vmodel.cpp
	$(CC) -c vmodel.cpp -o vmodel.o $(CFLAGS)

mcubes.o: mcubes.cpp
	$(CC) -c mcubes.cpp -o mcubes.o $(CFLAGS)

angle.o: angle.cpp model.h
	$(CC) -c angle.cpp -o angle.o $(CFLAGS)

texture.o: texture.cpp
	$(CC) -c texture.cpp -o texture.o $(CFLAGS)

intersection.o: intersection.cpp
	$(CC) -c intersection.cpp -o intersection.o $(CFLAGS)

clean:
	rm -rf *o main
