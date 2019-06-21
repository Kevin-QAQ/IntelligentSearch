#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define SIZE 16

#define ValueFunction 2
#if ValueFunction == 2
#define DIFF(A, B) ((A) > (B) ? (A) - (B) : (B) - (A))
#endif

int AstarCalculateSteps(Item state, Item target);
int AstarFindAllSolutions(Item state, Item target, int currentMethodNumber, FILE * fp);
void BFSearch(Item state, Item target, FILE * fp);
void DFSearch(Item state, Item target, int maxDepth, FILE * fp);
char calculateReverseOrder(Item * pItem);
char setF(Item * pItem, const Item * pTarget);
void update(Item * pItem, Item * father, const Item * pTarget, List * pOpenList, char method);
char count(const Item * pItem, const Item * ptarget);
void getRowAndColumn(const Item * pItem, char * pRow0, char * pColumn0, char index);
void display(const Item * pitem, FILE * fp);
void showItem(const Item * pItem, FILE * fp);
bool Expand(Item * pItem, List * pOpenList, const List * pCloseList, const Item * pTarget, char method);
bool input(Item * pItem);

int main(void) {
	Item target[SIZE] = { { { { 1, 2, 3 }, { 8, 0, 4 }, { 7, 6, 5 } }, 0, 0, NULL },
						  { { { 8, 1, 2 }, { 7, 0, 3 }, { 6, 5, 4 } }, 0, 0, NULL },
						  { { { 7, 8, 1 }, { 6, 0, 2 }, { 5, 4, 3 } }, 0, 0, NULL },
						  { { { 6, 7, 8 }, { 5, 0, 1 }, { 4, 3, 2 } }, 0, 0, NULL },
						  { { { 5, 6, 7 }, { 4, 0, 8 }, { 3, 2, 1 } }, 0, 0, NULL },
						  { { { 4, 5, 6 }, { 3, 0, 7 }, { 2, 1, 8 } }, 0, 0, NULL },
						  { { { 3, 4, 5 }, { 2, 0, 6 }, { 1, 8, 7 } }, 0, 0, NULL },
						  { { { 2, 3, 4 }, { 1, 0, 5 }, { 8, 7, 6 } }, 0, 0, NULL },
						  { { { 1, 8, 7 }, { 2, 0, 6 }, { 3, 4, 5 } }, 0, 0, NULL },
						  { { { 2, 1, 8 }, { 3, 0, 7 }, { 4, 5, 6 } }, 0, 0, NULL },
						  { { { 3, 2, 1 }, { 4, 0, 8 }, { 5, 6, 7 } }, 0, 0, NULL },
						  { { { 4, 3, 2 }, { 5, 0, 1 }, { 6, 7, 8 } }, 0, 0, NULL },
						  { { { 5, 4, 3 }, { 6, 0, 2 }, { 7, 8, 1 } }, 0, 0, NULL },
						  { { { 6, 5, 4 }, { 7, 0, 3 }, { 8, 1, 2 } }, 0, 0, NULL },
						  { { { 7, 6, 5 }, { 8, 0, 4 }, { 1, 2, 3 } }, 0, 0, NULL },
						  { { { 8, 7, 6 }, { 1, 0, 5 }, { 2, 3, 4 } }, 0, 0, NULL } };
	bool first;
	char minimalPath;
	int i, minimalSteps, currentMethodNumber;
	int steps[SIZE];

	FILE * fp;
	char * filename = "output.txt";
	if ((fp = fopen(filename, "w")) == NULL)
	{
		printf("Can't create output file \"%s\"!\n", filename);
		exit(EXIT_FAILURE);
	}

	puts("Look at an example first: \n");
	fputs("Look at an example first: \n\n", fp);
	Item state = { { { 2, 8, 3 }, { 1, 0, 4 }, { 5, 6, 7 } }, 0, 0, NULL };
	puts("Initial state: ");
	fputs("Initial state: \n", fp);
	showItem(&state, fp);

	first = false;
	minimalSteps = -1;
	for (i = 0; i < SIZE; i++)
	{
		steps[i] = AstarCalculateSteps(state, target[i]);
		if ((steps[i] >= 0) && (first == false))
		{
			minimalSteps = steps[i];
			minimalPath = i;
			first = true;
		}
		if ((first == true) && (steps[i] >= 0) && (steps[i] < minimalSteps))
		{
			minimalSteps = steps[i];
			minimalPath = i;
		}
	}
	if (minimalSteps >= 0)
	{
		printf("The minimal steps are: \t%d steps\n\n", minimalSteps);
		fprintf(fp, "The minimal steps are: \t%d steps\n\n", minimalSteps);

		puts("A* algorithm:\n");
		fputs("A* algorithm:\n\n", fp);

		currentMethodNumber = 1;
		for (i = 0; i < SIZE; i++)
			if (steps[i] == minimalSteps)
				currentMethodNumber = AstarFindAllSolutions(state, target[minimalPath], currentMethodNumber, fp);
		puts("That's all.\n");
		fputs("That's all.\n\n", fp);

		puts("Breadth-first search:\n");
		fputs("Breadth-first search:\n\n", fp);
		BFSearch(state, target[minimalPath], fp);

		puts("Depth-first search:\n");
		fputs("Depth-first search:\n\n", fp);
		DFSearch(state, target[minimalPath], minimalSteps, fp);
	}
	else
	{
		puts("\nNo solution!");
		fputs("\nNo solution!\n", fp);
	}

	puts("Please enter the initial state (exit when you enter q): ");
	putchar('\t');
	while (input(&state))
	{
		puts("Initial state: ");
		fputs("Initial state: \n", fp);
		showItem(&state, fp);

		first = false;
		minimalSteps = -1;
		for (i = 0; i < SIZE; i++)
		{
			steps[i] = AstarCalculateSteps(state, target[i]);
			if ((steps[i] >= 0) && (first == false))
			{
				minimalSteps = steps[i];
				minimalPath = i;
				first = true;
			}
			if ((first == true) && (steps[i] >= 0) && (steps[i] < minimalSteps))
			{
				minimalSteps = steps[i];
				minimalPath = i;
			}
		}
		if (minimalSteps >= 0)
		{
			printf("The minimal steps are: \t(%d steps)\n\n", minimalSteps);
			fprintf(fp, "The minimal steps are: \t(%d steps)\n\n", minimalSteps);

			puts("A* algorithm:\n");
			fputs("A* algorithm:\n\n", fp);
			currentMethodNumber = 1;
			for (i = 0; i < SIZE; i++)
				if (steps[i] == minimalSteps)
					currentMethodNumber = AstarFindAllSolutions(state, target[minimalPath], currentMethodNumber, fp);
			puts("That's all.\n");
			fputs("That's all.\n\n", fp);

			puts("Breadth-first search:\n");
			fputs("Breadth-first search:\n\n", fp);
			BFSearch(state, target[minimalPath], fp);

			puts("Depth-first search:\n");
			fputs("Depth-first search:\n\n", fp);
			DFSearch(state, target[minimalPath], minimalSteps, fp);
		}
		else
		{
			puts("\nNo solution!");
			fputs("\nNo solution!\n", fp);
		}

		puts("Please enter the initial state (exit when you enter q): ");
		putchar('\t');
	}

	puts("Bye!\n");
	fputs("Bye!\n", fp);
	if (fclose(fp) != 0)
		printf("Error in closing file %s!\n", filename);

	getchar();

	return 0;
}

