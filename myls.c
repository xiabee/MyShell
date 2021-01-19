#include "shell.h"

/**************************************************/
/*名称：mode2Char
/*描述：将保护模式转换成对应字符串，便于打印
/*作成日期： 2021.1.19
/*参数：int mode, char str[]
/*返回值：void
/*作者：tulerfeng
/***************************************************/
void mode2Char(int mode, char str[])
{
    //权限初始化
    strcpy(str, "----------");

    if (S_ISDIR(mode))
    {
        str[0] = 'd';
    }

    if (S_ISCHR(mode))
    {
        str[0] = 'c';
    }

    if (S_ISBLK(mode))
    {
        str[0] = 'b';
    }

    if ((mode & S_IRUSR))
    {
        str[1] = 'r';
    }

    if ((mode & S_IWUSR))
    {
        str[2] = 'w';
    }

    if ((mode & S_IXUSR))
    {
        str[3] = 'x';
    }

    if ((mode & S_IRGRP))
    {
        str[4] = 'r';
    }

    if ((mode & S_IWGRP))
    {
        str[5] = 'w';
    }

    if ((mode & S_IXGRP))
    {
        str[6] = 'x';
    }

    if ((mode & S_IROTH))
    {
        str[7] = 'r';
    }

    if ((mode & S_IWOTH))
    {
        str[8] = 'w';
    }

    if ((mode & S_IXOTH))
    {
        str[9] = 'x';
    }
}

/**************************************************/
/*名称：uidToName
/*描述：将用户id转换成对应字符串，便于打印
/*作成日期： 2021.1.19
/*参数：uid_t uid
/*返回值：char*
/*作者：tulerfeng
/***************************************************/
char *uidToName(uid_t uid)
{
    struct passwd *pw_ptr;
    static char tmp_str[10];
    //获取用户信息
    if ((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(tmp_str, "%d", uid);
        return tmp_str;
    }
    else
    {
        return pw_ptr->pw_name;
    }
}

/**************************************************/
/*名称：gid2Name
/*描述：将用户所在的组的id转换成对应字符串，便于打印
/*作成日期： 2021.1.19
/*参数：gid_t gid
/*返回值：char*
/*作者：tulerfeng
/***************************************************/
char *gid2Name(gid_t gid)
{
    struct group *grp_ptr;
    static char tmp_str[10];
    //获取组信息
    if ((grp_ptr = getgrgid(gid)) == NULL)
    {
        sprintf(tmp_str, "%d", gid);
        return tmp_str;
    }
    else
    {
        return grp_ptr->gr_name;
    }
}

/**************************************************/
/*名称：printFileInfo
/*描述：根据命令传入的参数，选择性地打印文件的信息
/*作成日期： 2021.1.19
/*参数：char* filename, struct stat* info_p,int arg_mode
/*返回值：void
/*作者：tulerfeng
/***************************************************/
void printFileInfo(char *filename, struct stat *info_p, int arg_mode)
{
    //若未用l参数且当前文件为隐藏文件
    if (!((arg_mode >> 0) & 1) && filename[0] == '.')
    {
        return;
    }
    //若有l参数或当前文件不是隐藏文件
    else
    {
        //若有a参数，打印详细信息
        if ((arg_mode >> 1) & 1)
        {
            char modestr[11];
            mode2Char(info_p->st_mode, modestr);
            printf("%s", modestr);
            printf(" %4d", (int)info_p->st_nlink);
            printf(" %-8s", uidToName(info_p->st_uid));
            printf(" %-8s", gid2Name(info_p->st_gid));
            printf(" %8ld", (long)info_p->st_size);
            printf(" %.12s", 4 + ctime(&info_p->st_mtime));
            printf(" %s\n", filename);
        }
        else
        {
            printf(" %s\n", filename);
        }
    }
}

/**************************************************/
/*名称：myls
/*描述：模拟ls命令，可以通过myls -[op]调用
/*作成日期： 2021.1.19
/*参数：char *arglist
/*返回值：void
/*作者：tulerfeng
/***************************************************/
void myls(int num, char *arglist[])
{
    //参数模式位掩码，规定第一位表示有无a，第二位表示有无l
    int arg_mode = 0;
    //命令后还有参数
    if (num != 1)
    {
        int arg_len = strlen(arglist[1]);
        for (int i = 1; i < arg_len; i++)
        {
            if (arglist[1][i] == 'a')
                arg_mode = (arg_mode | (1 << 0));
            else if (arglist[1][i] == 'l')
                arg_mode = (arg_mode | (1 << 1));
        }
    }

    DIR *dir_ptr;
    struct dirent *direntp;
    if ((dir_ptr = opendir(".")) == NULL)
    {
        fprintf(stderr, "ls: cannot open %s \n", ".");
    }
    else
    {
        //遍历当前目录
        while ((direntp = readdir(dir_ptr)) != NULL)
        {
            struct stat info;
            if (stat(direntp->d_name, &info) == -1)
            {
                perror(direntp->d_name);
            }
            else
            {
                //打印文件信息
                printFileInfo(direntp->d_name, &info, arg_mode);
            }
        }
        closedir(dir_ptr);
    }
}