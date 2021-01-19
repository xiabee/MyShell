/**
 * ------------------MyShell-----------------------
 * Main Function of the shell	
 * Simulate the implementation of shell
 *
 *	Author: xiabee
 *	Date  : 2020.1.18
 *  Compiling environment:	gcc version 10.2.1 20201207 (Debian 10.2.1-1)
 * ---------------------------------------------
 */

#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <pwd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <utime.h>
#include <sys/types.h>
#include <grp.h>
#include <unistd.h>
#include <time.h>


#include "mycp.c"
#include "myps.c"
#include "myls.c"
#include "mycd.c"

#define MAXN 1024
#define BUFFSIZE 1024
#define LEN 128


const char *COMMAND_EXIT = "exit";
const char *COMMAND_HELP = "help";
const char *COMMAND_CD = "cd";
const char *COMMAND_IN = "<";
const char *COMMAND_IN2 = "<<";

const char *COMMAND_OUT = ">";
const char *COMMAND_OUT2 = ">>";

const char *COMMAND_PIPE = "|";

char *arglist[MAXN];
// shell 指令的参数表

int num = 0;
// shell指令的参数个数

enum
{
    RESULT_NORMAL,
    ERROR_FORK,
    ERROR_COMMAND,
    ERROR_WRONG_PARAMETER,
    ERROR_MISS_PARAMETER,
    ERROR_TOO_MANY_PARAMETER,
    ERROR_CD,
    ERROR_SYSTEM,
    ERROR_EXIT,

    /* 重定向的错误信息 */
    ERROR_MANY_IN,
    ERROR_MANY_OUT,
    ERROR_FILE_NOT_EXIST,

    /* 管道的错误信息 */
    ERROR_PIPE,
    ERROR_PIPE_MISS_PARAMETER
};
// 设置状态码

int flag;

#endif