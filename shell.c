#include "shell.h"

int find(char *str, char *ch) // 查询字符第一次出现的位置
{
    int len1 = strlen(str);
    int len2 = strlen(ch);
    int flag = 1;

    if (len1 < len2)
        return -1;
    for (int i = 0; i < len1 - len2 + 1; i++)
    {
        flag = 1;
        for (int j = 0; j < len2; j++)
        {
            if (str[i + j] != ch[j])
            {
                flag = 0;
                break;
            }
        }
        if (flag)
            return i;
    }
    return -1;
}

void Init() // shell初始化
{
    cnt = 0;
    memset(arglist, 0x00, sizeof(arglist));
    // 初始化命令列表

    char pwd[LEN];
    char name[LEN];
    struct passwd *pass;

    gethostname(name, sizeof(name) - 1);
    pass = getpwuid(getuid());

    getcwd(pwd, sizeof(pwd) - 1);
    int len = strlen(pwd);
    char *ph = pwd + len - 1;
    while (*ph != '/' && len--)
        ph--;
    ph++;

    printf("\e[32;1m[%s @ %s %s]\n\e[0m\e[31;1m$ \e[0m", pass->pw_name, name, ph);
    // 输出用户名、主机名、目录名
    return;
}

int execute(char *arglist[]) // 执行外部指令
{
    int err;
    err = execvp(arglist[0], arglist);
    if (-1 == err)
    {
        printf("Execute Failed!\n");
        exit(-1);
    }
    return err;
}

char *make(char *buf) // 将字符传入参数表内,并分配空间
{
    char *arc;
    arc = malloc(strlen(buf) + 1);
    // 分配堆内存

    if (arc == NULL)
    {
        fprintf(stderr, "No Memory!\n");
        exit(1);
    }

    strcpy(arc, buf);
    return arc;
}

int mystrtok(char *str, char *delim) // 对传入参数进行分割
{
    /*
    * 这里使用 strtok_r 函数，线程安全
    * 若直接使用 strtok 函数，多线程时会导致异常
    * 
    * str: 传入字符串
    * delim: 分割符
    */

    char *token = NULL;
    // strtok_r 原型函数的token指针

    char *save = NULL;
    // strtok_r 原型函数的保存指针

    int num = 0;
    // 参数计数器

    char chBuffer[MAXN];

    char *token1 = NULL;
    char *tmp = NULL;
    // 用于记录切分前的token

    char *save1 = NULL;
    // 用于切分重定向符号和管道符号

    char *operator[] = {">", "<", "|"};
    int opline = sizeof(operator) / sizeof(operator[0]);
    // 记录操作符与操作符个数, 使用 opline 增强可移植性

    int flag_op = 0;
    // 标记是否存在符号与参数相连

    strncpy(chBuffer, str, sizeof(chBuffer) - 1);
    token = chBuffer;

    tmp = make(token);
    while (NULL != (token = strtok_r(token, delim, &save)))
    {
        for (int i = 0; i < opline; i++)
        {
            if (strlen(token) > 1 && find(token, operator[i]) != -1)
            {
                flag_op = 1;
                if (find(token, operator[i]) == 0) // 首位出现, 例如 ls >a.txt
                {
                    arglist[num++] = make(operator[i]);
                    token1 = strtok_r(token, operator[i], &save1);
                    arglist[num++] = make(token1);
                }

                else if (find(token, operator[i]) == strlen(token) - 1) // 末位出现, 例如 ls> a.txt
                {
                    token1 = strtok_r(token, operator[i], &save1);
                    arglist[num++] = make(token1);
                    arglist[num++] = make(operator[i]);
                }

                else
                {
                    token1 = strtok_r(token, operator[i], &save1);
                    arglist[num++] = make(token1);

                    arglist[num++] = make(operator[i]);
                    arglist[num++] = make(save1);
                }

                token1 = NULL;
                save1 = NULL;

                strcmp(token, tmp);
                // 还原token
            }
            // 字符内出现操作符
        }

        arglist[num++] = make(token);

        token = NULL;
        // strtok_r 第一个形参需设置为NULL
    }

    return num;
}

