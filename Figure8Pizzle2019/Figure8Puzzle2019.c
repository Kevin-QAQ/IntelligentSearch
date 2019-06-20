#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define SIZE 16

int AstarCalculateSteps(Item state, Item target);
int AstarFindAllSolutions(Item state, Item target, int currentMethodNumber);
void BFSearch(Item state, Item target);
void DFSearch(Item state, Item target, int maxDepth);
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

	puts("Look at an example first: \n");
	puts("Example：\n");
	Item state = { { { 2, 8, 3 }, { 1, 0, 4 }, { 5, 6, 7 } }, 0, 0, NULL };
	puts("Initial state: ");
	showItem(&state);

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

		puts("A* algorithm:\n");
		currentMethodNumber = 1;
		for (i = 0; i < SIZE; i++)
			if (steps[i] == minimalSteps)
				currentMethodNumber = AstarFindAllSolutions(state, target[minimalPath], currentMethodNumber);
		puts("That's all.\n");

		puts("Breadth-first search:\n");
		BFSearch(state, target[minimalPath]);

		puts("Depth-first search:\n");
		DFSearch(state, target[minimalPath], minimalSteps);
	}
	else
		puts("\nNo solution!");

	puts("Please enter the initial state (exit when you enter q): ");
	putchar('\t');
	while (input(&state))
	{
		puts("Initial state: ");
		showItem(&state);

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

			puts("A* algorithm:\n");
			currentMethodNumber = 1;
			for (i = 0; i < SIZE; i++)
				if (steps[i] == minimalSteps)
					currentMethodNumber = AstarFindAllSolutions(state, target[minimalPath], currentMethodNumber);
			puts("That's all.\n");

			puts("Breadth-first search:\n");
			BFSearch(state, target[minimalPath]);

			puts("Depth-first search:\n");
			DFSearch(state, target[minimalPath], minimalSteps);
		}
		else
			puts("\nNo solution!");

		puts("Please enter the initial state (exit when you enter q): ");
		putchar('\t');
	}

	puts("Thank you!\n");

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

int AstarFindAllSolutions(Item state, Item target, int currentMethodNumber)
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
				printf("Method %d：\n", currentMethodNumber++);
				display(&(closeList->item));
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

void BFSearch(Item state, Item target)
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
			display(&(closeList->item));
			break;
		}
		else
			Expand(&(closeList->item), &openList, &closeList, &target, BFS);
	}

	/* clean up         */
	EmptyTheList(&openList);
	EmptyTheList(&closeList);
}

void DFSearch(Item state, Item target, int maxDepth)
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
				display(&(closeList->item));
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

bool input(Item * pItem)
{
	char index = 0, i, j, k;
	int ch[9];
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			scanf_s("%d", &ch[index]);
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
