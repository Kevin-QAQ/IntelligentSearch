# 八数码难题

[English](https://github.com/Kevin-QAQ/IntelligentSearch/blob/master/8-puzzle-problem/README.md) | 简体中文

通过单步移动把下面的矩阵移动成 1\~8 环绕一周的矩阵（即 0 在中间，1\~8 顺序排成一圈，1 在哪儿无所谓）

$$
\begin{matrix}
2 & 8 & 3\\\\
1 & 0 & 4\\\\
5 & 6 & 7\\\\
\end{matrix}
$$

分别用广度优先搜索、深度优先搜索和启发式搜索算法 A* 算法进行搜索。

****
**Note**: 若无法显示 markdown 公式，请在 Chrome 浏览器中安装插件 MathJax Plugin for Github ([https://chrome.google.com/webstore/detail/mathjax-plugin-for-github/ioemnmodlmafdkllaclgeombjnmnbima/related](https://chrome.google.com/webstore/detail/mathjax-plugin-for-github/ioemnmodlmafdkllaclgeombjnmnbima/related))
****

# 要求

支持 C99 标准的 C 语言环境

# 如何使用本仓库

在当前目录运行以下命令：

首先将两个 .c 的源文件进行联合编译：

```
gcc list.c 8-puzzle-problem.c -o 8-puzzle-problem
```

然后运行编译链接后的可执行程序：

若是在 Windows 命令提示符中：

```
8-puzzle-problem
```

若是在 Windows PowerShell 中：

```
.\8-puzzle-problem
```

若是在 Linux terminal 中：

```
./8-puzzle-problem
```

# 程序说明

本程序采用 C 语言实现，分别采用 A\* 算法、广度优先搜索算法和深度优先搜索算法。这三种搜索算法的具体步骤可以参见[上一个说明文件](https://github.com/Kevin-QAQ/IntelligentSearch/blob/master/README_zh-cn.md)。需要注意的是，由于求解八数码难题的状态空间十分巨大（见后文），本程序实现的深度优先搜索算法的最大搜索深度被限制为事先由 A* 算法求得的最少步数。

## 定义八数码难题的结构体

我们用二维数组保存八数码矩阵，八数码结构体共包含八数码矩阵、在搜索树中的深度、估价函数值及指向父节点的指针。定义如下：

```c
typedef struct status
{
	char chessboard[3][3];
	int deep;
	int f;
	struct status * parent;
} Item;
```

## 本程序八数码难题的节点扩展算法

找到八数码矩阵中数字 0 的位置，判断数字 0 上下左右是否有数字可以移动到数字 0 处得到新的节点。代码中采用 expand() 函数实现。

## 判断八数码难题是否有解

由于在无解情况下，A\* 算法需要遍历 9!/2=181440 种情况，与广度优先搜索和深度优先搜索相同，所需要的时间和空间开销都较大，故采用计算排列的逆序数的方法来判断两种八数码状态是否可达：

**逆序数：**
> 在 n 个元素的任一排列中，当某两个元素的先后次序与标准次序不同时，就说有一个逆序。
> 一个排序中的所有逆序的总数叫做这个排列的逆序数。
> 设 P1 P2 …… Pn 为 n 个自然数的一个排列，
> 考虑元素 Pi (i=1,2,……,n)，如果比 Pi 大的且排在 Pi 前面的元素有 Ti 个，就说 Pi 这个元素的逆序数是 Ti，
> 全体元素的逆序数之总和即是这个排列的逆序数。

考虑八数码难题：将八数码展开成一维数组，计算除 0 以外所有数的逆序数之和，如果两个状态的逆序数奇偶性不同，则这两个状态不可达；反之，则可达。代码中采用 calculateReverseOrder() 函数实现。

## 估价函数/启发函数

本程序分别实现了两种启发函数：

* 启发函数一：W(n) 表示节点 n 中与目标状态相比“不在位”的数码个数；

* 启发函数二：W(n) 定义为每一个数码与其目标位置之间距离（不考虑夹在其间的数码）的总和。

在 8-puzzle-problem.c 中通过设置宏 METHOD 的值为 1 或 2 可以选择采用启发函数一或启发函数二，包含在 setF() 函数中。（使用启发函数二搜索速度远快于启发函数一）

## 本程序采用多文件编译

* list.h 包含结构体的定义及链表各操作的函数原型；
* list.c 包含链表各操作的函数实现；
* 8-puzzle-problem.c 包含 main() 函数、八数码难题的节点扩展算法等。

list.h 和 list.c 主要参考了（美）Stephen Prata 著 云巅工作室译 的书籍 《C Primer Plus（第五版）中文版》 人民邮电出版社

## 运行

本程序首先展示了一个示例，该示例即为题目所求。本程序不仅能输出最少步数的值，还能输出所有步数最少的路径的操作步骤，本题至少需要 16 步才能移动成 1~8 环绕一周的状态；本程序还允许用户自行输入初始状态和目标状态并设置了程序出口：当用户在初始状态中输入 q 时程序退出。**本程序将运行结果输出到命令行并同时保存在文件 output.txt 中。**
