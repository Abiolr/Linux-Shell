all: clean shell

shell: shell.o str_lib.o command.o job.o mylib.o
	gcc shell.o str_lib.o command.o job.o mylib.o -o shell

shell.o: shell.c str_lib.h constants.h
	gcc -g -c shell.c

str_lib.o: str_lib.c str_lib.h constants.h mylib.h
	gcc -g -c str_lib.c

command.o: command.c command.h constants.h mylib.h
	gcc -g -c command.c

job.o: job.c job.h constants.h
	gcc -g -c job.c

mylib.o: mylib.c mylib.h constants.h
	gcc -g -c mylib.c

clean:
	-rm -f shell *.o