int AstarCalculateSteps(Item state, Item target)
{
	int minimalSteps;
	if (calculateReverseOrder(&state) % 2 == calculateReverseOrder(&target) % 2)
	{
		List openList;
		List closeList;
		/*初始化*/
		InitializeList(&openList);
		if (ListIsFull(&openList))
		{
			fprintf(stderr, "No Memory available! Bye!\n\n");
			exit(EXIT_FAILURE);
		}
		InitializeList(&closeList);
		if (ListIsFull(&closeList))
		{
			fprintf(stderr, "No Memory available! Bye!\n\n");
			exit(EXIT_FAILURE);
		}

		if (AddItem(&state, &openList) == false)
			if (AddItem(&state, &openList) == false)
			{
				fprintf(stderr, "Problem allocating memory\n\n");
				exit(EXIT_FAILURE);
			}

		while (ListIsEmpty(&openList) == false)
		{
			if (headInserted(&(openList->item), &closeList) == false)
			{
				fprintf(stderr, "Problem allocating memory\n\n");
				exit(EXIT_FAILURE);
			}

			DeleteItem(&openList);

			if (count(&(closeList->item), &target) == 0)
			{
				minimalSteps = closeList->item.deep;
				break;
			}
			else
				Expand(&(closeList->item), &openList, &closeList, &target, Astar);
		}

		/* clean up         */
		EmptyTheList(&openList);
		EmptyTheList(&closeList);
	}
	else
		minimalSteps = -1;

	return minimalSteps;
}

