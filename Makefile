CC := gcc
CFLAGS := -Os -oretest
LFLAGS := -lreadline -s

retest: retest.c
	$(CC) $(CFLAGS) retest.c $(LFLAGS)

clean:
	rm retest.c~ Makefile~ retest.o

touch:
	touch retest.c

