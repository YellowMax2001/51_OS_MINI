#include "fs.h"

#ifdef FS_SELECT

/* 该值存储指针地址，该指针指向当前文件夹 */
xdata unsigned int fs_struct_ptr;

/* 该值存储指针地址，该指针指向第一个空的内存块，加快系统内存分配过程 */
xdata unsigned int first_empty_page;

/* 擦除的时候用来保存临时的变量，长度为92字节 */
xdata unsigned char erase_return_buff[FS_NEED_KEEP];

/* 往文本文件里面写入数据的缓冲区,长度为1K字节 */
xdata unsigned char write_buff[1024];

/* 
 * 在指定的内存块查找相应名字的node
 * 例：find_file(0x5000, "dir1", 0); //在0x5000内存块处查找名字为"dir1"的文件
 * 若找到则返回所在node的地址，若没找到则返回0
 *     find_file(0x5000, "dir1", 1); //在0x5000内存块处查找名字为"dir1"的文件
 * 若找到则返回文件的地址，若没找到则返回0
 */
unsigned int find_file(unsigned int ptr, unsigned char *name, unsigned char flag)
{
	xdata unsigned int i,count = 0;
	xdata unsigned int next_ptr_mod = 0;
	xdata unsigned char name_mod[20];
	
	/* 跳过所有不需要查找的 */
	ptr = ptr + FS_FLAG_SIZE + FS_NAME_SIZE + FS_SIZE_SIZE + FS_PREV_SIZE + FS_NEXT_SIZE;
ENTER_CRITICAL;	//需要长时间执行的程序要先进入临界段	
	/* 查找非空的node节点 */
	for(i = 0; i < FS_NODE_SIZE; i += 2)
	{
		if(iap_read(ptr + i) == 0xFF)
		{
//			return 0;
//			printf("0xFF\r\n");
		}
		else
		{
			next_ptr_mod = iap_read(ptr + i + 1);	//读高字节放入ptr_mod
			next_ptr_mod = next_ptr_mod << 8;					//左移8位，放入ptr_mod高字节
			next_ptr_mod |= iap_read(ptr + i);		//读低字节放入ptr_mod低8位
			
			next_ptr_mod = next_ptr_mod + FS_FLAG_SIZE;		//跳过文件类型的读取，后续会添加			

			while(name_mod[count] = iap_read(next_ptr_mod + count))	//字串非空就一直读取名字存放到数组缓冲区中
			{
				count ++;
			}
			name_mod[count] = '\0';	//字符串封存
			
			if(!os_strcmp("*", name))	/* 要找到所有的字串 */
			{
				if(flag)
				{
					EXIT_CRITICAL;
					return (next_ptr_mod - FS_FLAG_SIZE);	//只要找到第一个不是空的node就返回
				}
				else
				{
					EXIT_CRITICAL;
					return i;	//只要找到第一个不是空的node就返回
				}
				
			}
			if(!os_strcmp(name_mod, name))	 /* 是否找到相应的字串 */
			{
//				printf("find\r\n");
				if(flag)
				{
					EXIT_CRITICAL;
					return (next_ptr_mod - FS_FLAG_SIZE);
				}
				else
				{
					EXIT_CRITICAL;
					return i;	//返回node的地址
				}
			}					

			count = 0;	
			next_ptr_mod = 0;	//清零，等待下一次查找
		}
	}

	EXIT_CRITICAL;
	return 0;	//未找到
}

/* 
 * 空间分配，返回所分配的空间地址
 * 例：alloc(128 * 3, pos); 分配一个128*3字节大小的空间并且返回空间所在的位置到pos中
 * 该函数如果返回的是0则表示空间不足，分配失败
 */
unsigned int alloc(unsigned int len, unsigned int *pos)
{
	xdata unsigned int i;
	xdata unsigned int temp;	
	xdata unsigned int find;

	/* 先确定要分配的块的大小，如果块非512字节对齐，则需要补齐多出来的部分到512字节 */
	if(i = len % FS_PAGE)
	{
		i = (len + FS_PAGE) / FS_PAGE;
	}
	else
	{
		i = len / FS_PAGE;
	}

	temp = i;

	/* 查找空间中的空块 */
	for(find = FS_BEGIN; (find < FS_END) && (i > 0); find += FS_PAGE)
	{
		if(iap_read(find) & 0x01) 	//为1表示此块是空闲的，未使用的
		{
			pos[temp-i] = find;	//将找到的空闲的块放入pos缓存中
			i --;	//找到一个空的块，i的值减一
		}
	}

	if(i > 0)
	{
		return 0;	//表示空间不足
	}
	pos[temp] = '\0';	//将字符串封存

	return temp;	//返回分配的空间块数量
}

