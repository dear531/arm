#ifndef __COMMON_H__
#define __COMMON_H__

#define NULL (void *)0x00
#define printf(fmt, ...) (((int(*)(const char *, ...))0xc3e0c434)(fmt, ##__VA_ARGS__))

#endif
