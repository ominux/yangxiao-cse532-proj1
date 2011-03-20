all : main.exe

main.exe : main.o  
	gcc main.o -static -lpthread -o main.exe

main.o : main.c
	gcc -c main.c

clean : 
	rm -rf *.o *.exe