/* 
 * 文件系统格式化
 * 例：fs_format(); 格式化从0x5000到0xEFFF这40K的地址空间
 */
void fs_format(void)
{
	xdata unsigned int addr;	

	/* 清空文件系统区 */
	for(addr = FS_BEGIN; addr < FS_END; addr += 512)
	{
		iap_erase(addr);
	}

	/* 创建根目录 */
	create_dir_file("/", FS_BEGIN, FS_BEGIN, ROOT_DIR);
	fs_struct_ptr = FS_BEGIN;
	first_empty_page = FS_BEGIN + FS_PAGE;	//实际没使用
}

/* 
 * 文件写函数，包括要写的地址以及要写的长度
 * 例：fs_write(0x5000, 20, "1234567"); 向0x5000地址写入20字节的数据，内容为"1234567"
 */
void fs_write(unsigned char *name)
{
	xdata unsigned int reve_buf_len = 0;
	xdata unsigned char select_cmd = 0;
	xdata unsigned int pos_mod[2] = 0;
	xdata unsigned char pos_number = 0, i = 0;
	xdata unsigned int file_ptr = 0;
	xdata unsigned int file_number = 0;
	
	/* 获得要删除文件的地址 */
	if(file_ptr = find_file(fs_struct_ptr, name, 1))
	{
		/* 如果是个文件夹，退出 */
		if(iap_read(file_ptr) & 0x02)
		{
			printf("It is a floder\r\n");
			return;
		}
	}
	else
	{
		printf("No such file\r\n");
		return;
	}
	
	/* 如果文件里面已经有了内容，那么就不编辑 */
	if(find_file(file_ptr, "*", 1))
	{
		printf("There are texts in this file, please delete this file and create a new\r\n");
		return;
	}	
	
	while(1)
	{
		while(1)
		{				
			write_buff[reve_buf_len] = uart_reve_byte();	//接收到一个字符并存取到接收缓冲区里面
			if(write_buff[reve_buf_len] == 0) 			//如果没有接收到字串，继续查询
			{
				task_delay(2);							//延时两个任务周期，以便其他任务继续执行
				TF1 = 1;	//立马产生一个中断
			}
			else
				break;
		}

		if(reve_buf_len >= 500)	//由于每一个块要进行标记，所以要少2个字节，又要避免字符判断超限，干脆再减少2个字节
		{			
			printf("\r\nToo much words\r\n");		//用户输入的字符数量过多
			printf("Please save it now\r\n");
			goto wait_write_quit;		
		}	
		else
		{
			if(write_buff[reve_buf_len] == 0x1B)		//接收到ESC按键
			{
				write_buff[reve_buf_len] = '\0';
wait_write_quit:
				write_buff[reve_buf_len + 1] = '\0';
				printf("\r\n");
				while(1)
				{
					select_cmd = uart_wait_byte();
					putchar(select_cmd);
					if(select_cmd == 'w')
					{
//						printf(write_buff);
						goto write_buff2eeprom;	//如果是写入的话就跳到写入的程序入口
					}
					else if(select_cmd == 'q')
					{
						printf("you quit\r\n");
						return;
					}
					else
					{
						printf("\b \b");	//一直发送退格写空再退格，形成符合我们认知的退格
					}
				}
			}
			else if(write_buff[reve_buf_len] == '\b')	  	//接收到退格符
			{
				if(reve_buf_len > 0)
				{
					reve_buf_len --;
					printf("\b \b");
				}
			}
			else if(write_buff[reve_buf_len] == '\r')	  	//接收到换行符
			{
				printf("\r\n");
				reve_buf_len ++;
				write_buff[reve_buf_len] = '\n';
				reve_buf_len ++;
			}
			else
			{
				uart_send_byte(write_buff[reve_buf_len]);	//将刚刚接收到的一个字节并且发送给串口软件显示
				reve_buf_len ++;							//字串索引值加1
			}			
		}
	}

write_buff2eeprom:
ENTER_CRITICAL;	//需要长时间执行的程序要先进入临界段
	if(pos_number = alloc(reve_buf_len, pos_mod))
	{
		/* 跳过所有不需要设置的 */
		file_ptr = file_ptr + FS_FLAG_SIZE + FS_NAME_SIZE + FS_SIZE_SIZE + FS_PREV_SIZE + FS_NEXT_SIZE;
		
		/* 查找空的node节点 */
		for(i = 0; i < FS_NODE_SIZE; i += 2)
		{
			if(iap_read(file_ptr + i) == 0xFF)
			{
				for(file_number = 0; file_number < pos_number; file_number ++)
				{
					iap_write(file_ptr + i + file_number * 2, pos_mod[file_number] % 256);		//先写低地址
					iap_write(file_ptr + i + 1 + file_number * 2, pos_mod[file_number] / 256);	//再写高地址
					/* 打印文件在内存当中的位置 */
					putchar(((pos_mod[file_number]) >> 12) + 48);
					putchar((((pos_mod[file_number]) >> 8) & 0x0F) + 48);
					putchar((((pos_mod[file_number]) >> 4) & 0x0F) + 48);
					putchar((((pos_mod[file_number]) >> 0) & 0x0F) + 48);
				}
				break;	//写完退出
			}
		}

		i = pos_number; file_number = 0;
		while(i)
		{
			iap_write((pos_mod[pos_number-i])++, 0x00);
			iap_write((pos_mod[pos_number-i])++, 0x00);
			while(write_buff[file_number])
			{
				if(file_number > 508) 	//事实上不可能大于508
				{
					break;
				}
				iap_write((pos_mod[pos_number-i])++, write_buff[file_number]);
				file_number ++;
			}
			iap_write((pos_mod[pos_number-i])++, 0x00);	//文件的结尾写入空字符，代表文件的结束
			i --; 	//写入块数量减一
		}
		printf("\r\n");
	}
	else
	{
		printf("Alloc space failed\r\n");
	}
EXIT_CRITICAL;	//写完退出临界段
}

