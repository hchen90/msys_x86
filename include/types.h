#ifndef _TYPES_H_
#define _TYPES_H_

#undef uint8_t
#define uint8_t unsigned char

#undef uint16_t
#define uint16_t unsigned short

#undef uint32_t
#define uint32_t unsigned int

#undef size_t
#define size_t unsigned int

#undef key_t
#define key_t int

#undef NULL
#define NULL 0

#undef M_PIC
#define M_PIC 0x20

#undef M_IMR
#define M_IMR 0x21

#undef S_PIC
#define S_PIC 0xa0

#undef S_IMR
#define S_IMR 0xa1

#undef P_TIMER
#define P_TIMER 0x40

#undef EOI
#define EOI 0x20

#undef R_OK
#define R_OK 0x01

#undef W_OK
#define W_OK 0x02

#undef X_OK
#define X_OK 0x04

#undef F_OK
#define F_OK R_OK | W_OK | X_OK

#undef uid_t
#define uid_t unsigned int

#undef gid_t
#define gid_t unsigned int

#undef mode_t
#define mode_t unsigned int

#undef pid_t
#define pid_t unsigned int

#undef REBOOT_HALT
#define REBOOT_HALT 0x01

#undef REBOOT_RESTART
#define REBOOT_RESTART 0x02

#undef off_t
#define off_t size_t

#undef SEEK_SET
#define SEEK_SET 0

#undef SEEK_CUR
#define SEEK_CUR 1

#undef SEEK_END
#define SEEK_END 2

#undef time_t
#define time_t unsigned int

#undef stdin
#define stdin 0

#undef stdout
#define stdout 1

#undef stderr
#define stderr 2

#endif
