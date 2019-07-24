lab5: libmath.a
	gcc lab5.c -L. -lmath -o lab5

libmath.a: math.o
	ar rc libmath.a math.o
	ranlib libmath.a

math.o: math.c
	gcc -c math.c

clean:
	rm math.o lab5 libmath.a
