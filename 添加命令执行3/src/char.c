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

/* 比较两个字符串是否相同 */
unsigned char os_strcmp(unsigned char *str1, unsigned char *str2)
{
	while((*str1) || (*str2))
	{
		if(*str1++ != *str2++)
			return 1;
	}
	return 0;
}