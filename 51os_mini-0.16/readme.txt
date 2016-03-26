带根文件系统的51微型操作系统

1、开发工具
单片机：IAP15F2K61S2
IDE：keil4（工程编译优化级别2）
下载工具：stc-isp-15xx-v6.85I（24MHz，115200波特率）
串口工具：SecureCRT 7.2

2、整体系统介绍
内核：抢占式内核、支持优先级256级（暂时不提供串口界面新建自定义优先级的任务）、支持任务延时/挂起、任务数量理论最多32个、任务运行
过程中的新建与删除、文件管理、暂不支持信号量（串口打印未设置锁---一个巨坑）
文件系统：支持文本文件与文件夹的创建与删除（文本文件不超过508个字节）、文件详细类型的查看、文件系统格式化

3、文件说明
工程根目录->
board	与板子有关驱动文件
head	各种头文件
main	主函数存放目录
pro		工程文件
src		源文件

board->
head	头文件 -->
	18b20.h		温度传感器
	at24c02.h	eeprom存储芯片
	ds1302.h	时钟芯片
	hc_sr04.h	超声波
	lcd1602.h	液晶
	nt_led.h	数码管以及led灯
src		源文件 -->
	18b20.c		温度传感器
	at24c02.c	eeprom存储芯片
	ds1302.c	时钟芯片
	hc_sr04.c	超声波
	lcd1602.c	液晶
	nt_led.c	数码管以及led灯
	
head->
char.h		与字符串操作有关的头文件，包括字串对比等等
core.h		内核，任务调度等
common.h	宏开关
eeprom.h	单片机自带EEPROM的驱动
fs.h		文件系统
task.h		具体任务
uart.h		串口操作
vector.h	定时器操作

src->
char.c		与字符串操作有关的头文件，包括字串对比等等
core.c		内核，任务调度等
eeprom.c	单片机自带EEPROM的驱动
fs.c		文件系统
task.c		具体任务
uart.c		串口操作
vector.c	定时器操作

4、指令系统
最重要还是help了，直接运行可以显示所有的命令信息

1.ps:   显示正在运行的任务
2.kill: 杀掉选中的正在运行的任务
        example:
                kill 4 ---kill the 4th task
3.new:  创建一个任务
        example:
                new task2 ---create the task named 'task2'
4.ls:   显示当前文件夹下面的所有文件，加上l选项的时候会显示文件类型，操作权限、文件在内存中的位置
        example:
                ls l ---display in detail
5.mkdir:创建一个文件夹
        example:
                mkdir dir ---make a floder named 'dir'
6.touch:穿件一个文本文件
        example:
                touch txt ---make a file named 'txt'
7.cd:   切换文件夹
        example:
                cd dir ---change to dir named 'dir'
8.mkfs: 格式化整个系统
9.rm:   删除选中的文件 
        example: 
                rm dir ---delete the file named 'dir'
10.vi:  编辑选中的文本文件
        example:
                 vi txt ---edit the file named 'txt'
11.cat: 查看文本文件内容
        example:
                cat txt ---look up the file named 'txt'
12.exit:退出整个系统

5、个性化操作
1、如何添加一个自己的任务到系统中
1.1、在src/task.c文件中添加自己任务函数类型为void task_tty0(void)形式
1.2、在src/task.c的static void new_msghandler(void *msg)函数中加入类似：
else if(!os_strcmp("你的任务名字", cmd_split[1]))
{
	CREATE_TASK_RUNNING(优先级, 你的任务名字, "你的任务名字");
	printf("The task has been created : task name = ");
	printf(cmd_split[1]);
	printf("\r\n");
}的语句
1.3、在main/main.c中添加类似：
task_create(优先级, (unsigned int)(&你的任务名字), 0, "你的任务名字");的语句
1.4、在head/core.h中
#define IDATA_SP_BUFFER		//栈数据缓存区的选择
//可改为XDATA_SP_BUFFER以增加任务数量（因为idata区长度不够）
#define TASK_MAX 5 			//可运行的最多任务数（改大任务数量）
#define SP_DEEP  30			//任务堆栈的深度
#define PUBLIC_SP_DEEP 30	//任务的公共堆栈空间深度（当宏定义XDATA_SP_BUFFER时有效，作用等同于SP_DEEP）
（最后发现在增加了文件系统之后改为XDATA区程序运行出错了，所以无法增加任务了，
  但是可以在轻量版只含内核的那个版本中增加，可以在head/common.h中改变宏定义或者注释宏定义即可）悲剧
注意：由于不提供优先级自动选择，所以请手动选择与已存在优先级不一样的优先级，数字越小代表优先级越高
	  另外在创建的任务中请务必加上{OS_delay(100); TF1 = 1;}这样形式的语句，代表任务执行空闲的时候进行系统延时以供更低优先级的任务轮换
	  创建任务的时候任务的嵌套调用层数尽量不要超过6层，否则有可能会造成运行出错，如果非要超过这个数量，请改为XDATA_SP_BUFFER，并且
	  把PUBLIC_SP_DEEP的数改大一点
2、如何添加一个自己的命令到系统中去
2.1、在code unsigned char cmd_buff[][100]中加入自己命令的描述
2.2、在src/task.c中添加类似：
/* 'cd 'dirname'' ：切换文件夹 */
static void cd_msghandler(void *msg)
{
	change_dir(cmd_split[1]);
}的代码
2.3、在src/task.c的void task_msghandler(void)中添加类似
else if(!os_strcmp("cd", cmd_split[0]))
{
	cd_msghandler(0);
}的语句

注意：不要使命令的描述超过100字节

6、如何进行源代码阅读
全部文件中最重要的几个文件是task.c以及core.c还有fs.c
包括它们的头文件
core.c里面需要关注
unsigned char task_kill(unsigned char pid);	//任务的删除
unsigned char task_create(unsigned char priority, unsigned int task_address, unsigned char xdata *task_msg, unsigned char *name);	//任务的创建
void task_switch(void);	//任务的切换

task.c里面只需要关注
extern void task_msghandler(void);	//对控制台输入的消息进行处理
extern void task_tty0(void);		//命令控制台接收用户输入
上面已经涵盖了一个大致的操作系统内核，下面的文件系统则是附加的可看可不看

fs.c中
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
unsigned int alloc(unsigned int len, unsigned int *pos);	//在内存当中分配空间
void fs_format(void);	//格式化整个系统				
（与mkfs命令有关）
unsigned char create_dir_file(unsigned char *name, unsigned int addr, unsigned int prev_addr, unsigned char flag);	//创建文件
（与mkdir和touch命令有关）
void list_file(unsigned int cur_f_ptr, unsigned char l_flag);	//列出当前文件夹下面所有的文件
（与ls命令有关）
unsigned char change_dir(unsigned char *name);	//切换文件夹
（与cd命令有关）
void fs_write(unsigned char *name);	//编辑文本文件
（与vi命令有关）
void fs_read(unsigned char *name);	//读文本文件
（与cat命令有关）
unsigned char delete_file(unsigned char *name);	//删除文件
（与rm命令有关）

每一个函数都需要阅读以下，顺序上面已经排好，按顺序看会比较容易理解一点