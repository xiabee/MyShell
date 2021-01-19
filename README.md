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
* 测试进程运行时间：`mytime`
* 显示目录结构：`mytree`



#### 待实现

* 递归分割操作符
* `shell`历史记录
* 删除操作



#### 目录列表

```
.
├── LICENSE
├── main.c
├── mycd.c
├── mycp.c
├── myls.c
├── myps.c
├── mytime.c
├── README.md
├── shell
└── shell.h

0 directories, 10 files
```



#### 部分演示

```bash
[xiabee @ DESKTOP-DOIHA8N src]
$ ls
bin  LICENSE  main.c  mycd.c  mycp.c  myls.c  myps.c  mytime.c  mytree.c  README.md  shell  shell.h
[xiabee @ DESKTOP-DOIHA8N src]
$ myps
USER            PID     PPID    STATE   PNAME
root            1       0       S       (init)
root            7       1       S       (init)
root            8       7       S       (init)
xiabee          9       8       S       (zsh)
root            178     1       S       (init)
root            179     178     S       (init)
xiabee          180     179     S       (sh)
xiabee          181     180     S       (sh)
xiabee          186     181     S       (sh)
xiabee          188     186     S       (node)
xiabee          229     188     S       (node)
xiabee          242     188     S       (node)
xiabee          264     242     S       (cpptools)
xiabee          2890    264     S       (cpptools-srv)
xiabee          9617    9       R       (shell)
[xiabee @ DESKTOP-DOIHA8N src]
$ mytree .
.
├── .
├── ..
├── bin
│   ├── .
│   ├── ..
│   ├── sss file
│   ├── sss.c file
│   ├── strtok file
│   ├── strtok.c file
│   ├── test file
│   └── test.c file
├── LICENSE file
├── main.c file
├── mycd.c file
├── mycp.c file
├── myls.c file
├── myps.c file
├── mytime.c file
├── mytree.c file
├── README.md file
├── shell file
└── shell.h file
[xiabee @ DESKTOP-DOIHA8N src]
$ test
Command Error!
You may need 'help'

[xiabee @ DESKTOP-DOIHA8N src]
$ help
XSLF bash, version 5.1.4(1)-release (x86_64-pc-linux-gnu)
These shell commands are defined internally.  Type 'help' to see this list.

pwd             : Show the current working directory.
cd              : Change working directory.
mycp            : Copy entire directory.
myps            : Display process information.
myls            : Display file information.
mytime          :Measure the running time of the process or control the specified running time of the process
mytree          :Show the structure of the directory
exit            : Exit the shell.
```



