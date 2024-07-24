CC = gcc
CFLAGS = -g -Wall

udev_examples:
	$(CC) $(CFLAGS) -o udev_test main.c -ludev

default: udev_examples

all: default

clean:
	rm -f udev_test