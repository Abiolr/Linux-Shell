all: clean shell

shell: shell.o str_lib.o
	gcc shell.o str_lib.o -o shell

shell.o: shell.c constants.h str_lib.h
	gcc -g -c shell.c

str_lib.o: str_lib.c str_lib.h constants.h
	gcc -g -c str_lib.c

clean:
	-rm -f shell *.o