int AstarFindAllSolutions(Item state, Item target, int currentMethodNumber, FILE * fp)
{
	List openList;
	List closeList;
	/*初始化*/
	InitializeList(&openList);
	if (ListIsFull(&openList))
	{
		fprintf(stderr, "No Memory available! Bye!\n\n");
		exit(EXIT_FAILURE);
	}
	InitializeList(&closeList);
	if (ListIsFull(&closeList))
	{
		fprintf(stderr, "No Memory available! Bye!\n\n");
		exit(EXIT_FAILURE);
	}

	if (AddItem(&state, &openList) == false)
		if (AddItem(&state, &openList) == false)
		{
			fprintf(stderr, "Problem allocating memory\n\n");
			exit(EXIT_FAILURE);
		}

	bool first = false;
	int minimalSteps = 0;

	while (ListIsEmpty(&openList) == false)
	{
		if (headInserted(&(openList->item), &closeList) == false)
		{
			fprintf(stderr, "Problem allocating memory\n\n");
			exit(EXIT_FAILURE);
		}

		DeleteItem(&openList);

		if ((first == false) || (closeList->item.deep <= minimalSteps))
		{
			if (count(&(closeList->item), &target) == 0)
			{
				if (first == false)
				{
					minimalSteps = closeList->item.deep;
					first = true;
				}
				fprintf(fp, "Method %d: \n", currentMethodNumber);
				printf("Method %d: \n", currentMethodNumber++);
				display(&(closeList->item), fp);
			}
			else
				Expand(&(closeList->item), &openList, &closeList, &target, Astar);
		}
		else
			continue;
	}

	/* clean up         */
	EmptyTheList(&openList);
	EmptyTheList(&closeList);

	return currentMethodNumber;
}

void BFSearch(Item state, Item target, FILE * fp)
{
	List openList;
	List closeList;
	/*初始化*/
	InitializeList(&openList);
	if (ListIsFull(&openList))
	{
		fprintf(stderr, "No Memory available! Bye!\n\n");
		exit(EXIT_FAILURE);
	}
	InitializeList(&closeList);
	if (ListIsFull(&closeList))
	{
		fprintf(stderr, "No Memory available! Bye!\n\n");
		exit(EXIT_FAILURE);
	}

	if (AddItem(&state, &openList) == false)
		if (AddItem(&state, &openList) == false)
		{
			fprintf(stderr, "Problem allocating memory\n\n");
			exit(EXIT_FAILURE);
		}

	while (ListIsEmpty(&openList) == false)
	{
		if (headInserted(&(openList->item), &closeList) == false)
		{
			fprintf(stderr, "Problem allocating memory\n\n");
			exit(EXIT_FAILURE);
		}

		DeleteItem(&openList);

		if (count(&(closeList->item), &target) == 0)
		{
			display(&(closeList->item), fp);
			break;
		}
		else
			Expand(&(closeList->item), &openList, &closeList, &target, BFS);
	}

	/* clean up         */
	EmptyTheList(&openList);
	EmptyTheList(&closeList);
}

