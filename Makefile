all: listDests

listDests:	listDests.c myLibrary.c myLibrary.h
	gcc -w -o listDests listDests.c myLibrary.c -lcups 

clean:
	rm listDests	