#define NAME_LEN 20
typedef struct {
	char name[NAME_LEN];
	int age;
}ckx;
int sharememory(int ipc_size,int flag);
int create_ipc(int ipc_size);
int get_ipc(int ipc_size);
int destroy_sharememory(int id);