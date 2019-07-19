lab5: libmath.so
	gcc lab5.c -o lab5 -ldl

libmath.so: math.o
	gcc -shared math.o -o libmath.so

math.o: math.c
	gcc -c -fPIC math.c

clean:
	rm math.o lab5 libmath.so