int inner(char *arglist[]) //执行内置指令
{
    if (strcmp(arglist[0], "exit\0") == 0) //exit
    {
        exit(0);
        return 1;
    }

    else if (strcmp(arglist[0], "pwd\0") == 0) //pwd
    {
        char buf[LEN];
        getcwd(buf, sizeof(buf)); //获得当前目录
        printf("%s\n\n", buf);
        return 1;
    }

    else if (strcmp(arglist[0], "cd\0") == 0) //cd
    {
        char buf[LEN];
        int sta = chdir(arglist[1]);
        if (sta < 0) // 找不到文件夹时
        {
            printf("\e[31;1mcd: No such file or directory: %s\n\n\e[0m", arglist[1]);
            return 0;
        }
        printf("\n");
        return 1;
    }

    else
        return 0;
}

int callCommandWithRedi(int left, int right) // 所要执行的指令区间[left, right)，不含管道，可能含有重定向
{
    /* 判断是否有重定向 */
    int inNum = 0, outNum = 0;
    char *inFile = NULL, *outFile = NULL;
    int endIdx = right;
    // 指令在重定向前的终止下标

    for (int i = left; i < right; ++i)
    {
        if (strcmp(arglist[i], COMMAND_IN) == 0 && strcmp(arglist[i], COMMAND_IN2) != 0)
        {
            // 输入重定向
            inNum++;
            if (i + 1 < right)
                inFile = arglist[i + 1];
            else
                return ERROR_MISS_PARAMETER;
            // 重定向符号后缺少文件名

            if (endIdx == right)
                endIdx = i;
        }
        else if (strcmp(arglist[i], COMMAND_OUT) == 0 && strcmp(arglist[i], COMMAND_OUT2) != 0)
        {
            // 输出重定向
            outNum++;
            if (i + 1 < right)
                outFile = arglist[i + 1];
            else
                return ERROR_MISS_PARAMETER;
            // 重定向符号后缺少文件名

            if (endIdx == right)
                endIdx = i;
        }
    }
    /* 处理重定向 */
    if (inNum == 1)
    {
        FILE *fp = fopen(inFile, "r");
        if (fp == NULL)
            return ERROR_FILE_NOT_EXIST;
        // 输入重定向文件不存在

        fclose(fp);
    }

    if (inNum > 1)
        return ERROR_MANY_IN;

    else if (outNum > 1)
        return ERROR_MANY_OUT;

    int result = RESULT_NORMAL;
    pid_t pid = vfork();
    if (pid == -1)
    {
        result = ERROR_FORK;
    }
    else if (pid == 0)
    {
        /* 输入输出重定向 */
        if (inNum == 1)
            freopen(inFile, "r", stdin);
        if (outNum == 1)
            freopen(outFile, "w", stdout);

        /* 执行命令 */
        char *comm[MAXN];
        for (int i = left; i < endIdx; ++i)
            comm[i] = arglist[i];
        comm[endIdx] = NULL;
        execvp(comm[left], comm + left);
        exit(errno); // 执行出错，返回errno
    }

    else
    {
        int status;
        waitpid(pid, &status, 0);
        int err = WEXITSTATUS(status); // 读取子进程的返回码

        if (err)
        { // 返回码不为0，意味着子进程执行出错，用红色字体打印出错信息
            printf("\e[31;1mError: %s\n\e[0m", strerror(err));
        }
    }

    return result;
}

