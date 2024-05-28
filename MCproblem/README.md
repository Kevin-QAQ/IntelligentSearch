# Missionaries and Cannibals Problem (M-C Problem for short)

English | [简体中文](https://github.com/Kevin-QAQ/IntelligentSearch/blob/master/MCproblem/README_zh-cn.md)

On one bank of the river there are three cannibals, three missionaries, and a boat, and the missionaries want to use this boat to transport all the people to the other side of the river, subject to the following conditions:

1. Both missionaries and cannibals can row, but at most two people can be carried on each boat;

2. On either bank of the river, if the number of cannibals exceeds the number of missionaries, the missionaries will be eaten by cannibals.

If cannibals will obey any river crossing arrangements, please plan a safe crossing plan that ensures both missionaries and cannibals can cross the river, and that no missionaries are eaten by cannibals.

# Requirements

C locale that supports the C99 standard

****
**Note**: If the markdown formula cannot be displayed, please install the plugin MathJax Plugin for Github in Chrome browser ([https://chrome.google.com/webstore/detail/mathjax-plugin-for-github/ioemnmodlmafdkllaclgeombjnmnbima/ related](https://chrome.google.com/webstore/detail/mathjax-plugin-for-github/ioemnmodlmafdkllaclgeombjnmnbima/related))
****

# How to use this repository?

Run the following command in the current directory:

First, jointly compile the two .c source files:

````
gcc list.c MCproblem.c -o MCproblem
````

Then run the compiled and linked executable:

If in a Windows command prompt:

````
MCproblem
````

If in Windows PowerShell:

````
.\MCproblem
````

If in Linux terminal:

````
./MCproblem
````

# Program Description

The program is implemented in C language and adopts the global optimal search algorithm A\* algorithm. For the specific steps, see the [previous document](https://github.com/Kevin-QAQ/IntelligentSearch/blob/master/README.md)

## Structure definition of M-C Problem

The M-C Problem state information is defined as follows: **(Number of missionaries on the left bank, number of cannibals on the left bank, number of ships on the left bank)**
It can be represented by a triple $(m, c, b)$, where the third quantity can also be directly understood as: when the ship is on the left bank, the corresponding value is 1 , when the ship is on the right bank, the corresponding value is 0 .

The M-C Problem structure contains the M-C state information, the depth in the search tree, the evaluation function value and the pointer to the parent node.
The specific definitions are as follows:

````c
typedef struct status
{
int missionaries;
int cannibals;
char boat;
int deep;
int f;
struct status * parent;
} Item;
````

## The node expansion algorithm of this program:

If the ship is transported from the opposite bank to the local bank, the number of people in the M-C status information will increase;
If the ship is transported from the local shore to the opposite shore, the number of people in the M-C status information will be reduced.
The number of people on board must meet the following three conditions:

1. The number of missionaries on board is not negative and does not exceed the number of missionaries on the starting bank, and the same is true for cannibals;

2. The total number of missionaries and cannibals on board does not exceed the maximum capacity of the ship;

3. If there is at least one missionary on board, the number of cannibals cannot exceed the number of missionaries.

At the same time, it is necessary to judge whether the extended node is legal:

**If there is at least one missionary on either bank, the number of missionaries and cannibals must be equal on either bank. **

## The evaluation function/Heuristic function of this program:

$$f=d+h$$

where $h=m+c-2\times b$

where, $d$ represents the depth of the node, $(m, c, b)$ is the state information of the M-C Problem, that is, $m$ represents the number of missionaries on the left bank, $c$ represents the number of cannibals on the left bank, and $b$ represents the number of ships on the left bank.

## This program is compiled with multiple files:

* list.h contains the definition of the structure and the function prototype of each operation of the linked list;

* list.c contains the function implementation of each operation of the linked list;

* MCproblem.c contains the function main(), the node expansion algorithm for M-C Problems, etc.

list.h and list.c mainly refer to Stephen Prata's book "C Primer Plus (Fifth Edition)"

## Program Output

<div align=center>
<img src="https://github.com/Kevin-QAQ/IntelligentSearch/blob/master/images/MCoutput.png"/>
</div>

The program first shows an example, which is what the title asks for. This program can not only output the value of the minimum number of steps, but also output the operation steps of all paths with the minimum number of steps. This program also allows the user to enter the custom total number of missionaries and the maximum passenger capacity of the ship to solve different M-C Problems (the total number of cannibals is the same as the total number of missionaries), and the program exits when q is input. **This program outputs the running result to the command line and saves it in the file output.txt at the same time. **

Example 1 search graph is as follows:

<div align=center>
<img src="https://github.com/Kevin-QAQ/IntelligentSearch/blob/master/images/MCsteps.png"/>
</div>

The following are some experimental data: (NUMBER represents the total number of missionaries, CAPACITY represents the maximum passenger capacity of the ship)

````
When NUMBER = 10, CAPACITY = 4, there are 361 shortest path solutions, and the minimum number of steps is 17;
When NUMBER = 10, CAPACITY = 3, there is no solution, and the program outputs No solution!
When NUMBER = 11, CAPACITY = 4, there are 361 shortest path solutions, and the minimum number of steps is 19;
When NUMBER = 10, CAPACITY = 7, there are 75 shortest path solutions, and the minimum number of steps is 7;
When NUMBER = 19, CAPACITY = 3, there is no solution, and the program outputs No solution!
````

These can be viewed in the output file output.txt.
