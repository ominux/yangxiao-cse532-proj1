main.exe : main.o data_read.o
	gcc main.o data_read.o -pthread -o main.exe

main.o : main.c
	gcc -c main.c

data_read.o : data_read.c
	gcc -c data_read.c

clean : 
	rm -rf *.o *.exe
