#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

static ucontext_t uctx_main, uctx_func1, uctx_func2;

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

static void
func1(void)
{
    if (swapcontext(&uctx_func1, &uctx_func2) == -1)
        handle_error("swapcontext");
}

static void func2(void)
{
    if (swapcontext(&uctx_func2, &uctx_func1) == -1)
        handle_error("swapcontext");
}

int main(int argc, char *argv[])
{
    char func1_stack[16384];
    char func2_stack[16384];
    // 保存当前的执行上下文
    if (getcontext(&uctx_func1) == -1)
        handle_error("getcontext");
    // 设置新的栈
    uctx_func1.uc_stack.ss_sp = func1_stack;
    uctx_func1.uc_stack.ss_size = sizeof(func1_stack);
    // uctx_func1对应的协程执行完执行uctx_main
    uctx_func1.uc_link = &uctx_main;
    // 设置协作的工作函数
    makecontext(&uctx_func1, func1, 0);
    // 同上
    if (getcontext(&uctx_func2) == -1)
        handle_error("getcontext");
    uctx_func2.uc_stack.ss_sp = func2_stack;
    uctx_func2.uc_stack.ss_size = sizeof(func2_stack);
    // uctx_func2执行完执行uctx_func1
    uctx_func2.uc_link = (argc > 1) ? NULL : &uctx_func1;
    makecontext(&uctx_func2, func2, 0);
    // 保存当前执行上下文到uctx_main，然后开始执行uctx_func2对应的上下文
    if (swapcontext(&uctx_main, &uctx_func2) == -1)
        handle_error("swapcontext");

    printf("main: exiting\n");
    exit(EXIT_SUCCESS);
}