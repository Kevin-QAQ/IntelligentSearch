#include <stdio.h>
#include <stdlib.h>

#include "list.h"

#define SMALLER(X, Y) ((X) > (Y) ? (Y) : (X))
#define MIN(X, Y, Z) SMALLER(X, SMALLER(Y, Z))

extern int NUMBER;
extern int CAPACITY;

void solveMC(Item state, FILE * fp);
bool Expand(Item * pItem, List * pOpenList, const List * pCloseList);
bool isLegal(const Item * pItem);
void update(Item * pItem, List * pOpenList);
char setF(Item * pItem);
void display(const Item * pItem, FILE * fp);
void showItem(const Item * pItem, FILE * fp);

int main(void)
{
	FILE * fp;
	char * filename = "output.txt";
	if ((fp = fopen(filename, "w")) == NULL)
	{
		printf("Can't create output file \"%s\"!\n", filename);
		exit(EXIT_FAILURE);
	}

	puts("Look at an example first: \n");
	fputs("Look at an example first: \n\n", fp);
	Item state0 = { NUMBER, NUMBER, 1, 0, 0, NULL };
	setF(&state0);
	solveMC(state0, fp);

	puts("Please enter the number of missionaries and cannibals(exit when you enter q): ");
	putchar('\t');
	while (scanf("%d", &NUMBER) == 1)
	{
		puts("Please enter the capacity of the ship: ");
		putchar('\t');
		scanf("%d", &CAPACITY);
		Item state = { NUMBER, NUMBER, 1, 0, 0, NULL };
		putchar('\n');
		solveMC(state, fp);
		puts("Please enter the number of missionaries and cannibals(exit when you enter q): ");
		putchar('\t');
	}

	puts("\nBye!\n");
	if (fclose(fp) != 0)
		printf("Error in closing file %s!\n", filename);

	getchar();
	getchar();
	getchar();

	return 0;
}

void solveMC(Item state, FILE * fp)
{
	printf("Initial state: ");
	fputs("Initial state: ", fp);
	showItem(&state, fp);
	printf("The capacity of the ship is %d.\n", CAPACITY);
	fprintf(fp, "The capacity of the ship is %d.\n", CAPACITY);
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
					printf("\nThe minimal steps are: \t%d steps.\n", minimalSteps);
					fprintf(fp, "\nThe minimal steps are: \t%d steps.\n", minimalSteps);
				}
				fprintf(fp, "\nMethod %d: \n", methodNumber);
				printf("\nMethod %d: \n", methodNumber++);
				display(&(closeList->item), fp);
			}
			else
				Expand(&(closeList->item), &openList, &closeList);
		}
		else
			continue;
	}
	// 遍历完所有可达状态之后没有目标状态，即无法在此约束条件下将修道士和野人全部安全运过河
	if (!minimalSteps)
	{
		puts("\nNo solution!");
		fputs("\nNo solution!\n", fp);
	}

	puts("\nThat's all, thank you!\n");
	fputs("\nThat's all, thank you!\n\n", fp);

	/* clean up         */
	EmptyTheList(&openList);
	EmptyTheList(&closeList);
}

// 从当前结点扩展出合法的新结点
bool Expand(Item * pItem, List * pOpenList, const List * pCloseList)
{
	bool boolFlag = false;
	int deltaM, deltaC;
	Item temp;

	if (pItem->boat == 0)	// 船在河对岸，需要从对岸运到本岸
	{
		// 船上至少一个修道士
		for (deltaM = 1; deltaM <= SMALLER(NUMBER - pItem->missionaries, CAPACITY); deltaM++)
		{
			for (deltaC = 0; deltaC <= MIN(deltaM, NUMBER - pItem->cannibals, CAPACITY - deltaM); deltaC++)
			{
				temp = *pItem;
				temp.missionaries += deltaM;
				temp.cannibals += deltaC;
				temp.boat = 1;
				temp.deep++;
				if (isLegal(&temp) && !Traverse(pCloseList, &temp))
				{
					temp.parent = pItem;
					boolFlag = true;
					update(&temp, pOpenList);
				}
			}
		}

		// 船上没有修道士
		deltaM = 0;
		for (deltaC = 1; deltaC <= SMALLER(NUMBER - pItem->cannibals, CAPACITY); deltaC++)
		{
			temp = *pItem;
			temp.cannibals += deltaC;
			temp.boat = 1;
			temp.deep++;
			if (isLegal(&temp) && !Traverse(pCloseList, &temp))
			{
				temp.parent = pItem;
				boolFlag = true;
				update(&temp, pOpenList);
			}
		}
	}
	else	// 船在本岸，需要运到河对岸
	{
		// 船上至少一个修道士
		for (deltaM = 1; deltaM <= SMALLER(pItem->missionaries, CAPACITY); deltaM++)
		{
			for (deltaC = 0; deltaC <= MIN(deltaM, pItem->cannibals, CAPACITY - deltaM); deltaC++)
			{
				temp = *pItem;
				temp.missionaries -= deltaM;
				temp.cannibals -= deltaC;
				temp.boat = 0;
				temp.deep++;
				if (isLegal(&temp) && !Traverse(pCloseList, &temp))
				{
					temp.parent = pItem;
					boolFlag = true;
					update(&temp, pOpenList);
				}
			}
		}

		// 船上没有修道士
		deltaM = 0;
		for (deltaC = 1; deltaC <= SMALLER(pItem->cannibals, CAPACITY); deltaC++)
		{
			temp = *pItem;
			temp.cannibals -= deltaC;
			temp.boat = 0;
			temp.deep++;
			if (isLegal(&temp) && !Traverse(pCloseList, &temp))
			{
				temp.parent = pItem;
				boolFlag = true;
				update(&temp, pOpenList);
			}
		}
	}

	return boolFlag;
}

