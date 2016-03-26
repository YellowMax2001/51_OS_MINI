#include "char.h"

/* 清除字串，长度为len个字节 */
void clear_char_str(unsigned char *str2clear, unsigned int len)
{
	xdata unsigned int i;	

	for(i = 0; i < len; i++)
	{
		str2clear[i] = 0;
	}
}

/* 比较两个字符串是否相同，相同返回0，否则返回1 */
unsigned char os_strcmp(unsigned char *str1, unsigned char *str2)
{
	while((*str1) || (*str2))
	{
		if(*str1++ != *str2++)
			return 1;
	}
	return 0;
}

/* 字符串按空格进行拆分 */ 
unsigned char split_str(unsigned char *str2split, unsigned char split[][20]) 
{
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char space_end_flag = 1;
	
	if(*str2split == ' ')
	{
		return 1;	//第一个字母就是空格，则退出，显示未识别命令 
	} 
	
	while(*str2split)
	{
		if(*str2split == ' ')
		{
			split[i][j] = '\0';
			space_end_flag = 0;
		}
		else
		{
			if(space_end_flag == 0)
			{
				i ++;
				j = 0;
				space_end_flag = 1;
			}
			split[i][j] = (*str2split);
		}
		str2split ++;
		j ++; 
	}
	split[i][j] = '\0';
	split[i+1][0] = '\0';
	
	return 0;
}