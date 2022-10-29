#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#ifdef GNU_LINUX
#include<error.h>
#include<errno.h>
#include<x86_64-linux-gnu/sys/cdefs.h>
#define likely(arg) __glibc_likely(arg)
#define unlikely(arg) __glibc_unlikely(arg)
#else
#define likely(arg) __builtin_expect((arg),1)
#define unlikely(arg) __builtin_expect((arg),0)
#endif

#define uch unsigned char
#define ui unsigned int
#define us unsigned short

#define setbit(n) \
{   \
    checklist[n/8] |= (1 << (n % 8)); \
}

#define isset(n) \
{   \
    checklist[n/8] & ( 1 << (n % 8)); \
}
