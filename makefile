all : main.exe data_gen.exe

data_gen.exe : data_gen.c
	gcc -o data_gen.exe data_gen.c  


main.exe : main.o find_large_item.o data_read.o merge_sort.o print.o
	gcc main.o find_large_item.o data_read.o merge_sort.o print.o -static -lpthread -o main.exe

main.o : main.c
	gcc -c main.c

data_read.o : data_read.c
	gcc -c data_read.c

find_large_item.o : find_large_item.c
	gcc -c find_large_item.c

merge_sort.o : merge_sort.c
	gcc -c merge_sort.c 

print.o : print.c
	gcc -c print.c
clean : 
	rm -rf *.o *.exe
