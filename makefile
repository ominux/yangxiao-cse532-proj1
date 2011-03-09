min.exe : main.o assoc_eng.o
	gcc main.o assoc_eng.o -pthread -o main.exe

main.o : main.c
	gcc -c main.c

assoc_eng.o : assoc_eng.c
	gcc -std=c99 -c assoc_eng.c

clean : 
	rm -rf *.o *.exe
