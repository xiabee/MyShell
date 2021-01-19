# MyShell



#### 项目简介

* 功能：手动实现`Linux`的`shell`解释器



#### 编译运行

```
gcc -o main.c shell
chmod +x shell
./shell
```



#### 当前实现

* 外部指令调用
* 管道与重定向
* 操作符与参数相连时，自动分割操作符与参数
* 文件夹复制命令：`mycp <target> <dest>`
* 打印进程信息：`myps`
* 显示当前目录文件：`myls`
* 切换工作目录：`cd`，`cd`实现无参数时进入家目录



#### 待实现

* 递归分割操作符
* `shell`历史记录
* 显示文件夹结构`mytree`



#### 目录列表

```
├── LICENSE
├── main.c
├── mycd.c
├── mycp.c
├── myls.c
├── myps.c
├── README.md
├── shell
└── shell.h

0 directories, 9 files
```



#### 部分演示

```bash
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ pwd
/mnt/d/GitHub/MyShell

[xiabee @ DESKTOP-DOIHA8N MyShell]
$ myls -l
-rwxrwxrwx    1 xiabee   xiabee      16725 Jan 18 15:29 LICENSE
-rwxrwxrwx    1 xiabee   xiabee      14046 Jan 19 11:08 main.c
-rwxrwxrwx    1 xiabee   xiabee        915 Jan 19 11:07 mycd.c
-rwxrwxrwx    1 xiabee   xiabee       5261 Jan 19 08:32 mycp.c
-rwxrwxrwx    1 xiabee   xiabee       5058 Jan 19 08:46 myls.c
-rwxrwxrwx    1 xiabee   xiabee       5706 Jan 18 22:43 myps.c
-rwxrwxrwx    1 xiabee   xiabee       1323 Jan 19 11:09 README.md
-rwxrwxrwx    1 xiabee   xiabee      33200 Jan 19 11:08 shell
-rwxrwxrwx    1 xiabee   xiabee       1588 Jan 19 10:46 shell.h
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ myps
USER            PID     PPID    STATE   PNAME
root            1       0       S       (init)
root            5149    1       S       (init)
root            5150    5149    S       (init)
xiabee          5151    5150    S       (zsh)
xiabee          5438    5151    R       (shell)
[xiabee @ DESKTOP-DOIHA8N MyShell]
$

[xiabee @ DESKTOP-DOIHA8N MyShell]
$ help
XSLF bash, version 5.1.4(1)-release (x86_64-pc-linux-gnu)
These shell commands are defined internally.  Type 'help' to see this list.

pwd             : Show the current working directory.
cd              : Change working directory.
mycp            : Copy entire directory.
myps            : Display process information.
myls            : Display file information.
exit            : Exit the shell.

[xiabee @ DESKTOP-DOIHA8N MyShell]
$
```



