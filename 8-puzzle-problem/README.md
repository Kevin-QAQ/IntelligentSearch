# Eight-Puzzle Problem

Move the following matrix into a matrix of 1\~8 surrounding a circle (that is, 0 is in the center, 1\~8 are arranged in a circle, and it doesn't matter where the 1 is) by single-step movement

$$
\begin{matrix}
2 & 8 & 3\\\\
1 & 0 & 4\\\\
5 & 6 & 7\\\\
\end{matrix}
$$

Search with Breadth First Search, Depth First Search and Heuristic Search Algorithm A* Algorithm respectively.

****
**Note**: If the markdown formula cannot be displayed, please install the plugin MathJax Plugin for Github in Chrome browser ([https://chrome.google.com/webstore/detail/mathjax-plugin-for-github/ioemnmodlmafdkllaclgeombjnmnbima/ related](https://chrome.google.com/webstore/detail/mathjax-plugin-for-github/ioemnmodlmafdkllaclgeombjnmnbima/related))
****

# Requirements

C locale that supports the C99 standard

# How to use this repository?

Run the following command in the current directory:

First, jointly compile the two .c source files:

````
gcc list.c 8-puzzle-problem.c -o 8-puzzle-problem
````

Then run the compiled and linked executable:

If in a Windows command prompt:

````
8-puzzle-problem
````

If in Windows PowerShell:

````
.\8-puzzle-problem
````

If in Linux terminal:

````
./8-puzzle-problem
````

# Program Description

This program is implemented in C language, using A\* algorithm, Breadth-First Search algorithm and Depth-First Search algorithm respectively. The specific steps of these three search algorithms can be found in the [previous document](https://github.com/Kevin-QAQ/IntelligentSearch/blob/master/README.md). It should be noted that due to the huge state space for solving the Eight-Puzzle Problem (see below), the maximum search depth of the Depth-First Search algorithm implemented by this program is limited to the minimum number of steps obtained by the A* algorithm in advance.

## Define the structure of the Eight-Puzzle Problem

We use a two-dimensional array to store the matrix. The structure contains a total of matrix, the depth in the search tree, the evaluation function value, and the pointer to the parent node. Defined as follows:

````c
typedef struct status
{
char chessboard[3][3];
int deep;
int f;
struct status * parent;
} Item;
````

## The node expansion algorithm of this program

Find the position of the number 0 in the matrix, and judge whether there are numbers around the number 0 that can be moved to the number 0 to get a new node. The code is implemented using the function expand().

## Determine whether the Eight-Puzzle Problem has a solution

Since in the case of no solution, the A\* algorithm needs to traverse 9!/2=181440 cases, which is the same as the Breadth-First Search and Depth-First Search, and the time and space overheads required are large, so the method of calculating the reverse order number is used to judge whether two states are reachable:

**Reverse Order Number:**
> In any permutation of n elements, a reverse order is said to exist when the order of two elements differs from the standard order.
> The total number of all inversions in an ordering is called the reverse order number of this ordering.
> Let P1 P2 ...... Pn be a permutation of n natural numbers,
> Considering the element Pi (i=1,2,...,n), if there are Ti elements larger than Pi and preceded by Pi, we say that the reverse order number of the element Pi is Ti,
> The sum of the reverse order numbers of all elements is the reverse order number of this arrangement.

Consider the Eight-Puzzle Problem: expand the matrix into a one-dimensional array, and calculate the sum of the reverse order numbers of all elements except 0. If the parity of the reverse order numbers of the two states is different, the two states are unreachable; otherwise, they are reachable. The code is implemented using the function calculateReverseOrder().

## Evaluation function/Heuristic function

This program implements two heuristic functions:

* Heuristic function 1: W(n) represents the number of digits in node n that are "not in place" compared with the target state;

* Heuristic function 2: W(n) is defined as the sum of the distance between each digit and its target position (regardless of the digits sandwiched in between).

In 8-puzzle-problem.c, you can choose to use heuristic function 1 or heuristic function 2 by setting the value of macro METHOD to 1 or 2, which is included in the function setF(). (Using heuristic function 2 is much faster than heuristic function 1)

## This program is compiled with multiple files

* list.h contains the definition of the structure and the function prototype of each operation of the linked list;
* list.c contains the function implementation of each operation of the linked list;
* 8-puzzle-problem.c contains the function main(), the node expansion algorithm, etc.

list.h and list.c mainly refer to Stephen Prata's book "C Primer Plus (Fifth Edition)"

## Run

The program first shows an example, which is what the title asks for. This program can not only output the value of the minimum number of steps, but also output the operation steps of all paths with the minimum number of steps. This problem requires at least 16 steps to move into a state of 1~8 circles; this program also allows the user to input the initial state and target state and the program exits when the user enters q in the initial state. **This program outputs the running result to the command line and saves it in the file output.txt at the same time. **