/* 
 * 文件读函数，包括要读的地址以及要读的长度
 * 例：fs_read(0x5000, 20, str); 从0x5000地址开始读20字节长度数据放在str里面
 */
void fs_read(unsigned char *name)
{
	xdata unsigned int ptr_mod;
	xdata unsigned int i = 0;
	xdata unsigned char str_mod = 0;
	xdata unsigned int next_ptr_mod = 0;

	ptr_mod = fs_struct_ptr; 
	while(1)
	{
		/* 查找并返回文本文件的地址 */
		if(ptr_mod = find_file(ptr_mod, name, 1))
		{
			/* 判断是否是文本文件 */
			if(iap_read(ptr_mod) & 0x02)
			{
				printf("It not a text file\r\n");
				return;
			}
			else
			{	
				ptr_mod = ptr_mod + FS_FLAG_SIZE + FS_NAME_SIZE + FS_SIZE_SIZE + FS_PREV_SIZE + FS_NEXT_SIZE;
				/* 查找并且打印文本 */
				for(i = 0; i < FS_NODE_SIZE; i += 2)
				{
					if(iap_read(ptr_mod + i) == 0xFF)
					{
						return;	//遇到FF说明文本已经查找完毕
					}
					else
					{
						next_ptr_mod = iap_read(ptr_mod + i + 1);	//读高字节放入ptr_mod
						next_ptr_mod = next_ptr_mod << 8;					//左移8位，放入ptr_mod高字节
						next_ptr_mod |= iap_read(ptr_mod + i);		//读低字节放入ptr_mod低8位
						
						ENTER_CRITICAL;	//需要长时间执行的程序要先进入临界段
						ptr_mod = ptr_mod + FS_FLAG_SIZE; //跳过文件类型的读取，文本文件里面存储的一定是文本
						while(str_mod = iap_read(ptr_mod))
						{
							putchar(str_mod);	//打印出来其中的文本文件，直到遇到文本结束符为止
							ptr_mod ++;
						}

						EXIT_CRITICAL;	//写完退出临界段			
					}
				}//End for
			}
		}
		else
		{
			printf("No such file\r\n");
			return;
		}
	}
}

