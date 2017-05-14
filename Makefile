all: listDests listOpt

listDests:	listDests.c myLibrary.c myLibrary.h
	gcc -w -o listDests listDests.c myLibrary.c -lcups 

listOpt:	listOpt.c myLibrary.c myLibrary.h
	gcc -w -o listOpt listOpt.c myLibrary.c -lcups

clean:
	rm listDests	
	rm listOpt