/**
 * ------------------myline-----------------------
 *  count the total lines of a directory or file
 *
 *	Author: XC
 *	Date  : 2020.1.20
 * ---------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

// get number of lines of a file
int myline4file(char *path)
{
    // open file
    FILE *fp;
    fp = fopen(path, "r");

    if (fp == NULL)

    {
        puts("Can NOT open file!");
        return -1;
    }

    char ch;
    int row = 1, cnt = 0;

    // start counting lines
    while (feof(fp) == 0)
    {
        ch = fgetc(fp);
        cnt++;

        if (ch == '\n')

        {
            row++;
        }
    }

    fclose(fp);

    printf("%s\t%d\n", path, row);

    return row;
}

// get number of lines of a dir or file
int Myline(char *path)
{
    int total_lines = 0;

    // get info
    struct stat st;
    stat(path, &st);

    // path is a file
    if (S_ISREG(st.st_mode))

    {
        total_lines = myline4file(path);
        return total_lines;
    }

    // path is a dir
    DIR *dir;
    dir = opendir(path);

    // get presently working path
    char pwd[NAME_MAX];
    memset(pwd, 0, sizeof(pwd));
    if (getcwd(pwd, NAME_MAX) == NULL)
    {
        puts("Error in geting pwd!");
        return -1;
    }

    // deal with directory
    struct dirent *dp;
    while (1)
    {
        dp = readdir(dir);

        // reach the end
        if (dp == NULL)

        {
            break;
        }

        // get name
        char child_name[NAME_MAX];
        memset(child_name, 0, sizeof(child_name));
        sprintf(child_name, "%s", dp->d_name);

        // deal with "."
        if (strcmp(child_name, ".") == 0)

        {
            continue;
        }

        // deal with ".."
        if (strcmp(child_name, "..") == 0)

        {
            continue;
        }

        unsigned char child_type = dp->d_type;

        // change working directory
        chdir(path);

        // recursively remove files and directories
        if (child_type == 4)

        {
            int line_sub;
            line_sub = Myline(child_name);
            if (line_sub == -1)
            {
                return -1;
            }
            else
            {
                total_lines += line_sub;
            }
        }

        else if (child_type == 8)

        {
            int line_sub;
            line_sub = myline4file(child_name);
            if (line_sub == -1)
            {
                return -1;
            }
            else
            {
                total_lines += line_sub;
            }
        }
    }

    // must close it before remove it
    closedir(dir);

    // switch back
    chdir(pwd);

    return total_lines;
}

int myline(int argc, char *argv[])
{
    // deal with the parameters
    if (argc < 2)
    {
        puts("Parameter error!");
        puts("Usage1: myline <dir>");
        puts("Usage2: myline <file>");
        return -1;
    }

    if (argc > 2)
    {
        puts("Parameter error!");
        puts("Usage1: myline <dir>");
        puts("Usage2: myline <file>");
        return -1;
    }

    // count total lines
    int total_lines = Myline(argv[1]);

    if (total_lines != -1)
    {
        printf("total lines: %d\n", total_lines);
    }

    return total_lines;
}