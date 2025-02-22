all: clean shell

shell: shell.o str_lib.o job.o mylib.o signal_handle.o
	gcc shell.o str_lib.o job.o mylib.o signal_handle.o -o shell

shell.o: shell.c str_lib.h constants.h
	gcc -g -c shell.c

signal_handle.o: signal_handle.c signal_handle.h 
	gcc -g -c signal_handle.c

str_lib.o: str_lib.c str_lib.h constants.h mylib.h
	gcc -g -c str_lib.c

job.o: job.c job.h struct.h constants.h
	gcc -g -c job.c

mylib.o: mylib.c mylib.h constants.h
	gcc -g -c mylib.c

clean:
	-rm -f shell *.o
