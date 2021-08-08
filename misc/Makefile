CC = cc
LD = ld
STRIP = strip
CFLAGS = -m32 -Werror -nostdinc -fno-stack-protector -fno-pic -fno-builtin -fno-strict-aliasing -fno-omit-frame-pointer -static -I../include
LDFLAGS = -T link.ld -m elf_i386
UTILS = cat ed init kill ls mkdir ps reboot rmdir sh wc
UTILSO = cat.o ed.o init.o ls.c mkdir.o kill.o ps.o reboot.o rmdir.o sh.o wc.o utils.o

all: $(UTILS)
	$(STRIP) $(UTILS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

cat: cat.o
	$(LD) $(LDFLAGS) -o $@ $<
ed: ed.o
	$(LD) $(LDFLAGS) -o $@ $<
init: init.o
	$(LD) $(LDFLAGS) -o $@ $<
kill: kill.o utils.o
	$(LD) $(LDFLAGS) -o $@ $< utils.o 
ls: ls.o
	$(LD) $(LDFLAGS) -o $@ $<
mkdir: mkdir.o
	$(LD) $(LDFLAGS) -o $@ $<
ps: ps.o
	$(LD) $(LDFLAGS) -o $@ $<
reboot: reboot.o
	$(LD) $(LDFLAGS) -o $@ $<
rmdir: rmdir.o
	$(LD) $(LDFLAGS) -o $@ $<
sh: sh.o
	$(LD) $(LDFLAGS) -o $@ $<
wc: wc.o
	$(LD) $(LDFLAGS) -o $@ $<

clean:
	rm -rvf $(UTILS) $(UTILSO)
