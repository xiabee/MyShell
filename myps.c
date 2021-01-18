#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>

//进程信息结构体
typedef struct ps_info
{
    char pname[30];
    char user[30];
    int pid;
    int ppid;
    char state;
    struct ps_info *next;
} psi;


/**************************************************/
/*Name: uid2Name
/*Describe: 由进程uid得到进程的所有者user
/*Date:  2021.1.18
/*Parameter: uid_t uid, struct ps_info *p1
/*Return value: void
/*Author: tulerfeng
/***************************************************/

void uid2Name(uid_t uid, struct ps_info *p1)
{

    struct passwd *pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(numstr, "%d", uid);
        strcpy(p1->user, numstr);
    }
    else
        strcpy(p1->user, pw_ptr->pw_name);
}

/**************************************************/
/*Name: isNum
/*Describe: 判断一个字符串是否只由数字组成，若是，返回0，否则，返回-1
/*Date:  2021.1.18
/*Parameter: char p_name[]
/*Return value: int
/*Author: tulerfeng
/***************************************************/

int isNum(char p_name[])
{
    int i, len;
    len = strlen(p_name);
    if (len == 0)
        return -1;
    for (i = 0; i < len; i++)
        if (p_name[i] < '0' || p_name[i] > '9')
            return -1;
    return 0;
}

/**************************************************/
/*Name: getProcessInfo
/*Describe: 读取进程信息，若读取成功，返回0，否则返回-1，并打印错误信息
/*Date:  2021.1.18
/*Parameter: char d_name[],struct ps_info *p1
/*Return value: int
/*Author: tulerfeng
/***************************************************/

int getProcessInfo(char d_name[], struct ps_info *p1)
{
    FILE *fd;
    char dir[20];
    struct stat infobuf;

    char *path = NULL;
    path = getcwd(NULL, 0);
    // 记录当前目录

    sprintf(dir, "%s/%s", "/proc/", d_name);
    //切换至/proc目录

    chdir("/proc");
    if (stat(d_name, &infobuf) == -1)
        fprintf(stderr, "stat error %s\n", d_name);
    else
        uid2Name(infobuf.st_uid, p1);

    //切换到/proc/pid目录
    chdir(dir);
    if ((fd = fopen("stat", "r")) < 0)
    {
        printf("open the file is error!\n");
        return -1;
    }
    //读文件的前四个字段内容，并存放在相关的链表成员中
    while (4 == fscanf(fd, "%d %s %c %d\n", &(p1->pid), p1->pname, &(p1->state), &(p1->ppid)))
    {
        break;
    }

    chdir(path);
    // 切换回当前工作目录

    fclose(fd);
    free(path);

    return 0;
}


/**************************************************/
/*Name: getAllProcess
/*Describe: 遍历/proc目录所有进程目录,获取所有进程的信息，并存入进程链表中,返回链表的头指针
/*Date:  2021.1.18
/*Parameter: None
/*Return value: psi *
/*Author: tulerfeng
/***************************************************/

psi *getAllProcess()
{
    DIR *dir_ptr;
    psi *head, *p1, *p2;
    char dir[] = "/proc/";
    struct dirent *direntp;
    struct stat infobuf;

    if ((dir_ptr = opendir(dir)) == NULL)
        fprintf(stderr, "dir error %s\n", dir);
    else
    {
        //建立进程链表
        head = p1 = p2 = (struct ps_info *)malloc(sizeof(struct ps_info));
        //遍历/proc目录所有进程目录
        while ((direntp = readdir(dir_ptr)) != NULL)
        {
            //判断目录名字是否为纯数字
            if ((isNum(direntp->d_name)) == 0)
            {
                if (p1 == NULL)
                {
                    printf("malloc error!\n");
                    exit(0);
                }

                //获取进程信息
                if (getProcessInfo(direntp->d_name, p1) != 0)
                {
                    printf("getProcessInfo error\n");
                    exit(0);
                }
                //更新进程链表
                p2->next = p1;
                p2 = p1;
                p1 = (struct ps_info *)malloc(sizeof(struct ps_info));
            }
        }
        closedir(dir_ptr);
    }
    p2->next = NULL;
    return head;
}


/**************************************************/
/*Name: printPS
/*Describe: 遍历进程链表，打印进程信息
/*Date:  2021.1.18
/*Parameter: struct ps_info *head
/*Return value: void
/*Author: tulerfeng
/***************************************************/

void printPS(struct ps_info *head)
{
    psi *list;
    printf("USER\t\tPID\tPPID\tSTATE\tPNAME\n");
    //从头到尾，遍历链表
    for (list = head; list != NULL; list = list->next)
    {
        //用户，id，父进程的id，状态，名字
        printf("%s\t\t%d\t%d\t%c\t%s\n", list->user, list->pid, list->ppid, list->state, list->pname);
    }
}


/**************************************************/
/*Name: myps
/*Describe: 模拟ps命令，打印当前所有进程的信息
/*Date:  2021.1.18
/*Parameter: None
/*Return value: void
/*Author: tulerfeng
/***************************************************/

void myps()
{
    psi *head, *link;

    head = getAllProcess();
    //遍历/proc/目录失败
    if (head == NULL)
        printf("traverse error\n");
    printPS(head);

    //释放链表，防止内存泄漏
    while (head != NULL)
    {
        link = head;
        head = head->next;
        free(link);
    }
}

// int main(int ac, char *av[])
// {
//     myps();
//     return 0;
// }