int callCommandWithPipe(int left, int right) // 所要执行的指令区间[left, right)，可能含有管道
{
    if (left >= right)
        return RESULT_NORMAL;
    /* 判断是否有管道命令 */
    int pipeIdx = -1;
    for (int i = left; i < right; ++i)
    {
        if (strcmp(arglist[i], COMMAND_PIPE) == 0)
        {
            pipeIdx = i;
            break;
        }
    }
    if (pipeIdx == -1)
    { // 不含有管道命令
        return callCommandWithRedi(left, right);
    }
    else if (pipeIdx + 1 == right)
    { // 管道命令'|'后续没有指令，参数缺失
        return ERROR_PIPE_MISS_PARAMETER;
    }

    /* 执行命令 */
    int fds[2];
    if (pipe(fds) == -1)
    {
        return ERROR_PIPE;
    }
    int result = RESULT_NORMAL;
    pid_t pid = vfork();
    if (pid == -1)
    {
        result = ERROR_FORK;
    }
    else if (pid == 0)
    { // 子进程执行单个命令
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO);
        // 将标准输出重定向到fds[1]

        close(fds[1]);

        result = callCommandWithRedi(left, pipeIdx);
        exit(result);
    }
    else
    {
        // 父进程递归执行后续命令
        int status;
        waitpid(pid, &status, 0);
        int exitCode = WEXITSTATUS(status);

        if (exitCode != RESULT_NORMAL)
        {
            // 子进程的指令没有正常退出，打印错误信息

            char info[4096] = {0};
            char line[MAXN];
            close(fds[1]);
            dup2(fds[0], STDIN_FILENO);
            // 将标准输入重定向到fds[0]

            close(fds[0]);
            while (fgets(line, MAXN, stdin) != NULL)
            {
                // 读取子进程的错误信息
                strcat(info, line);
            }

            printf("%s", info);
            // 打印错误信息

            result = exitCode;
        }
        else if (pipeIdx + 1 < right)
        {
            close(fds[1]);
            dup2(fds[0], STDIN_FILENO);
            // 将标准输入重定向到fds[0]

            close(fds[0]);
            result = callCommandWithPipe(pipeIdx + 1, right);
            // 递归执行后续指令
        }
    }

    return result;
}

int callCommand(int commandNum)
{ // 给用户使用的函数，用以执行用户输入的命令
    pid_t pid = fork();
    if (pid == -1)
    {
        return ERROR_FORK;
    }
    else if (pid == 0)
    {
        /* 获取标准输入、输出的文件标识符 */
        int inFds = dup(STDIN_FILENO);
        int outFds = dup(STDOUT_FILENO);

        int result = callCommandWithPipe(0, commandNum);

        /* 还原标准输入、输出重定向 */
        dup2(inFds, STDIN_FILENO);
        dup2(outFds, STDOUT_FILENO);
        exit(result);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}

int main()
{
    char buf[MAXN];
    int count;
    // 用于记录缓冲区字符和字符个数

    int pid;
    int num;
    int result;

    while (1)
    {
        Init();
        fflush(stdout);

        fgets(buf, BUFFSIZE, stdin);
        //读入单行指令

        if (strcmp(buf, "\n") == 0)
        {
            printf("\n");
            continue;
        }
        // 仅读到回车

        num = mystrtok(buf, " \b\r\n\t");
        // 处理命令，分割为多个参数

        int inner_flag;
        inner_flag = inner(arglist);
        //内置指令判断

        if (inner_flag)
            continue;

        result = callCommand(num);

        switch (result)
        {
        case ERROR_FORK:
            fprintf(stderr, "\e[31;1mError: Fork error.\n\e[0m");
            exit(ERROR_FORK);
        case ERROR_COMMAND:
            fprintf(stderr, "\e[31;1mError: Command not exist in myshell.\n\e[0m");
            break;
        case ERROR_MANY_IN:
            fprintf(stderr, "\e[31;1mError: Too many redirection symbol \"%s\".\n\e[0m", COMMAND_IN);
            break;
        case ERROR_MANY_OUT:
            fprintf(stderr, "\e[31;1mError: Too many redirection symbol \"%s\".\n\e[0m", COMMAND_OUT);
            break;
        case ERROR_FILE_NOT_EXIST:
            fprintf(stderr, "\e[31;1mError: Input redirection file not exist.\n\e[0m");
            break;
        case ERROR_MISS_PARAMETER:
            fprintf(stderr, "\e[31;1mError: Miss redirect file parameters.\n\e[0m");
            break;
        case ERROR_PIPE:
            fprintf(stderr, "\e[31;1mError: Open pipe error.\n\e[0m");
            break;
        case ERROR_PIPE_MISS_PARAMETER:
            fprintf(stderr, "\e[31;1mError: Miss pipe parameters.\n\e[0m");
            break;

            printf("\n");
        }
    }
    return 0;
}