head_proc: head_proc.o son_proc
	gcc -Wall head_proc.o -o head_proc

head_proc.o: head_proc.c
	gcc -c head_proc.c 

son_proc: son_proc.o
	gcc -Wall son_proc.o -o son_proc

son_proc.o: son_proc.c
	gcc -c son_proc.c
clean:
	rm head_proc head_proc.o son_proc son_proc.o
