all : main.exe

main.exe : main.o find_large_item.o 
	gcc main.o find_large_item.o -static -lpthread -o main.exe

main.o : main.c
	gcc -c main.c

find_large_item.o : find_large_item.c
	gcc -c find_large_item.c

clean : 
	rm -rf *.o *.exe
