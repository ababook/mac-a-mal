/*
 * Copyright (c) fG!, 2011, 2012, 2013, 2014 - reverser@put.as - http://reverse.put.as
 * All rights reserved.
 * sysent.c
 *
 */

#ifndef onyx__my_data_definitions_h
#define onyx__my_data_definitions_h


#include <mach/mach_types.h>
#include <sys/types.h>
#include <stdint.h>


// sysent definitions
// found in xnu/bsd/sys/sysent.h
typedef int32_t	sy_call_t(struct proc *, void *, int *);
typedef void	sy_munge_t(const void *, void *);


struct kernel_info
{
    mach_vm_address_t running_text_addr; // the address of running __TEXT segment
    mach_vm_address_t disk_text_addr;    // the same address at /mach_kernel in filesystem
    mach_vm_address_t kaslr_slide;       // the kernel aslr slide, computed as the difference between above's addresses
    void *linkedit_buf;                  // pointer to __LINKEDIT buffer containing symbols to solve
    uint64_t linkedit_fileoff;           // __LINKEDIT file offset so we can read
    uint64_t linkedit_size;
    uint32_t symboltable_fileoff;        // file offset to symbol table - used to position inside the __LINKEDIT buffer
    uint32_t symboltable_nr_symbols;
    uint32_t stringtable_fileoff;        // file offset to string table
    uint32_t stringtable_size;
    // other info from the header we might need
    uint64_t text_size;                  // size of __text section to disassemble
    struct mach_header_64 *mh;           // ptr to mach-o header of running kernel
    uint32_t fat_offset;                 // the file offset inside the fat archive for the active arch
};
struct patch_location
{
    mach_vm_address_t address;
    int size;
    char orig_bytes[15];
    int jmp;                        // 0 = jz, 1 = jnz
    struct patch_location *next;
};

/* for all versions before Mavericks, found in bsd/sys/sysent.h */
struct sysent {		/* system call table */
	int16_t		sy_narg;        /* number of args */
	int8_t		sy_resv;        /* reserved  */
	int8_t		sy_flags;       /* flags */
	sy_call_t	*sy_call;       /* implementing function */
	sy_munge_t	*sy_arg_munge32; /* system call arguments munger for 32-bit process */
	sy_munge_t	*sy_arg_munge64; /* system call arguments munger for 64-bit process */
	int32_t		sy_return_type; /* system call return types */
	uint16_t	sy_arg_bytes;	/* Total size of arguments in bytes for
								 * 32-bit system calls
								 */
};

/* Sysent structure got modified in Mavericks */
struct sysent_mavericks {
    sy_call_t   *sy_call;
    sy_munge_t  *sy_arg_munge32;
    sy_munge_t  *sy_arg_munge64;
    int32_t     sy_return_type;
    int16_t     sy_narg;
    uint16_t    sy_arg_bytes;
};

/* And again in Yosemite */
struct sysent_yosemite {
    sy_call_t   *sy_call;
    sy_munge_t  *sy_arg_munge64;
    int32_t     sy_return_type;
    int16_t     sy_narg;
    uint16_t    sy_arg_bytes;
};


#define DISABLE 0
#define ENABLE 1

#define MAVERICKS   13
#define YOSEMITE    14
#define EL_CAPITAN  15
#define SIERRA      16

#if DEBUG
#define LOG_DEBUG(fmt, ...) printf("[PHAMOOK DEBUG] " fmt "\n", ## __VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...) do {} while (0)
#endif

#define LOG_MSG(...) printf(__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printf("[PHAMOOK ERROR] " fmt "\n", ## __VA_ARGS__)

#define LOG_INFO(fmt, ...) printf("[PHAMOOK INFO] " fmt "\n", ## __VA_ARGS__)

#endif
