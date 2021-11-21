# MyShell

## 项目简介

* 功能：手动实现`Linux`的`shell`解释器

* 演示视频：[https://www.bilibili.com/video/BV1Pq4y1g7w4/](https://www.bilibili.com/video/BV1Pq4y1g7w4/)



## 编译运行

```bash
gitclone https://github.com/xiabee/MyShell
cd ./MyShell
gcc -o MyShell shell.c
./MyShell
```



## 功能实现

* 外部指令调用
* 管道与重定向
* 操作符与参数相连时，自动分割操作符与参数
* 命令提示：`help`
* 显示工作目录：`pwd`
* 显示进程信息：`myps`
* 显示当前目录文件：`myls`
* 切换工作目录：`cd`
* 显示进程运行时间：`mytime`
* 显示目录结构：`mytree`
* 显示输入历史：`myhis`
* 自定义复制文件、文件夹：`mycp `
* 自定义移动文件、文件夹：`mymv`
* 自定义删除文件、文件夹：`myrm`
* 显示文件、文件夹内全部文件的函数：`myline`



## 部分演示

```bash
# xiabee @ DESKTOP-DOIHA8N in /mnt/d/GitHub/MyShell on git:main x [17:35:52]
$ ./shell
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ pwd
/mnt/d/GitHub/MyShell

[xiabee @ DESKTOP-DOIHA8N MyShell]
$ ls
LICENSE  mycp.c    myls.c  myps.c  mytime.c  README.md  shell.c
mycd.c   myline.c  mymv.c  myrm.c  mytree.c  shell      shell.h
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ myps
USER            PID     PPID    STATE   PNAME
root            1       0       S       (init)
root            765     1       S       (init)
root            766     765     S       (init)
xiabee          767     766     S       (sh)
xiabee          768     767     S       (sh)
xiabee          773     768     S       (sh)

[xiabee @ DESKTOP-DOIHA8N MyShell]
$ cd

[xiabee @ DESKTOP-DOIHA8N xiabee]
$ cd /mnt/d/GitHub/MyShell

[xiabee @ DESKTOP-DOIHA8N MyShell]
$ mytime pwd
/mnt/d/GitHub/MyShell
running time: 1.583000 ms
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ mytree . 2
.
├── .
├── ..
├── .git
│   ├── .
│   ├── FETCH_HEAD file
│   ├── HEAD file
| ......
└── shell.h file

[xiabee @ DESKTOP-DOIHA8N MyShell]
$ myline shell.c
shell.c         626

total lines:    626

[xiabee @ DESKTOP-DOIHA8N MyShell]
$ myhis
-------------------------------------
**  Print Input History until now: **
pwd
ls
myps
cd
cd /mnt/d/GitHub/MyShell
mytime pwd
mytree . 2
myline shell.c
myhis
-------------------------------------
[xiabee @ DESKTOP-DOIHA8N MyShell]
$ exit
Bye~

# xiabee @ DESKTOP-DOIHA8N in /mnt/d/GitHub/MyShell on git:main x [17:37:31]
$
```



## 暂未实现

* `makefile`编写（太鸽了忘记了）
* 特殊符号处理，例如方向键（太鸽了忘记了）
