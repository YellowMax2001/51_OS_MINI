#include <stdio.h>
unsigned char task_run_flag = 0x01; //0000 0001
unsigned char current_id = 0;

/* 比较两个字符串是否相同 */
unsigned char mystrcmp(unsigned char *str1, unsigned char *str2)
{
	while((*str1) || (*str2))
	{
		if(*str1++ != *str2++)
			return 1;
	}
	return 0;
}

int main(int argv, char *argc[])
{
	if(!mystrcmp("cytytfytfytfy", "1234"))
	{
		printf("equ\n");
	}
	else
	{
		printf("not equ\n");
	}	
	
	return 0;
} 
