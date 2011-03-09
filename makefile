main : main.o assoc_eng.o
	gcc main.o assoc_eng.o -DLINUX -DPOSIX -pthread -o main

main.o : main.c
	gcc -c main.c

assoc_eng.o : assoc_eng.c
	gcc -std=c99 -c assoc_eng.c

