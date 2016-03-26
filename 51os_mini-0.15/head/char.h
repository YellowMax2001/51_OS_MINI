#ifndef __char_H__
#define __char_H__

#include <stc15f2k60s2.h>

typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef unsigned long uint32;
typedef signed char int8;
typedef signed int  int16;
typedef signed long int32;
/* 下面两个是为了兼容已经写过的代码 */
#define uint unsigned int	
#define uchar unsigned char	

extern void clear_char_str(unsigned char *str2clear, unsigned int len);
extern unsigned char os_strcmp(unsigned char *str1, unsigned char *str2);
extern unsigned char split_str(unsigned char *str2split, unsigned char split[][20]);

#endif