make:
	gcc filecrawl.c -lpthread -o filecrawl

install:
	cp filecrawl /usr/local/bin/filecrawl