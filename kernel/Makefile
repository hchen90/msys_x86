CC = cc
ASFLAGS = -g -m32
CFLAGS = -g -m32 -Werror -nostdinc -fno-stack-protector -fno-pic -fno-builtin -fno-strict-aliasing -fno-omit-frame-pointer -static -I../include

all: irq.o isr.o proc.o sched.o shm.o sys.o syscall.o timer.o video.o x86.o

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -vfr *.o
