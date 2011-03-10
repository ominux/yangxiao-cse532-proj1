all : main.exe data_gen.exe

data_gen.exe : data_gen.c
	gcc -o data_gen.exe data_gen.c  


main.exe : main.o find_large_item.o data_read.o 
	gcc main.o find_large_item.o data_read.o -pthread -o main.exe

main.o : main.c
	gcc -c main.c

data_read.o : data_read.c
	gcc -c data_read.c

find_large_item.o : find_large_item.c
	gcc -c find_large_item.c

clean : 
	rm -rf *.o *.exe
