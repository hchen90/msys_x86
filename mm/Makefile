CC = cc
CFLAGS = -g -m32 -Werror -nostdinc -fno-stack-protector -fno-pic -fno-builtin -fno-strict-aliasing -fno-omit-frame-pointer -static -I../include

all: chunk.o mem.o

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -vfr *.o
