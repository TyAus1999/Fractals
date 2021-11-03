gcc -c asm.S -O3 -o asm.o
gcc -c Fractal.c -O3 -o c.o -lm -mavx2 -lglut -lGL
gcc -g asm.o c.o -O3 -o out -lGL -lm -mavx2 -lglut -lpthread
rm asm.o
rm c.o
