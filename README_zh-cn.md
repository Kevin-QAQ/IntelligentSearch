# 说明文档

[English](https://github.com/Kevin-QAQ/IntelligentSearch/blob/master/README.md) | 简体中文

本仓库主要解决两道搜索题目：

* 8-puzzle-problem：八数码难题

* MCproblem：修道士（Missionaries）与野人（Cannibals）问题

两题均采用 C 语言实现，使用的搜索算法有 **A\* 算法**、**广度优先搜索算法**和**深度优先搜索算法**。

## 来源

此代码在GitHub上维护，链接如下：

[https://github.com/Kevin-QAQ/IntelligentSearch](https://github.com/Kevin-QAQ/IntelligentSearch)

## 搜索算法

所有搜索算法具体步骤都可归纳如下：

1. 把初始节点 S0 放入 Open 表中；
2. 检查 Open 表是否为空，若为空，则问题无解，失败退出；
3. 把 Open 表的第一个节点取出放入 Closed 表，并记该节点为节点 n；
4. 考察节点 n 是否为目标节点。若是则得到了问题的解，成功退出；
5. 若节点 n 不可扩展，则转第 2 步；
6. 扩展节点 n，生成一组子节点。遍历 Closed 表，按如下策略删去重复节点：
    * 重复节点删除策略：若子节点是重复节点 且 深度大于原节点，则删除；（注）
7. 将子节点插入到 Open 表中，三种搜索算法对应三种不同的插入策略：
	* 广度优先搜索：将子节点插入到 Open 表的尾部；
	* 深度优先搜索：将子节点插入到 Open 表的首部；
	* A* 算法：依据估价函数值寻找正确的插入位置，维护一个依估价函数值有序的 Open 表；
8. 转第 2 步。

注：之所以还需要保留相同深度的重复节点是为了输出所有步数最少的路径的操作步骤。

# 要求

支持 C99 标准的 C 语言环境
