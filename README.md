# MyShell



#### 项目简介

* 作者：xiabee

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



#### 待实现

* 递归分割操作符
* shell历史记录



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

