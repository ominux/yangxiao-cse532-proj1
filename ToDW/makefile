all : main.exe run.exe

run.exe : tosubmit.o  
	gcc tosubmit.o -static -lpthread -o run.exe

tosubmit.o : tosubmit.c
	gcc -c tosubmit.c

main.exe : main.o  
	gcc main.o -static -lpthread -o main.exe

main.o : main.c
	gcc -c main.c

clean : 
	rm -rf *.o *.exe
