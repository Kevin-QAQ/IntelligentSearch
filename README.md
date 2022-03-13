#Kevin-QAQ/IntelligentSearch

This repository mainly solves two search problems:

* 8-puzzle-problem: Eight-Puzzle Problem

* MCproblem: Missionaries and Cannibals Problem

Both questions are implemented in C language, and the search algorithms used are **A\* algorithm**, **Breadth-First Search algorithm** and **Depth-First Search algorithm**.

## Search Algorithm

The specific steps of all search algorithms can be summarized as follows:

1. Put the initial node S0 into the list Open;
2. Check whether the list Open is empty, if it is empty, the problem is unsolved, and exit;
3. Take the first node of the list Open and put it into the list Closed, and record the node as node n;
4. Check whether node n is the target node. If so, the solution to the problem has been found and exit successfully;
5. If node n is not scalable, go to step 2;
6. Expand node n to generate a set of child nodes. Traverse the list Closed and delete duplicate nodes according to the following strategy:
    * Duplicate node deletion strategy: if the child node is a duplicate node and its depth is greater than the original node, delete it; (Note)
7. Insert child nodes into the list Open. Three search algorithms correspond to three different insertion strategies:
* Breadth-First Search: insert child nodes at the end of the list Open;
* Depth-First Search: insert child nodes into the header of the list Open;
* A* algorithm: find the correct insertion position according to the value of the evaluation function, and maintain an ordered list Open by the value of the evaluation function;
8. Go to step 2.

Note: The reason why it is necessary to keep repeating nodes of the same depth is to output the operation steps of all paths with the fewest steps.

# Requirements

C locale that supports the C99 standard
