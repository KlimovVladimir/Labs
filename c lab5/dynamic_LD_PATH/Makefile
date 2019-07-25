lab5: libmath.so
	gcc lab5.c -L. -lmath -o lab5

libmath.so: math.o
	gcc -shared math.o -o libmath.so

math.o: math.c
	gcc -c -fPIC math.c

clean:
	rm math.o lab5 libmath.so
