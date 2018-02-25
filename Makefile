# Makefile for the rayTrace project

# OBJS_ALL = *.o
OBJS_ALL = main.o shape.o camera.o vectormath.o ray.o color.o image.o objParser.o

main: $(OBJS_ALL)
	g++ -o main $(OBJS_ALL)

main.o: image.o camera.o shape.o main.cpp rayTrace.h rayCast.h
	g++ -c main.cpp

image.o: color.o image.cpp
	g++ -c image.cpp

camera.o: vectormath.o ray.o camera.cpp
	g++ -c camera.cpp

color.o: color.cpp
	g++ -c color.cpp

vectormath.o: vectormath.cpp
	g++ -c vectormath.cpp

ray.o: vectormath.o color.o ray.cpp
	g++ -c ray.cpp

shape.o: vectormath.o color.o ray.o shape.cpp
	g++ -c shape.cpp

objParser.o: shape.o vectormath.o objParser.cpp
	g++ -c objParser.cpp

clean:
	del $(OBJS_ALL)