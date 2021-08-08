  .text
  .align 4
  .long  0x1badb002  /* magic */
  .long  0x00        /* flags */
  .long  - (0x1badb002 + 0x00) /* checksum */

  .global start
  .extern kmain
start:
  /* addr: 0x10000c */
  cli
  lgdt gdt48
  lidt idt48
  
  inb $0x92, %al /*A20*/
  orb $0x2, %al
  outb %al, $0x92

  movl %cr0, %eax /*PE*/
  orl $1, %eax
  movl %eax, %cr0

  ljmp $(8 * 1), $flush
  nop
  nop

flush: /*protect mode*/
  movl $(8 * 2), %eax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss
  mov $0x20000, %esp
mainloop:
  call kmain
  jmp mainloop

  .data
  .equ NULLSEG, 0x0000000000000000
  .equ CODESEG, 0x00df98000000ffff /*kernel code*/
  .equ DATASEG, 0x00df92000000ffff /*kernel data*/
gdt:
  .quad NULLSEG /* 0  (0x00) */
  .quad CODESEG /* 1  (0x08) */
  .quad DATASEG /* 2  (0x10) */
  .quad NULLSEG /* 3  (0x18) */
  .quad NULLSEG /* 4  (0x20) */
  .quad NULLSEG /* 5  (0x28) */
  .quad NULLSEG /* 6  (0x30) */
  .quad NULLSEG /* 7  (0x38) */
  .quad NULLSEG /* 8  (0x40) */
  .quad NULLSEG /* 9  (0x48) */
  .quad NULLSEG /* 10 (0x50) */
  .quad NULLSEG /* 11 (0x58) */
gdt48:
  .word 8 * 3 - 1
  .long gdt
  .word 0
idt:
  .quad NULLSEG /* 0  */
  .quad NULLSEG /* 1  */
  .quad NULLSEG /* 2  */
  .quad NULLSEG /* 3  */
  .quad NULLSEG /* 4  */
  .quad NULLSEG /* 5  */
  .quad NULLSEG /* 6  */
  .quad NULLSEG /* 7  */
  .quad NULLSEG /* 8  */
  .quad NULLSEG /* 9  */
  .quad NULLSEG /* 10 */
  .quad NULLSEG /* 11 */
  .quad NULLSEG /* 12 */
  .quad NULLSEG /* 13 */
  .quad NULLSEG /* 14 */
  .quad NULLSEG /* 15 */
  .quad NULLSEG /* 16 */
  .quad NULLSEG /* 17 */
  .quad NULLSEG /* 18 */
  .quad NULLSEG /* 19 */
  .quad NULLSEG /* 20 */
  .quad NULLSEG /* 21 */
  .quad NULLSEG /* 22 */
  .quad NULLSEG /* 23 */
  .quad NULLSEG /* 24 */
  .quad NULLSEG /* 25 */
  .quad NULLSEG /* 26 */
  .quad NULLSEG /* 27 */
  .quad NULLSEG /* 28 */
  .quad NULLSEG /* 29 */
  .quad NULLSEG /* 30 */
  .quad NULLSEG /* 31 */
  .quad NULLSEG /* 32 */
  .quad NULLSEG /* 33 */
  .quad NULLSEG /* 34 */
  .quad NULLSEG /* 35 */
  .quad NULLSEG /* 36 */
  .quad NULLSEG /* 37 */
  .quad NULLSEG /* 38 */
  .quad NULLSEG /* 39 */
  .quad NULLSEG /* 40 */
  .quad NULLSEG /* 41 */
  .quad NULLSEG /* 42 */
  .quad NULLSEG /* 43 */
  .quad NULLSEG /* 44 */
  .quad NULLSEG /* 45 */
  .quad NULLSEG /* 46 */
  .quad NULLSEG /* 47 */
idt48:
  .word 8 * 48 - 1
  .long idt
  .word 0
/* end */