/* 
 * 新建文件夹或者文件
 * 例：create_dir("/", 0x5000, 0x5000, NORMAL_DIR_TYPE);	
 * 在0x5000处建立一个新的文件夹，名字为"/"，上一个结构体地址为0x5000，也就是自己,此文件夹是整个根文件系统的根目录
 * 
 * 
 */
unsigned char create_dir_file(unsigned char *name, unsigned int addr, unsigned int prev_addr, unsigned char flag)
{
	xdata unsigned int i = 0;	
	
	/* 说明已经有了一个同名文件 */
	if(find_file(prev_addr, name, 1))
	{
		return 1;
	}

	/* 如果不等，就表明不是根文件 */
	if(addr != prev_addr)
	{
		/* 跳过所有不需要设置的 */
		prev_addr = prev_addr + FS_FLAG_SIZE + FS_NAME_SIZE + FS_SIZE_SIZE + FS_PREV_SIZE + FS_NEXT_SIZE;
		
		/* 查找空的node节点 */
		for(i = 0; i < FS_NODE_SIZE; i += 2)
		{
			if(iap_read(prev_addr + i) == 0xFF)
			{
				iap_write(prev_addr + i, addr % 256);		//先写低地址
				iap_write(prev_addr + i + 1, addr / 256);	//再写高地址
//				fs_struct_ptr = addr;
				i = 0;
				prev_addr = prev_addr - FS_FLAG_SIZE - FS_NAME_SIZE - FS_SIZE_SIZE - FS_PREV_SIZE - FS_NEXT_SIZE;
				goto create_new;
			}
		}

		return 1; 	//返回1表示创建失败，没有足够的空间了
	}

create_new:	
	iap_write(addr, flag);
	iap_write(addr + 1, 0x00);
	addr += FS_FLAG_SIZE;	//跳过flag
	
	while(*name)
	{
		iap_write(addr+i, *name++);
		i ++;
	}
	iap_write(addr+i, '\0');	//在结尾添加字串结束符
	i = 0;

	addr += FS_NAME_SIZE;		//写完文件夹的名字
	addr += FS_SIZE_SIZE;		//跳过文件大小选项

	/* 写上一个结构体地址，先写低地址后写高地址，用来返回上一层文件夹 */
	iap_write(addr, prev_addr % 256);
	iap_write(addr + 1, prev_addr / 256);
	addr += FS_PREV_SIZE;		//写完此结构体的上一个结构体地址

/*	设计最终不需要构建成双向链表的形式，因此下面的都被注释掉了，一个新的文件是没有子node节点的	
//	 由于新建的结构体始终不会立刻就有下一个结构体 
	iap_write(addr, 0x00);
	iap_write(addr + 1, 0x00);
	addr += FS_NEXT_SIZE;		//写完此结构体的下一个结构体地址
*/

	return 0;
}

/* 
 * 列出所选中的文件夹下面的所有文件
 * 例：list_dir(0x5000); 列出0x5000这个块下面所有的文件
 * 
 */
