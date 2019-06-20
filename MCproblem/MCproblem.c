#include <stdio.h>
#include <stdlib.h>

#include "list.h"

extern int NUMBER;
extern int CAPACITY;

void solveMC(Item state);

int main(void)
{
	puts("Example: \n");
	Item state0 = { NUMBER, NUMBER, 1, 0, 0, NULL };
	setF(&state0);
	solveMC(state0);

	puts("\nPlease enter the number of missionaries and cannibals(exit when you enter q): ");
	putchar('\t');
	while (scanf_s("%d", &NUMBER) == 1)
	{
		puts("Please enter the capacity of the ship: ");
		putchar('\t');
		scanf_s("%d", &CAPACITY);
		Item state = { NUMBER, NUMBER, 1, 0, 0, NULL };
		putchar('\n');
		solveMC(state);
		puts("\nPlease enter the number of missionaries and cannibals(exit when you enter q): ");
		putchar('\t');
	}

	puts("\nBye!\n");

	getchar();
	getchar();
	getchar();

	return 0;
}

void solveMC(Item state)
{
	printf("Initial state: ");
	showItem(&state);
	printf("The capacity of the ship is %d.\n", CAPACITY);
	List openList;
	List closeList;

	/* 初始化 */
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
	{
		fprintf(stderr, "Problem allocating memory\n\n");
		exit(EXIT_FAILURE);
	}

	int methodNumber = 1;
	int minimalSteps = 0;

	while (ListIsEmpty(&openList) == false)
	{
		if (headInserted(&(openList->item), &closeList) == false)
		{
			fprintf(stderr, "Problem allocating memory\n\n");
			exit(EXIT_FAILURE);
		}

		DeleteItem(&openList);

		if((methodNumber == 1) || (closeList->item.deep <= minimalSteps))
		{
			// 到达目标状态了——全员都在河对岸
			if ((closeList->item.missionaries == 0) && (closeList->item.cannibals == 0) && (closeList->item.boat == 0))
			{
				if (methodNumber == 1)
				{
					minimalSteps = closeList->item.deep;
					printf("\nThe minimal steps are：\t%d steps.\n", minimalSteps);
				}
				printf("\nMethod %d：\n", methodNumber++);
				display(&(closeList->item));
			}
			else
				Expand(&(closeList->item), &openList, &closeList);
		}
		else
			continue;
	}
	// 遍历完所有可达状态之后没有目标状态，即无法在此约束条件下将修道士和野人全部安全运过河
	if (!minimalSteps)
		puts("\nNo solution!");

	puts("\nThat's all, thank you!");

	/* clean up         */
	EmptyTheList(&openList);
	EmptyTheList(&closeList);
}
