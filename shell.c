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

#include "shell.h"

int find(char *str, char *ch) // query the position where the character first appears
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

void Init() // shell initialization
{
    argc = 0;
    memset(arglist, 0x00, sizeof(arglist));
    // command list initialization

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
    //Output user name, host name and directory name

    return;
}

int execute(char *arglist[]) // execute external instructions
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

char *make(char *buf) //pass characters into parameter table and allocate space
{
    char *arc;
    arc = malloc(strlen(buf) + 1);
    //Allocate heap memory

    if (arc == NULL)
    {
        fprintf(stderr, "No Memory!\n");
        exit(1);
    }

    strcpy(arc, buf);
    return arc;
}

int mystrtok(char *str, char *delim) // split the incoming parameters
{
    /*
    * strtok_r() function, thread safety
    * If you use the strtok() function directly, it will cause an exception in multithreading
    *
    * str: incoming string
    * delim: separator
    */

    char *token = NULL;
    // Token pointer of prototype function strtok_r()

    char *save = NULL;
    // Save pointer of prototype function strtok_r()

    argc = 0;
    //Parameter counter

    char chBuffer[MAXN];

    char *token1 = NULL;
    char *tmp = NULL;
    // Used to record the token before segmentation

    char *save1 = NULL;
    //Used to segment redirection symbols and pipeline symbols

    char *operator[] = {">>", "<<", ">", "<", "|"};
    int opline = sizeof(operator) / sizeof(operator[0]);
    // Record operators and the number of operators, and use opline to enhance portability
    // ">>" and "<<" not yet realized

    strncpy(chBuffer, str, sizeof(chBuffer) - 1);
    token = chBuffer;

    while (NULL != (token = strtok_r(token, delim, &save)))
    {
        for (int i = 0; i < opline; i++)
        {
            if (strlen(token) > 1 && find(token, operator[i]) != -1)
            {
                if (find(token, operator[i]) == 0) // For example, ls >a.txt
                {
                    arglist[argc++] = make(operator[i]);
                    token1 = strtok_r(token, operator[i], &save1);
                    arglist[argc++] = make(token1);
                }

                else if (find(token, operator[i]) == strlen(token) - 1) // For example, ls> a.txt
                {
                    token1 = strtok_r(token, operator[i], &save1);
                    arglist[argc++] = make(token1);
                    arglist[argc++] = make(operator[i]);
                }

                else
                {
                    token1 = strtok_r(token, operator[i], &save1);
                    arglist[argc++] = make(token1);

                    arglist[argc++] = make(operator[i]);
                    arglist[argc++] = make(save1);
                }

                token1 = NULL;
                save1 = NULL;

                strcmp(token, tmp);
                // restore token
            }
            //An operator appears within a character
        }

        arglist[argc++] = make(token);
        token = NULL;
    }

    return argc;
}