void list_file(unsigned int cur_f_ptr, unsigned char l_flag)
{
	xdata unsigned int i = 0;
	xdata unsigned int ptr_mod = 0;
	xdata unsigned char name_mod = 0;	
	
	/* 跳过所有不需要查找的 */
	cur_f_ptr = cur_f_ptr + FS_FLAG_SIZE + FS_NAME_SIZE + FS_SIZE_SIZE + FS_PREV_SIZE + FS_NEXT_SIZE;
	
	/* 查找非空的node节点 */
	for(i = 0; i < FS_NODE_SIZE; i += 2)
	{
		if(iap_read(cur_f_ptr + i) == 0xFF)
		{
//			return;
		}
		else
		{
			ptr_mod = iap_read(cur_f_ptr + i + 1);	//读高字节放入ptr_mod
			ptr_mod = ptr_mod << 8;					//左移8位，放入ptr_mod高字节
			ptr_mod |= iap_read(cur_f_ptr + i);		//读低字节放入ptr_mod低8位

			if(l_flag)
			{
				if(iap_read(ptr_mod) & 0x02)
				{
					printf("文件夹\t");	//代表是文件夹
				}
				else
				{
					printf("文本\t");	//代表是文本文件
				}
				if(iap_read(ptr_mod) & 0x20)
				{
					printf("可读\t");	//代表是可读的
				}
				else
				{
					printf("不可读\t");	//代表是不可读的
				}
				if(iap_read(ptr_mod) & 0x10)
				{
					printf("可写\t");	//代表是可写的
				}
				else
				{
					printf("不可写\t");	//代表是不可写的
				}
				if(iap_read(ptr_mod) & 0x08)
				{
					printf("可删\t");	//代表是可删除的
				}
				else
				{
					printf("不可删\t");	//代表是不可删除
				}
				putchar((ptr_mod >> 12) + 48);
				putchar(((ptr_mod >> 8) & 0x0F) + 48);
				putchar(((ptr_mod >> 4) & 0x0F) + 48);
				putchar(((ptr_mod >> 0) & 0x0F) + 48);

				printf("\t\t");
			}

			ptr_mod = ptr_mod + FS_FLAG_SIZE;		//跳过文件类型的读取			
			while(name_mod = iap_read(ptr_mod++))	//字串非空就一直读取名字存放到数组缓冲区中
			{
				putchar(name_mod);			//打印出名字字符串
			}					

			name_mod = 0;	//清零，等待下一次查找
			if(l_flag)
			{
				printf("\r\n");	//直接换行
			}
			else
			{
				printf("\t\t");	//换到下一个对齐处
			}
		}
	}
}

/* 
 * 切换文件夹的目录
 * 例：change_dir("test"); 切换到当前目录下的test目录中去
 * 
 */
unsigned char change_dir(unsigned char *name)
{
	xdata unsigned int ptr_mod = 0;
	xdata unsigned int prev_ptr_mod = 0;
	
	ptr_mod = fs_struct_ptr;
	
	if(!os_strcmp("..", name))	//返回上一级文件夹
	{
		goto exit2last_dir;
	}
	else if(!os_strcmp("../", name))  //返回上一级文件夹
	{
		goto exit2last_dir;
	}
	else if(!os_strcmp("/", name))	 //返回根目录
	{
		fs_struct_ptr = FS_BEGIN;
		return 0;
	}
	else
	{
		/* 如果找到的话就进行文件夹的切换 */
		if(ptr_mod = find_file(ptr_mod, name, 1))
		{
			if(iap_read(ptr_mod) & 0x02)
			{
				fs_struct_ptr = ptr_mod;
				return 0;
			}
			else
			{
				printf("Not a dir, but a text file\r\n");
				return 1;
			}
			
		}		
		printf("No such dir\r\n");
		return 1;
	}

exit2last_dir:
	ptr_mod = ptr_mod + FS_FLAG_SIZE + FS_NAME_SIZE + FS_SIZE_SIZE;
	prev_ptr_mod = iap_read(ptr_mod + 1);
	prev_ptr_mod = prev_ptr_mod << 8;
	prev_ptr_mod |= iap_read(ptr_mod);
	fs_struct_ptr = prev_ptr_mod;

return 0;
}

/* 
 * 文件的删除
 * 例：delete_file("123"); //删除当前文件夹下名字为123的文件
 * 
 */
