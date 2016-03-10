#include <stdio.h>
unsigned char task_run_flag = 0x01; //0000 0001
unsigned char current_id = 0;

void task_create()
{
	static unsigned char i = 0;

	for(i = 0; i < 8; i ++)
	{
		if(task_run_flag & (1 << i))
			;
		else
		{
			task_run_flag |= (1 << i);		//把此位设置为有任务
			
			return;	//任务创建成功，返回
		}
	}
}

int main(int argv, char *argc[])
{
	unsigned char temp = 0;
	
	while(1)
	{
		for(temp = current_id + 1; temp < 8; temp ++)
		{
			if(task_run_flag & (1 << temp))
			{
				current_id = temp;
				goto T2_switch_exit;			
			}
		}
		
		for(temp = 0; temp < current_id + 1; temp ++)
		{
			if(task_run_flag & (1 << temp))
			{
				current_id = temp;
				goto T2_switch_exit;	
			}
			else
				goto T2_switch_exit;
		}
	
	T2_switch_exit:
		printf("%d", current_id);
		while(getchar() != '\n')
		{
			if(getchar() == 'c')
			{
				task_create();
			}
		}
		
			
	}
	
	return 1;
} 
