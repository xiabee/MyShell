/**
 * ------------------mytree-----------------------
 *  Show the structure of the directory, you can enter the number of layers
 *
 *	Author: Gary Lao
 *	Date  : 2020.1.19
 *  Compiling environment:	gcc version 10.2.1 20201207 (Debian 10.2.1-1)
 * ---------------------------------------------
 */

#include "shell.h"

//栈，用于辅助输出，值为0说明该层目录已输出完毕，值为1说明该层未输出完毕
int mytree_flag[15];

//该函数实现输出非最后一条记录的形式
void mytree_print1(int n)
{
    for (int i = 1; i < n; i++)
    {
        if (mytree_flag[i] == 1)
        {
            printf("│   ");
        }
        else
        {
            printf("    ");
        }
    }
    printf("├── ");
    return;
}

//该函数实现输出最后一条记录的形式
void mytree_print2(int n)
{
    for (int i = 1; i < n; i++)
    {
        if (mytree_flag[i] == 1)
        {
            printf("│   ");
        }
        else
        {
            printf("    ");
        }
    }
    printf("└── ");
    return;
}

//使用深度优先的思路实现对文件树的输出
void mytree_dfs(char *_path, int target_deep, int this_deep)
{
    //调整路径防越界
    char *current_dir = getcwd(NULL, 0);
    chdir(_path);

    //路径副本的初始化
    char the_path[1024];
    memset(the_path, 0, sizeof(the_path));
    strcpy(the_path, ".");

    //打开目标文件，（存在性已经得到保证）
    DIR *dir = NULL;
    dir = opendir(the_path);

    //记录目录内容的信息
    struct dirent *items;
    struct stat the_stat;

    //文件名副本的初始化
    char c_name[1024];
    memset(c_name, 0, sizeof(c_name));

    //该层目录被激活，未完成输出
    mytree_flag[this_deep] = 1;

    //读目录第一行
    if ((items = readdir(dir)) == NULL)
    {
        return;
    }

    //循环输出该目录所含文件信息
    while (1)
    {
        //如果输出完毕，则退出循环
        if (mytree_flag[this_deep] == 0)
        {
            break;
        }

        //记录文件名至副本
        strcpy(c_name, items->d_name);

        //读取下一项文件，若读到NULL，则说明现在一项是最后一项
        if ((items = readdir(dir)) == NULL)
        {
            //将mytree_flag[this_deep]置0
            mytree_flag[this_deep] = 0;

            //选择最后一项形式输出
            mytree_print2(this_deep);
            printf("%s", c_name);
        }
        else
        {
            //选择非最后一项形式输出
            mytree_print1(this_deep);
            printf("%s", c_name);
        }

        //处理两项不输出类型的
        if (strcmp(c_name, ".") == 0 || strcmp(c_name, "..") == 0)
        {
            printf("\n");
            continue;
        }

        //修改路径
        strcpy(the_path, "./");
        strcat(the_path, c_name);
        lstat(the_path, &the_stat);

        //输出文件类型
        if (S_ISDIR(the_stat.st_mode))
        {
            if (this_deep == target_deep)
            {
                printf(" dir\n");
            }
            else
            {
                //若不为最后一层，则不用强调是目录（因为已经对该目录展开了）
                printf("\n");

                //同时调用dfs函数，层数加一
                mytree_dfs(the_path, target_deep, this_deep + 1);
            }
        }
        else if (S_ISLNK(the_stat.st_mode))
        {
            printf(" link\n");
        }

        else
        {
            printf(" file\n");
        }
    }
    closedir(dir);

    //返回上一层目录
    chdir(current_dir);
    return;
}

//提高函数的封装性
void mytree_(char *Dir, int n)
{
    mytree_dfs(Dir, n, 1);
    return;
}

int mytree(int argc, char *argv[])
{
    //处理用户输入
    //用户输入必须为2或3个，调用本指令为第一个，目标地址是第二个
    if (argc != 2 && argc != 3)
    {
        printf("Parameter error!\n");
        printf("Usage1: mytree <path>\n");
        printf("Usage2: mytree <path> <depth>\n\n");

        return 0;
    }
    //n用来记录树的深度
    int n = 0;
    //默认树的深度是5，仅允许用户输入1~9 作为树的深度
    if (argc == 2)
    {
        n = 5;
    }
    else
    {
        if (argv[2][1] != '\0')
        {
            printf("Please input 1~9 for depth\n");
            return 0;
        }
        if (argv[2][0] < '1' || argv[2][0] > '9')
        {
            printf("Please input 1~9 for depth\n");
            return 0;
        }
        n = argv[2][0] - '0';
    }

    struct stat the_stat;

    char *current_dir = getcwd(NULL, 0);

    //测试目标路径是否存在，若不存在则报错
    if (chdir(argv[1]))
    {
        printf("Please input a corrert dir!\n\n");
        return -1;
    }
    chdir(current_dir);

    //输出树根的信息
    printf("%s\n", argv[1]);

    //调用函数输出树
    mytree_(argv[1], n);

    return 1;
}

// int main(int argc, char *argv[])
// {
//     mytree(argc, argv);
//     return 0;
// }
