



build: main.o
	gcc main.o -o autoprintdbg

main.o: main.c
	gcc -c main.c


clean: 
	rm main.o autoprintdbg