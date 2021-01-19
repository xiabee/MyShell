/**
 * ------------------mytime-----------------------
 *	Measure the running time of the process and control the specified running time of the process
 *
 *	Author: xiabee
 *	Date  : 2020.1.19
 *  Compiling environment:	gcc version 10.2.1 20201207 (Debian 10.2.1-1)
 * ---------------------------------------------
 */

#include "shell.h"
void mytime(int argc, char *argv[])
{
    struct timeval start_time;
    struct timeval end_time;
    // 精确到微秒
    double running_time = 0;

    pid_t pid;

    if (argc <= 1)
    {
        printf("Please input the process name!\n");
        return;
    }

    pid = fork();
    // 获取进程pid

    if (pid < 0)
    {
        printf("fork failed!\n");
        return;
    }
    // 进程出错，fork失败

    if (pid == 0) //返回子进程
    {
        // printf("Create Child:\n");
        gettimeofday(&start_time, NULL);

        // execvl(argv[1], &argv[1]);
        execvp(argv[1], &argv[1]);
        exit(errno);
    }

    else
    {
        int status;
        gettimeofday(&start_time, NULL);
        wait(&status);
        gettimeofday(&end_time, NULL);
        running_time = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
        printf("running time: %lf ms\n", running_time / 1000);
    }

    return;
}