bool isLegal(const Item * pItem)	// 判断结点状态是否合法
{
	if ((pItem->missionaries != 0) && (pItem->missionaries != NUMBER))
	{
		if (pItem->missionaries == pItem->cannibals)
			return true;
		else
			return false;
	}
	else
		return true;
}

void update(Item * pItem, List * pOpenList)		// 更新链表
{
	setF(pItem);
	if (AddItem(pItem, pOpenList) == false)
	{
		fprintf(stderr, "Problem allocating memory\n\n");
		exit(EXIT_FAILURE);
	}
}

char setF(Item * pItem)		// 更新结点的估价函数值
{
	pItem->f = pItem->deep + (pItem->missionaries + pItem->cannibals - 2 * pItem->boat);
	return pItem->f;
}

void display(const Item * pItem, FILE * fp)	// 输出安全过河方案
{
	static int step;
	if (pItem->parent->parent == NULL)
	{
		step = 1;
		printf("\tStep %3d: ", step);
		fprintf(fp, "\tStep %3d: ", step);
		if (pItem->boat == 0)
		{
			printf(" --->(%2d,%2d)   >>> ", pItem->parent->missionaries - pItem->missionaries, pItem->parent->cannibals - pItem->cannibals);
			fprintf(fp, " --->(%2d,%2d)   >>> ", pItem->parent->missionaries - pItem->missionaries, pItem->parent->cannibals - pItem->cannibals);
		}
		else
		{
			printf(" <---(%2d,%2d)   >>> ", pItem->missionaries - pItem->parent->missionaries, pItem->cannibals - pItem->parent->cannibals);
			fprintf(fp, " <---(%2d,%2d)   >>> ", pItem->missionaries - pItem->parent->missionaries, pItem->cannibals - pItem->parent->cannibals);
		}
		showItem(pItem, fp);
	}
	else
	{
		display(pItem->parent, fp);
		printf("\tStep %3d: ", ++step);
		fprintf(fp, "\tStep %3d: ", step);
		if (pItem->boat == 0)
		{
			printf(" --->(%2d,%2d)   >>> ", pItem->parent->missionaries - pItem->missionaries, pItem->parent->cannibals - pItem->cannibals);
			fprintf(fp, " --->(%2d,%2d)   >>> ", pItem->parent->missionaries - pItem->missionaries, pItem->parent->cannibals - pItem->cannibals);
		}
		else
		{
			printf(" <---(%2d,%2d)   >>> ", pItem->missionaries - pItem->parent->missionaries, pItem->cannibals - pItem->parent->cannibals);
			fprintf(fp, " <---(%2d,%2d)   >>> ", pItem->missionaries - pItem->parent->missionaries, pItem->cannibals - pItem->parent->cannibals);
		}
		showItem(pItem, fp);
	}
}

void showItem(const Item * pItem, FILE * fp)	// 输出结点的信息
{
	printf(" (%2d,%2d,%2d) \n", pItem->missionaries, pItem->cannibals, pItem->boat);
	fprintf(fp, " (%2d,%2d,%2d) \n", pItem->missionaries, pItem->cannibals, pItem->boat);
}