void DFSearch(Item state, Item target, int maxDepth, FILE * fp)
{
	List openList;
	List closeList;
	/*初始化*/
	InitializeList(&openList);
	if (ListIsFull(&openList))
	{
		fprintf(stderr, "No Memory available! Bye!\n\n");
		exit(EXIT_FAILURE);
	}
	InitializeList(&closeList);
	if (ListIsFull(&closeList))
	{
		fprintf(stderr, "No Memory available! Bye!\n\n");
		exit(EXIT_FAILURE);
	}

	if (AddItem(&state, &openList) == false)
		if (AddItem(&state, &openList) == false)
		{
			fprintf(stderr, "Problem allocating memory\n\n");
			exit(EXIT_FAILURE);
		}

	while (ListIsEmpty(&openList) == false)
	{
		if (headInserted(&(openList->item), &closeList) == false)
		{
			fprintf(stderr, "Problem allocating memory\n\n");
			exit(EXIT_FAILURE);
		}

		DeleteItem(&openList);

		if (closeList->item.deep <= maxDepth)
		{
			if (count(&(closeList->item), &target) == 0)
			{
				display(&(closeList->item), fp);
				break;
			}
			else
				Expand(&(closeList->item), &openList, &closeList, &target, DFS);
		}
	}

	/* clean up         */
	EmptyTheList(&openList);
	EmptyTheList(&closeList);
}

bool Expand(Item * pItem, List * pOpenList, const List * pCloseList, const Item * pTarget, char method)
{
	bool boolFlag = false;
	char itemRow0, itemColumn0;
	getRowAndColumn(pItem, &itemRow0, &itemColumn0, 0);
	Item temp;
	if (itemRow0 - 1 >= 0)
	{
		temp = *pItem;
		temp.chessboard[itemRow0][itemColumn0] = temp.chessboard[itemRow0 - 1][itemColumn0];
		temp.chessboard[itemRow0 - 1][itemColumn0] = 0;
		temp.deep++;
		if (Traverse(pCloseList, &temp, count) == false)
		{
			boolFlag = true;
			update(&temp, pItem, pTarget, pOpenList, method);
		}
	}
	if (itemRow0 + 1 <= 2)
	{
		temp = *pItem;
		temp.chessboard[itemRow0][itemColumn0] = temp.chessboard[itemRow0 + 1][itemColumn0];
		temp.chessboard[itemRow0 + 1][itemColumn0] = 0;
		temp.deep++;
		if (Traverse(pCloseList, &temp, count) == false)
		{
			boolFlag = true;
			update(&temp, pItem, pTarget, pOpenList, method);
		}
	}
	if (itemColumn0 - 1 >= 0)
	{
		temp = *pItem;
		temp.chessboard[itemRow0][itemColumn0] = temp.chessboard[itemRow0][itemColumn0 - 1];
		temp.chessboard[itemRow0][itemColumn0 - 1] = 0;
		temp.deep++;
		if (Traverse(pCloseList, &temp, count) == false)
		{
			boolFlag = true;
			update(&temp, pItem, pTarget, pOpenList, method);
		}
	}
	if (itemColumn0 + 1 <= 2)
	{
		temp = *pItem;
		temp.chessboard[itemRow0][itemColumn0] = temp.chessboard[itemRow0][itemColumn0 + 1];
		temp.chessboard[itemRow0][itemColumn0 + 1] = 0;
		temp.deep++;
		if (Traverse(pCloseList, &temp, count) == false)
		{
			boolFlag = true;
			update(&temp, pItem, pTarget, pOpenList, method);
		}
	}
	return boolFlag;
}

char calculateReverseOrder(Item * pItem)	//计算逆序和
{
	char i, j, k, index, number, temp[8];
	k = number = 0;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (pItem->chessboard[i][j] != 0)
			{
				temp[k] = pItem->chessboard[i][j];
				for (index = 0; index < k; index++)
				{
					if (temp[index] > temp[k])
						number++;
				}
				k++;
			}
		}
	}
	return number;
}

