all: clean shell

shell: shell.o str_lib.o command.o job.o
	gcc shell.o str_lib.o command.o job.o -o shell

shell.o: shell.c str_lib.h constants.h
	gcc -g -c shell.c

str_lib.o: str_lib.c str_lib.h constants.h
	gcc -g -c str_lib.c

command.o: command.c command.h constants.h
	gcc -g -c command.c

job.o: job.c job.h constants.h
	gcc -g -c job.c

clean:
	-rm -f shell *.o