#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/shm.h>
#include "command.h"
int sharememory(int ipc_size,int flag)
{
	int id;
	key_t key=ftok("/tmp",66);
	if(key < 0)
	{
		printf("get key error\n");
		return -1;
	}
	id = shmget(key,ipc_size,flag);
	if(id < 0)
	{
		printf("get id error\n");
		return -1;
	}
	return id;
}
 
int create_ipc(int ipc_size)
{
	return sharememory(ipc_size,IPC_CREAT|IPC_EXCL|0666);
}
int get_ipc(int ipc_size)
{
	return sharememory(ipc_size,IPC_CREAT|0666);
}
int destroy_sharememory(int id)
{
	return shmctl(id,IPC_RMID,NULL);
}