int inner(char *arglist[]) // execute built-in instructions
{
    if (strcmp(arglist[0], "exit\0") == 0) //exit
    {
        printf("Bye~\n");
        exit(0);
        return 1;
    }

    if (strcmp(arglist[0], "help\0") == 0) // help
    {
        printf("XSLF bash, version 5.1.4(1)-release (x86_64-pc-linux-gnu)\n");
        // XSLF is the short names of the four authors: XYJ, SXC, LJH, FKT

        printf("These shell commands are defined internally.  Type 'help' to see this list.\n\n");
        printf("pwd\t\t: Show the current working directory.\n");
        printf("cd\t\t: Change working directory.\n");
        printf("mycp\t\t: Copy entire directory.\n");
        printf("myps\t\t: Display process information.\n");
        printf("myls\t\t: Display file information.\n");
        printf("mytime\t\t: Measure the running time of the process or control the specified running time of the process.\n");
        printf("mytree\t\t: Show the structure of the directory.\n");
        printf("myrm\t\t: Recursively and forcedly remove files and directories.\n");
        printf("mymv\t\t: Move or rename directory or file\n");
        printf("history\t\t: Show input history.\n");
        printf("exit\t\t: Exit the shell.\n");

        printf("\n");

        return 1;
    }

    else if (strcmp(arglist[0], "pwd\0") == 0) //pwd
    {
        char buf[LEN];
        getcwd(buf, sizeof(buf));
        // get current directory

        printf("%s\n\n", buf);
        return 1;
    }

    else if (strcmp(arglist[0], "cd\0") == 0) //cd
    {
        mycd(argc, arglist);
        return 1;
    }

    else if (strcmp(arglist[0], "mycp\0") == 0)
    {

        struct stat statbuf;
        //stat structure

        struct utimbuf timeby;
        // time file structure

        if (Check(argc, arglist, statbuf))
            return -1;

        Mycp(arglist[1], arglist[2]);
        // starting copying

        stat(arglist[1], &statbuf);
        timeby.actime = statbuf.st_atime;
        // modify the time attribute to access the time

        timeby.modtime = statbuf.st_mtime;
        // mofify time

        utime(arglist[2], &timeby);
        printf("Copy Finished!\n");
        printf("\n");
        return 1;
    }

    else if (strcmp(arglist[0], "myps\0") == 0)
    {
        myps();
        return 1;
    }

    else if (strcmp(arglist[0], "myls\0") == 0)
    {
        myls(argc, arglist);
        return 1;
    }

    else if (strcmp(arglist[0], "mytime\0") == 0)
    {
        mytime(argc, arglist);
        return 1;
    }

    else if (strcmp(arglist[0], "mytree\0") == 0)
    {
        mytree(argc, arglist);
        return 1;
    }

    else if (strcmp(arglist[0], "history\0") == 0)
    {
        printf("-------------------------------------\n");
        printf("**  Print Input History until now: **\n");
        for (int i = 0; i < cmd_cnt; i++)
            printf("%s", history[i]);
        printf("-------------------------------------\n");
        return 1;
    }

    else if (strcmp(arglist[0], "myrm\0") == 0)
    {
        myrm(argc, arglist);
        return 1;
    }

    else if (strcmp(arglist[0], "mymv\0") == 0)
    {
        mymv(argc, arglist);
        return 1;
    }

    else if (strcmp(arglist[0], "myline\0") == 0)
    {
        myline(argc, arglist);
        return 1;
    }

    else
        return 0;
}

int callCommandWithRedi(int left, int right) // the instruction interval [left, right) to be executed does not contain pipeline and may contain redirection
{

    int inNum = 0, outNum = 0;
    char *inFile = NULL, *outFile = NULL;
    // determine whether there is a redirection

    int endIdx = right;
    // the ending subscript of an instruction before resetting

    for (int i = left; i < right; ++i)
    {
        if (strcmp(arglist[i], COMMAND_IN) == 0 && strcmp(arglist[i], COMMAND_IN2) != 0)
        {

            inNum++;
            // input redirection

            if (i + 1 < right)
                inFile = arglist[i + 1];

            else
                return ERROR_MISS_PARAMETER;
            // missing file name after redirection symbol

            if (endIdx == right)
                endIdx = i;
        }
        else if (strcmp(arglist[i], COMMAND_OUT) == 0 && strcmp(arglist[i], COMMAND_OUT2) != 0)
        {
            // output redirection
            outNum++;
            if (i + 1 < right)
                outFile = arglist[i + 1];
            else
                return ERROR_MISS_PARAMETER;
            // missing file name after redirection symbol

            if (endIdx == right)
                endIdx = i;
        }
    }

    if (inNum == 1) // processing redirection
    {
        FILE *fp = fopen(inFile, "r");
        if (fp == NULL)
            return ERROR_FILE_NOT_EXIST;
        // The input redirection file does not exist

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

        if (inNum == 1)
            freopen(inFile, "r", stdin);
        if (outNum == 1)
            freopen(outFile, "w", stdout);
        // I/O redirection

        char *comm[MAXN];
        for (int i = left; i < endIdx; ++i)
            comm[i] = arglist[i];
        comm[endIdx] = NULL;
        execvp(comm[left], comm + left);
        // execute the order

        exit(errno);
        // error in execution, return errno
    }

    else
    {
        int status;
        waitpid(pid, &status, 0);
        int err = WEXITSTATUS(status);
        // read the return code of the subprocess

        if (err)
        {
            printf("Command Error!\n");
            printf("You may need \e[31;1m'help'\e[0m\n\n");
        }
        /* The return code is not 0, 
         * which means that the subprocess has made an error in execution, 
         * and the error information is printed in red font
         */
    }

    return result;
}

