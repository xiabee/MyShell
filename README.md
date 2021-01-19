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



#### 待实现

* 递归分割操作符
* `shell`历史记录
* 显示文件夹结构`mytree`



#### 目录列表

```
.
├── LICENSE
├── main.c
├── mycp.c
├── README.md
├── shell
└── shell.h

0 directories, 6 files
```



#### 部分演示

```
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
```



