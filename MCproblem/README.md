# 修道士(Missionaries)和野人(Cannibals)问题(简称 M-C 问题)
设在河的一岸有三个野人、三个修道士和一条船，修道士想用这条船把所有的人运到河对岸，但受以下条件的约束：  
1. 修道士和野人都会划船，但每次船上至多可载两个人；  
2. 在河的任一岸，如果野人数目超过修道士数，修道士会被野人吃掉。  

如果野人会服从任何一次过河安排，请规划一个确保修道士和野人都能过河，且没有修道士被野人吃掉的安全过河计划。

# Requirements
支持 C99 标准的 C 语言环境

# 程序说明
该程序采用 C 语言实现，采用全局择优搜索算法 A\* 算法，具体步骤见[上一个说明文件](https://github.com/KevinNum1/IntelligentSearch/blob/master/README.md)  

## MC 问题的结构体定义
MC 问题状态信息定义如下：**(左岸的修道士人数，左岸的野人人数，左岸的船只数目)**  
可以采用三元组 (m, c, b) 表示，其中，第三个量也可以直接理解为：当船在左岸时对应的值为 1 ，当船在右岸时对应的值为 0 。  

MC 问题结构体共包含 MC 状态信息、在搜索树中的深度、估价函数值及指向父节点的指针。  
具体定义如下：  
```c
typedef struct status
{
	int missionaries;
	int cannibals;
	char boat;
	int deep;
	int f;
	struct status * parent;
} Item;
```

## 本程序 MC 问题的节点扩展算法:  
若船从对岸运回本岸，则 MC 状态信息中人数就增加；  
若船从本岸运到对岸，则 MC 状态信息中人数就减少。  
且船上的人数需要满足以下三个条件：  
1. 船上修道士的人数非负且不超过起点岸的修道士人数，野人同理；  
2. 船上修道士和野人的总数不超过船的最大容量；  
3. 若船上至少有一名修道士，则野人人数不能超过修道士人数。

同时需要判断扩展出来的结点是否合法：  
**如果在任一岸都至少有一名修道士，则在任一岸修道士和野人人数都必须相等。**  

## 本程序 MC 问题的估价函数/启发函数：
估价函数/启发函数 f = deep + h  
其中 h = m + c - 2 * b  
其中，deep 表示节点的深度，(m, c, b) 为 MC 问题的状态信息，即 m 表示左岸的修道士人数，c 表示左岸的野人人数，b 表示左岸的船只数目。  

## 本程序采用多文件编译：  
* list.h 包含结构体的定义及链表各操作的函数原型；  
* list.c 包含链表各操作的函数实现；  
* MCproblem.c 包含 main() 函数、MC 问题的节点扩展算法等。  

list.h 和 list.c 主要参考了（美）Stephen Prata 著 云巅工作室译 的书籍 《C Primer Plus（第五版）中文版》 人民邮电出版社  

## 程序输出  

![MCoutput](https://github.com/KevinNum1/IntelligentSearch/blob/master/images/MCoutput.png)  

程序首先展示了一个示例，该示例即为题目所求。本程序不仅能输出最少步数的值，还能输出所有最少步数的路径的操作步骤。本程序还允许用户自行输入自定义的修道士总人数和船的最大载客量以求解不同的 MC 问题（野人总人数与修道士总人数相同），输入 q 时程序退出。**本程序将运行结果输出到命令行并同时保存在文件 output.txt 中。**  

示例一搜索图如下：  
![MCsteps](https://github.com/KevinNum1/IntelligentSearch/blob/master/images/MCsteps.png)  

以下是部分实验数据：（NUMBER表示修道士总人数，CAPACITY表示船的最大载客量）  
```
NUMBER = 10, CAPACITY = 4 时，有 361 个最短路径解，最少步数是 17 步；  
NUMBER = 10, CAPACITY = 3 时，无解，程序输出 No solution!  
NUMBER = 11, CAPACITY = 4 时，有 361 个最短路径解，最少步数是 19 步；  
NUMBER = 10, CAPACITY = 7 时，有 75 个最短路径解，最少步数是 7 步；  
NUMBER = 19, CAPACITY = 3 时，无解，程序输出 No solution!  
```
可以在输出文件 output.txt 中查看