unsigned char delete_file(unsigned char *name)
{
	xdata unsigned int ptr_mod = 0;
	xdata unsigned int next_file_ptr = 0;
	xdata unsigned int file_ptr = 0;
	xdata unsigned int i = 0;
	xdata unsigned int count = 0;
	
	ptr_mod = fs_struct_ptr;
	
	if(ptr_mod = find_file(ptr_mod, name, 1))
	{
		
		if(!(iap_read(ptr_mod) & 0x08))
		{
			printf("Permition denied\r\n");
			return 1;
		}
		else
		{
			/* 判断是否是文件夹 */
			if(iap_read(ptr_mod) & 0x02)
			{
				if(find_file(ptr_mod, "*", 1)) 	//如果是文件夹的话，里面是否有文件
				{
					printf("There are files in this folder, please delete them first\r\n");
					return 1;	//有文件，返回删除错误
				}
				else
				{
					file_ptr = ptr_mod;
					ptr_mod = find_file(fs_struct_ptr, name, 0);	//找到这个结构体中相应的node节点的位置
					/* 把原来的文件标志缓存起来 */
					for(i = 0; i < FS_NEED_KEEP; i ++)
					{
						if(i == (ptr_mod + 28))
						{
							/* 将刚刚删除的文件node节点擦除 */
							erase_return_buff[i] = 0xFF;
							i ++;
							erase_return_buff[i] = 0xFF;
						}
						else
						{
							erase_return_buff[i] = iap_read(fs_struct_ptr + i);
						}
					}					
					iap_erase(fs_struct_ptr);  //擦除当前文件夹下面的扇区
					for(i = 0; i < FS_NEED_KEEP; i ++)
					{
						iap_write(fs_struct_ptr + i, erase_return_buff[i]);
					}

					printf("\r\n");

					iap_erase(file_ptr);	//擦除文件块扇区

					return 0;
				}
			}
			else	//此文件块是一个文本文件
			{
				ptr_mod = ptr_mod + FS_FLAG_SIZE + FS_NAME_SIZE + FS_SIZE_SIZE + FS_PREV_SIZE + FS_NEXT_SIZE;
				/* 查找并且删除文件块 */
				for(i = 0; i < FS_NODE_SIZE; i += 2)
				{
					if(iap_read(ptr_mod + i) == 0xFF)
					{
						break;	//遇到FF说明文本已经查找完毕
					}
					else
					{
						next_file_ptr = iap_read(ptr_mod + i + 1);	//读高字节放入ptr_mod
						next_file_ptr = next_file_ptr << 8;					//左移8位，放入ptr_mod高字节
						next_file_ptr |= iap_read(ptr_mod + i);		//读低字节放入ptr_mod低8位
						
						ENTER_CRITICAL;	//需要长时间执行的程序要先进入临界段
//						putchar((next_file_ptr >> 12) + 48);
//						putchar(((next_file_ptr >> 8) & 0x0F) + 48);
//						putchar(((next_file_ptr >> 4) & 0x0F) + 48);
//						putchar(((next_file_ptr >> 0) & 0x0F) + 48);
						iap_erase(next_file_ptr);	//擦除文件块
						EXIT_CRITICAL;	//写完退出临界段			
					}
				}//End for

				ENTER_CRITICAL;	//需要长时间执行的程序要先进入临界段
				file_ptr = find_file(fs_struct_ptr, name, 1);	//找到这个结构体中相应的文件的位置

				ptr_mod = find_file(fs_struct_ptr, name, 0);	//找到这个结构体中相应的node节点的位置
//				putchar((ptr_mod >> 12) + 48);
//				putchar(((ptr_mod >> 8) & 0x0F) + 48);
//				putchar(((ptr_mod >> 4) & 0x0F) + 48);
//				putchar(((ptr_mod >> 0) & 0x0F) + 48);
				/* 把原来的文件标志缓存起来 */
				for(i = 0; i < FS_NEED_KEEP; i ++)
				{
					if(i == (ptr_mod + 28))
					{
						/* 将刚刚删除的文件node节点擦除 */
						erase_return_buff[i] = 0xFF;
						i ++;
						erase_return_buff[i] = 0xFF;
					}
					else
					{
						erase_return_buff[i] = iap_read(fs_struct_ptr + i);
					}
				}					
				iap_erase(fs_struct_ptr);  //擦除当前文件夹下面的扇区
				for(i = 0; i < FS_NEED_KEEP; i ++)
				{
					iap_write(fs_struct_ptr + i, erase_return_buff[i]);
				}

//				putchar((file_ptr >> 12) + 48);
//				putchar(((file_ptr >> 8) & 0x0F) + 48);
//				putchar(((file_ptr >> 4) & 0x0F) + 48);
//				putchar(((file_ptr >> 0) & 0x0F) + 48);
				iap_erase(file_ptr);	//删除此文件在当前文件夹下的记录
				EXIT_CRITICAL;	//写完退出临界段
			}
			return 0;			
		}		
	}

	printf("No such file or dir\r\n");
	return 1;
}
#endif