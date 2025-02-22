all: clean shell test_mylib

shell: shell.o str_lib.o job.o mylib.o
	gcc shell.o str_lib.o job.o mylib.o -o shell

shell.o: shell.c str_lib.h constants.h
	gcc -g -c shell.c

str_lib.o: str_lib.c str_lib.h constants.h mylib.h
	gcc -g -c str_lib.c

job.o: job.c job.h struct.h constants.h
	gcc -g -c job.c

mylib.o: mylib.c mylib.h constants.h
	gcc -g -c mylib.c

test_mylib: test_mylib.o mylib.o
	gcc test_mylib.o mylib.o -o test_mylib

test_mylib.o: test_mylib.c mylib.h constants.h
	gcc -g -c test_mylib.c

clean:
	-rm -f shell test_mylib *.o