all:
	mkdir -p bin
	gcc -o bin/bouncing_balls src/main.c -lGL -lGLU -lglut 