int callCommandWithPipe(int left, int right) // The instruction interval [left, right] to be executed may contain a pipeline
{
    if (left >= right)
        return RESULT_NORMAL;
    // Determine if there is a pipeline command

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
    {
        return callCommandWithRedi(left, right);
    }
    // Does not contain pipeline command

    else if (pipeIdx + 1 == right)
    {
        return ERROR_PIPE_MISS_PARAMETER;
    }
    // The pipeline command '|' has no subsequent command, and the parameter is missing

    int fds[2];
    if (pipe(fds) == -1)
    {
        return ERROR_PIPE;
    }

    /* Execute the order */
    int result = RESULT_NORMAL;
    pid_t pid = vfork();

    if (pid == -1)
    {
        result = ERROR_FORK;
    }
    else if (pid == 0)
    {
        //The subprocess executes a single command
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO);
        //Redirecting standard output to FDS [1]

        close(fds[1]);

        result = callCommandWithRedi(left, pipeIdx);
        exit(result);
    }
    else
    {
        // The parent process recursively executes subsequent commands
        int status;
        waitpid(pid, &status, 0);
        int exitCode = WEXITSTATUS(status);

        if (exitCode != RESULT_NORMAL)
        {
            // The instruction of the subprocess does not exit normally, and the error message is printed

            char info[4096] = {0};
            char line[MAXN];
            close(fds[1]);
            dup2(fds[0], STDIN_FILENO);
            // Redirect standard input to fds[0]

            close(fds[0]);
            while (fgets(line, MAXN, stdin) != NULL)
            {
                // Read the error information of the child process
                strcat(info, line);
            }

            printf("%s", info);
            // print errors

            result = exitCode;
        }

        else if (pipeIdx + 1 < right)
        {
            close(fds[1]);
            dup2(fds[0], STDIN_FILENO);
            // Redirect standard input to fds[0]

            close(fds[0]);
            result = callCommandWithPipe(pipeIdx + 1, right);
            // Execute subsequent instructions recursively
        }
    }

    return result;
}

int callCommand(int commandNum)
{
    // A function used by a user to execute commands entered by the user
    pid_t pid = fork();
    if (pid == -1)
    {
        return ERROR_FORK;
    }
    else if (pid == 0)
    {

        /*Get the file identifier of standard input and output*/
        int inFds = dup(STDIN_FILENO);
        int outFds = dup(STDOUT_FILENO);

        int result = callCommandWithPipe(0, commandNum);

        /*Restore standard input and output redirection*/
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

    int pid;
    int result;

    cmd_cnt = 0;
    memset(history, 0x00, sizeof(history));
    // Initialize history command

    while (1)
    {

        Init();
        fflush(stdout);

        fgets(buf, BUFFSIZE, stdin);
        // Read in single line instruction

        if (strcmp(buf, "\n") == 0)
        {
            printf("\n");
            continue;
        }
        // Only "Enter"

        strcpy(history[cmd_cnt], buf);
        cmd_cnt++;
        if (cmd_cnt >= MAXN)
        {
            cmd_cnt = 0;
            memset(history, 0x00, sizeof(history));
            printf("--------------------------------------------\n");
            printf(" Waring: Lack of space! Reset Input History!\n");
            printf("--------------------------------------------\n");
        }
        // overfloor, reset history[][]

        memset(arglist, 0x00, sizeof(arglist));

        argc = mystrtok(buf, " \b\r\n\t");
        // Processing command, divided into multiple parameters

        int inner_flag;
        inner_flag = inner(arglist);
        // Built in instruction judgment

        if (inner_flag)
            continue;

        result = callCommand(argc);

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