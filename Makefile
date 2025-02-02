all:
	gcc -oneondash src/main.c -I. -lncurses
clean:
	rm *.exe* neondash
