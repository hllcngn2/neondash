all:
	gcc -oneondash \
		src/main.c \
			-I. \
				-lncurses
