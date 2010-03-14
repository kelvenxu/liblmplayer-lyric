all:
	gcc -o main `pkg-config --cflags --libs gtk+-2.0` main.c