void update(Item * pItem, Item * father, const Item * pTarget, List * pOpenList, char method)
{
	setF(pItem, pTarget);
	pItem->parent = father;

	if (method == Astar)
	{
		if (AddItem(pItem, pOpenList) == false)
		{
			fprintf(stderr, "Problem allocating memory\n\n");
			exit(EXIT_FAILURE);
		}
	}
	else if (method == BFS)
	{
		if (tailInserted(pItem, pOpenList) == false)
		{
			fprintf(stderr, "Problem allocating memory\n\n");
			exit(EXIT_FAILURE);
		}
	}
	else if (method == DFS)
	{
		if (headInserted(pItem, pOpenList) == false)
		{
			fprintf(stderr, "Problem allocating memory\n\n");
			exit(EXIT_FAILURE);
		}
	}
}

char setF(Item * pItem, const Item * pTarget)
{
	char itemRow0, itemColumn0, targetRow0, targetColumn0, w;
	getRowAndColumn(pItem, &itemRow0, &itemColumn0, 0);
	getRowAndColumn(pTarget, &targetRow0, &targetColumn0, 0);
#if ValueFunction == 1
	w = count(pItem, pTarget);
	if (itemRow0 != targetRow0 || itemColumn0 != targetColumn0)
		w--;
#elif ValueFunction == 2
	char i, j, row, column;
	w = 0;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (pItem->chessboard[i][j] != 0)
			{
				getRowAndColumn(pTarget, &row, &column, pItem->chessboard[i][j]);
				w += DIFF(row, i) + DIFF(column, j);
			}
		}
	}
#endif
	pItem->f = pItem->deep + w;
	return pItem->f;
}

char count(const Item * pItem, const Item * pTarget)
{
	char number = 0, i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (pItem->chessboard[i][j] != pTarget->chessboard[i][j])
				number++;
		}
	}
	return number;
}

void getRowAndColumn(const Item * pItem, char * pRow0, char * pColumn0, char index)
{
	char i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (pItem->chessboard[i][j] == index)
			{
				*pRow0 = i;
				*pColumn0 = j;
			}
		}
	}
}

void display(const Item * pItem, FILE * fp)
{
	static int step;

	if (pItem->parent == NULL)
	{
		step = 0;
		printf("\tStep %d: \n", step);
		fprintf(fp, "\tStep %d: \n", step);
		showItem(pItem, fp);
		return;
	}
	if (pItem->parent->parent == NULL)
	{
		step = 1;
		printf("\tStep %d: \n", step);
		fprintf(fp, "\tStep %d: \n", step);
		showItem(pItem, fp);
		return;
	}
	else
	{
		display(pItem->parent, fp);
		printf("\tStep %d: \n", ++step);
		fprintf(fp, "\tStep %d: \n", step);
		showItem(pItem, fp);
		return;
	}
}

void showItem(const Item * pItem, FILE * fp)
{
	char i, j;

	for (i = 0; i < 3; i++) // 输出到命令行
	{
		putchar('\t');
		putchar('\t');
		for (j = 0; j < 3; j++)
			printf(" %1d", pItem->chessboard[i][j]);
		putchar('\n');
	}
	putchar('\n');

	for (i = 0; i < 3; i++)	// 输出到文件
	{
		putc('\t', fp);
		putc('\t', fp);
		for (j = 0; j < 3; j++)
			fprintf(fp, " %1d", pItem->chessboard[i][j]);
		putc('\n', fp);
	}
	putc('\n', fp);
}

bool input(Item * pItem)
{
	char index = 0, i, j, k;
	int ch[9];
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			scanf("%d", &ch[index]);
			if (ch[index] >= 0 && ch[index] <= 8)
			{
				for (k = 0; k < index; k++)
				{
					if (ch[k] == ch[index])
					{
						while (getchar() != '\n')
							continue;
						return false;
					}
				}
				pItem->chessboard[i][j] = (char)ch[index];
				index++;
			}
			else
				return false;
		}
	}
	pItem->deep = 0;
	pItem->f = 0;
	pItem->parent = NULL;

	return true;
}
