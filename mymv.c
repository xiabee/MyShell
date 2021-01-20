/**
 * ------------------mymv-----------------------
 *  move or rename directory or file
 *
 *	Author: XC
 *	Date  : 2020.1.20
 * ---------------------------------------------
 */

#include "shell.h"

// #include "myrm.c"

#define tpDIRECTORY 4
#define tpFILE 8
#define tpLINK 10

int mymv_main(char *src, char *tar)
{
	// get information of origin directory
	struct stat st;
	if (lstat(src, &st))

	{
		printf("lstat error\n");
		return 2;
	}

	// src is a file
	if (S_ISREG(st.st_mode))

	{
		int fd_src = open(src, O_RDONLY);

		// create new file
		int fd_tar = open(tar, O_RDWR | O_CREAT, st.st_mode);
		if (!fd_tar)

		{
			printf("open error\n");
			return 0;
		}

		int len;
		char buffer[BUFSIZ];
		while (len = read(fd_src, buffer, BUFSIZ))
		{
			write(fd_tar, buffer, len);
		}

		// change time
		struct timespec ts[2];
		ts[0] = st.st_atim;
		ts[1] = st.st_mtim;
		if (futimens(fd_tar, ts))
		{
			printf("futimens");
			return 2;
		}

		char pwd[NAME_MAX];
		memset(pwd, 0, sizeof(pwd));

		if (getcwd(pwd, NAME_MAX) == NULL)
		{
			printf("Error in geting pwd\n");
			return 2;
		}
		int argc = 2;
		char *argv[3];
		argv[0] = pwd;
		argv[1] = src;
		argv[2] = NULL;
		int flag = 0;
		flag = mvrm(2, argv);
		return flag;
	}

	// create new directory
	if (mkdir(tar, st.st_mode))

	{
		printf("mkdir error\n");
		return 2;
	}

	DIR *dir = opendir(src);
	struct dirent *de = readdir(dir);
	do

	{
		// get name
		char name[NAME_MAX];
		strcpy(name, de->d_name);

		// deal with "."
		if (!strcmp(name, "."))

		{
			continue;
		}

		// deal with ".."
		if (!strcmp(name, ".."))

		{
			continue;
		}

		// get full path
		char src_sub[NAME_MAX];
		char tar_sub[NAME_MAX];
		sprintf(src_sub, "%s/%s", src, name);
		sprintf(tar_sub, "%s/%s", tar, name);

		// start creating
		unsigned char type = de->d_type;

		// directory, recursively create
		if (type == tpDIRECTORY)

		{
			if (mymv_main(src_sub, tar_sub) != 1)

			{
				printf("mymv_main error\n");
				return 2;
			}
		}

		// deal with link
		else if (type == tpLINK)

		{
			// get path
			char link_path[BUFSIZ];
			int link_len = readlink(src_sub, link_path, BUFSIZ);
			if (link_len == 0)

			{
				printf("readlink error\n");
				return 2;
			}
			link_path[link_len] = '\0';

			// create new link
			if (!symlink(link_path, tar_sub))

			{
				printf("symlink error\n");
				return 2;
			}

			// change time
			struct stat src_sub_st;
			lstat(src_sub, &src_sub_st);
			if (lstat(src, &st))

			{
				printf("lstat error\n");
				return 2;
			}
			struct timespec ts[2];
			ts[0] = src_sub_st.st_atim;
			ts[1] = src_sub_st.st_mtim;

			// flag must set to AT_SYMLINK_NOFOLLOW
			if (!utimensat(AT_FDCWD, tar_sub, ts, AT_SYMLINK_NOFOLLOW))

			{
				printf("utimensat error\n");
			}
		}
		// oridinary file
		else
		{
			// get info
			int fd_src = open(src_sub, O_RDONLY);
			struct stat src_sub_st;
			lstat(src_sub, &src_sub_st);
			if (lstat(src, &st))

			{
				printf("lstat error\n");
				return 2;
			}

			// create new file
			int fd_tar = open(tar_sub, O_RDWR | O_CREAT, src_sub_st.st_mode);
			if (!fd_tar)

			{
				printf("open error\n");
				return 0;
			}
			int len;
			char buffer[BUFSIZ];
			while (len = read(fd_src, buffer, BUFSIZ))

			{
				write(fd_tar, buffer, len);
			}

			// change time
			struct timespec ts[2];
			ts[0] = src_sub_st.st_atim;
			ts[1] = src_sub_st.st_mtim;
			if (futimens(fd_tar, ts))

			{
				printf("futimens");
				return 2;
			}
		}

	} while (de = readdir(dir));

	// change time of this dir
	struct timespec ts[2];
	ts[0] = st.st_atim;
	ts[1] = st.st_mtim;
	utimensat(AT_FDCWD, tar, ts, 0);

	char pwd[NAME_MAX];
	memset(pwd, 0, sizeof(pwd));

	if (getcwd(pwd, NAME_MAX) == NULL)
	{
		printf("Error in geting pwd\n");
		return 2;
	}
	int argc = 2;
	char *argv[3];
	argv[0] = pwd;
	argv[1] = src;
	argv[2] = NULL;
	int flag = 0;
	flag = mvrm(2, argv);
	return flag;
}

int mymv(int argc, char *argv[])
{
	// deal with the parameters
	if (argc < 3)

	{
		puts("Parameter error!");
		puts("Usage: mymv <source> <destination>");
		return 2;
	}

	if (argc > 3)

	{
		puts("Parameter error!");
		puts("Usage: mymv <source> <destination>");
		return 2;
	}

	// pre-operate
	char src[NAME_MAX];
	char des[NAME_MAX];
	if (argv[1][0] != '.')

	{
		sprintf(src, "./%s", argv[1]);
	}

	if (argv[2][0] != '.')

	{
		sprintf(des, "./%s", argv[2]);
	}

	// start move
	int flag = 0;
	flag = mymv_main(src, des);

	if (flag == 1)

	{
		puts("Successfully moved.");
	}
	else

	{
		return 2;
	}
	return 0;
}