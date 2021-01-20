# MyShell



#### 项目简介

* 功能：手动实现`Linux`的`shell`解释器



#### 编译运行

```
gitclone https://github.com/xiabee/MyShell
cd ./MyShell
gcc -o shell shell.c
chmod +x shell
./shell
```



#### 当前实现

* 外部指令调用
* 管道与重定向
* 操作符与参数相连时，自动分割操作符与参数
* 命令提示：`help`
* 文件夹复制命令：`mycp <target> <dest>`
* 显示进程信息：`myps`
* 显示当前目录文件：`myls`
* 切换工作目录：`cd`，`cd`实现无参数时进入家目录
* 显示进程运行时间：`mytime`
* 显示目录结构：`mytree`
* 显示输入历史：`history`
* 自定义移动：`mymv`
* 自定义删除（支持直接删除文件夹）：`myrm`
* 显示文件/ 文件夹内全部文件的函数：`myline`







#### 部分演示

```bash
# xiabee @ DESKTOP-DOIHA8N in /mnt/d/GitHub/MyShell on git:main x [23:51:21]
$ ./shell
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ ls
LICENSE  mycd.c  mycp.c  myls.c  myps.c  mytime.c  mytree.c  README.md  shell  shell.c  shell.h
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ pwd
/mnt/d/GitHub/MyShell

[xiabee @ DESKTOP-DOIHA8N MyShell]
$ myps
USER            PID     PPID    STATE   PNAME
root            1       0       S       (init)
root            9767    1       S       (init)
root            9768    9767    R       (init)
xiabee          9769    9768    S       (zsh)
root            10241   1       S       (init)
root            10242   10241   S       (init)
xiabee          10243   10242   S       (sh)
xiabee          10244   10243   S       (sh)
xiabee          10249   10244   S       (sh)
xiabee          10251   10249   S       (node)
xiabee          10287   10251   S       (node)
xiabee          10309   10251   S       (node)
xiabee          10326   10309   S       (cpptools)
xiabee          11652   10326   S       (cpptools-srv)
xiabee          13462   9769    R       (shell)
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ cd .

[xiabee @ DESKTOP-DOIHA8N MyShell]
$ mycp
Invalid arguments.
Usage: mycp <path> <path>
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ mytree
Parameter error!
Usage1: mytree <path>
Usage2: mytree <path> <depth>

[xiabee @ DESKTOP-DOIHA8N MyShell]
$ mytime
Please input the process name!
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ history
-------------------------------------
**  Print Input History until now: **
ls
pwd
myps
cd .
mycp
mytree
mytime
history
-------------------------------------
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ exit
Bye~
```



