/**
 * ------------------myrm-----------------------
 *  recursively and forcedly remove files and directories
 *
 *	Author: XC
 *	Date  : 2020.1.20
 * ---------------------------------------------
 */

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define RM_SUCCESS 1
#define RM_FAILURE 2

// recursively and forcedly remove files and directories
int myrm_main(char *target, int mode)
{
    DIR *dir;
    dir = opendir(target);

    // file or nothing
    if (dir == NULL)
    {
        int flag = 0;

        // remove file
        flag = unlink(target);

        if (flag == 0 && mode)

        {
            printf("Successfully removed %s\n", target);
            return RM_SUCCESS;
        }

        else

        {
            return RM_FAILURE;
        }
    }

    // get presently working path
    char pwd[NAME_MAX];
    memset(pwd, 0, sizeof(pwd));
    if (getcwd(pwd, NAME_MAX) == NULL)
    {
        perror("Error in geting pwd");
        return RM_FAILURE;
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
        char tar_sub[NAME_MAX];
        memset(tar_sub, 0, sizeof(tar_sub));
        sprintf(tar_sub, "%s", dp->d_name);

        // deal with "."
        if (strcmp(tar_sub, ".") == 0)

        {
            continue;
        }

        // deal with ".."
        if (strcmp(tar_sub, "..") == 0)

        {
            continue;
        }

        // change working directory
        chdir(target);

        // recursively remove files and directories
        if (myrm_main(tar_sub, mode) == RM_FAILURE)

        {
            return RM_FAILURE;
        }
    }

    // must close it before remove it
    closedir(dir);

    // switch back
    chdir(pwd);

    // remove the current directory
    rmdir(target);

    if (mode)

    {
        printf("Successfully removed %s\n", target);
    }

    return RM_SUCCESS;
}

int myrm(int argc, char *argv[])
{
    // deal with the parameters
    if (argc < 2)

    {
        puts("Parameter error!");
        puts("Usage1: myrm <directory>");
        puts("Usage1: myrm <file>");
        return RM_FAILURE;
    }

    if (argc > 2)

    {
        puts("Parameter error!");
        puts("Usage1: myrm <directory>");
        puts("Usage1: myrm <file>");
        return RM_FAILURE;
    }

    // recursively and forcedly remove files and directories
    int result = myrm_main(argv[1], 1);

    return result;
}

int mvrm(int argc, char *argv[])
{
    // deal with the parameters
    if (argc < 2)

    {
        puts("Parameter error!");
        puts("Usage1: myrm <directory>");
        puts("Usage1: myrm <file>");
        return RM_FAILURE;
    }

    if (argc > 2)

    {
        puts("Parameter error!");
        puts("Usage1: myrm <directory>");
        puts("Usage1: myrm <file>");
        return RM_FAILURE;
    }

    // recursively and forcedly remove files and directories
    int result = myrm_main(argv[1], 0);

    return result;
}