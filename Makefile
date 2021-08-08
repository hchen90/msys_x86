CFLAGS = -g -m32 -Werror -nostdinc -fno-stack-protector -fno-pic -fno-builtin -fno-strict-aliasing -fno-omit-frame-pointer -static -I./include

FSd   := fs
FSo   := fs/dir.o fs/file.o fs/inode.o fs/lba.o fs/map.o fs/namei.o fs/super.o

INITd := init
INITo := init/main.o

LIBd  := lib
LIBo  := lib/snprintf.o lib/list.o lib/utils.o

MMd   := mm
MMo   := mm/chunk.o mm/mem.o

KNRd  := kernel
KNRo  := kernel/irq.o kernel/isr.o kernel/proc.o kernel/sched.o kernel/shm.o kernel/sys.o kernel/syscall.o kernel/timer.o kernel/video.o kernel/x86.o

MISC  := misc

SUBS  := $(FSd) $(INITd) $(LIBd) $(MMd) $(KNRd)
OBJS  := $(FSo) $(INITo) $(LIBo) $(MMo) $(KNRo)

AS    = as
LD    = ld
ASFLAGS = -g --32
LDFLAGS = -m elf_i386 -T link.ld

.PHONY: all $(SUBS) $(MISC)

all: kernel.i386.elf $(MISC)

$(SUBS) $(MISC):
	make --directory=$@

kernel.i386.elf: setup32.o $(SUBS)
	$(LD) $(LDFLAGS) -o $@ setup32.o $(OBJS)

.S.o:
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	rm -vrf kernel.i386.elf *.o $(OBJS)
	make clean --directory=$(MISC)
