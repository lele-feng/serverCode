#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<stdlib.h>
#include "Command.h"
 
int main()
{
	int id=create_ipc(sizeof(ckx));
	int i=0;
	ckx *p;
	if(id < 0)
	{
		printf("create sharememory error\n");
		return 0;
	}
	id = 0;
	while(1)
	{
		printf("\n\n1.input data to sharememory\n2.get sharememory data\n\
3.destroy sharememory\ninput select:");
		scanf("%d",&i);
		if(i > 3 |i< 1)
		{
			printf("input error\n");
			continue;
		}
		
		id = get_ipc(sizeof(ckx));
		if(id < 0)
		{
			printf("get sharememory error\n");
			break;
		}
		p = (ckx *)shmat(id,NULL,0);
		if(p < 0)
		{
			printf("get sharememory addr error\n");
			p = NULL;
			break;
		}
		
		if(i == 1)
		{
			char name[NAME_LEN];
			int age=0;
			
			printf("input name:");
			fflush(stdin);
			getchar();
			gets(name);
			printf("input age:");
			scanf("%d",&age);
			
			strcpy(p->name,name);
			p->age = age;
			printf("write success\n");
 
			if(shmdt(p) == -1)
			{
				printf("shmdt error\n");
			}
			id = 0;
		}
		if(i ==  2)
		{
			printf("name:%s \t age:%d\n",p->name,p->age);
			if(shmdt(p) == -1)
			{
				printf("shmdt error\n");
				break;
			}
			id = 0;
		}
		if(i == 3)
		{
			if(shmdt(p) == -1)
			{
				printf("shmdt error\n");
				break;
			}
			break;
		}
	}
	if(id !=0)
	{
		if(destroy_sharememory(id)<0)
		{
			printf("destroy error\n");
		}
	}
}