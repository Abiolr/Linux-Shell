default: shell

all: clean shell

shell: shell.c
	gcc shell.c -o shell -lrt

clean:
	rm -f shell *~
