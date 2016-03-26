/*
 * 51_MINI_OS file system head
 */

#ifndef __fs_H__
#define __fs_H__

#include <stc15f2k60s2.h>
#include "eeprom.h"
#include "uart.h"
#include "char.h"
#include "core.h"
#include "common.h"

#ifdef FS_SELECT

#define FS_BEGIN 0x5000	//文件系统开始地址
#define FS_END   0xEFFF	//文件系统结束地址
#define FS_PAGE  512	//文件系统最小块的大小

#define NORMAL_DIR_TYPE  0x3A	//标准文件夹：可读可写可删除，文件夹，已被占用
#define NORMAL_FILE_TYPE 0x38	//标准文本文件：可读可写可删除，文件，已被占用 
#define ROOT_DIR         0x32	//根目录，可读可写不可删除，文件夹，已被占用

#define FS_NEED_KEEP    92	//在扇区擦除之后需要回复这几个数据
#define FS_TOTAL_SIZE   92	//结构体的总的长度是92字节
#define FS_FLAG_SIZE	2	//fs结构体里面flag的长度是两字节
#define FS_NAME_SIZE	20	//fs结构体里面name的长度是20个字节
#define FS_SIZE_SIZE	2	//fs结构体里面size的长度是两字节
#define FS_PREV_SIZE	2	//fs结构体里面prev的长度是两字节
#define FS_NEXT_SIZE	2	//fs结构体里面next的长度是两字节
#define FS_NODE_SIZE	2*32	//fs结构体里面node的长度是32*2字节

/* 文件系统的类型，这里只是一个简单的文件类型，是一个双向链表的结构
 * 一个内存块的大小为128byte
 * 文件系统从0x5000开始到0xEFFF结束，一共是40K大小
 **************************重要***********************************
 *此结构体在使用IAP构建的时候要遵循int的低字节在前高字节在后的约定
 *****************************************************************
 */
struct fs_type							//111:可读可写删除				  // 01:文件夹
{			//0 0 0 0 0 0 0 0 0 0       0 0 0                          00(00:文件)          0(1:未被占用)
	unsigned int flag;		//        此文件权限，                 文件的type，         此块是否被占用,要两字节
	unsigned char name[20];	//文件名长度为20
	unsigned int size;		//文件大小
	/* 链表双向结构 */
	struct fs_type *prev;
	struct fs_type *next;
	unsigned int inode[32];	//inode的个数为32，也就是一个文件夹下面最多有32个文件
	//一个文件的大小最多有32*128字节
};
/* 上述的文件结构体是一个示范，实际操作的时候是使用IAP直接写入 */

/* 该值存储指针地址，该指针指向当前文件夹 */
extern xdata unsigned int fs_struct_ptr;

/* 该值存储指针地址，该指针指向第一个空的内存块，加快系统内存分配过程 */
extern xdata unsigned int first_empty_page;

extern void fs_format(void);
extern void fs_write(unsigned char *name);
extern void fs_read(unsigned char *name);
extern unsigned int alloc(unsigned int len, unsigned int *pos);
extern unsigned char create_dir_file(unsigned char *name, unsigned int addr, unsigned int prev_addr, unsigned char flag);
extern void list_file(unsigned int cur_f_ptr, unsigned char l_flag);
extern unsigned char change_dir(unsigned char *name);
extern unsigned char delete_file(unsigned char *name);

#endif

#endif