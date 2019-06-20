#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define ValueFunction 2
#if ValueFunction == 2
#define DIFF(A, B) ((A) > (B) ? (A) - (B) : (B) - (A))
#endif

/* local function prototype */
static void CopyToNode(Item item, Node * pnode);

/* interface functions   */
/* set the list to empty */
void InitializeList(List * plist)
{
	*plist = NULL;
}

/* returns true if list is empty */
bool ListIsEmpty(const List * plist)
{
	if (*plist == NULL)
		return true;
	else
		return false;
}

/* returns true if list is full */
bool ListIsFull(const List * plist)
{
	Node * pt;
	bool full;

	pt = (Node *)malloc(sizeof(Node));
	if (pt == NULL)
		full = true;
	else
		full = false;
	free(pt);

	return full;
}

/* returns number of nodes */
unsigned int ListItemCount(const List * plist)
{
	unsigned int count = 0;
	Node * pnode = *plist;    /* set to start of list */

	while (pnode != NULL)
	{
		++count;
		pnode = pnode->next;  /* set to next node     */
	}

	return count;
}

//插入到一个有序单链表。函数的参数是一个指向链表第一个节点的指针的指针，以及一个需要插入的新值。
bool AddItem(const Item * pItem, List * plist)
{	// 为新节点分配内存，并把新值存储到新节点中，如果内存分配失败，函数返回FALSE。
	Node * temp = (Node *)malloc(sizeof(Node));
	if (temp == NULL)
		return false;
	CopyToNode(*pItem, temp);
	if (ListIsEmpty(plist))
	{
		*plist = temp;
		temp->next = NULL;
	}
	else
	{
		Node * p = *plist;
		if (p->item.f >= pItem->f)
		{
			*plist = temp;
			temp->next = p;
		}
		else
		{	// 寻找正确的插入位置，方法是按序访问链表，直到到达一个其值大于或等于新值的节点。
			while (p->next != NULL && p->next->item.f < pItem->f)
				p = p->next;
			// 在链表中插入新节点
			temp->next = p->next;
			p->next = temp;
		}
	}
	return true;
}

//链表头插法
bool headInserted(const Item * pItem, List * plist)
{
	Node * pnew = (Node *)malloc(sizeof(Node));
	if (pnew == NULL)
		return false;
	CopyToNode(*pItem, pnew);
	pnew->next = *plist;
	*plist = pnew;
	return true;
}

//链表尾插法
bool tailInserted(const Item * pItem, List * plist)
{
	Node * temp = (Node *)malloc(sizeof(Node));
	if (temp == NULL)
		return false;
	CopyToNode(*pItem, temp);
	if (ListIsEmpty(plist))
	{
		*plist = temp;
		temp->next = NULL;
	}
	else
	{
		Node * p = *plist;
		while (p->next != NULL)
			p = p->next;
		p->next = temp;
		temp->next = NULL;
	}
	return true;
}

bool DeleteItem(List * plist)
{
	if (ListIsEmpty(plist))
		return false;
	else
	{
		Node * temp = *plist;
		*plist = temp->next;
		free(temp);
		return true;
	}
}

/* visit each node and execute function pointed to by pfun */
bool Traverse(const List * plist, const Item * pItem, char(*pfun)(const Item * pItem, const Item * pTarget))
{
	Node * pnode = *plist;    /* set to start of list   */
	Item temp;

	while (pnode != NULL)
	{
		temp = pnode->item;
		if ((pfun(pItem, &temp) == 0) && (pItem->deep > temp.deep)) /* apply function to item */
			return true;
		pnode = pnode->next;  /* advance to next item   */
	}
	return false;
}

/* free memory allocated by malloc() */
/* set list pointer to NULL          */
void EmptyTheList(List * plist)
{
	Node * psave;

	while (*plist != NULL)
	{
		psave = (*plist)->next; /* save address of next node */
		free(*plist);           /* free current node         */
		*plist = psave;         /* advance to next node      */
	}
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

void display(const Item * pItem)
{
	static int step;

	if (pItem->parent == NULL)
	{
		step = 0;
		printf("\tStep %d: \n", step);
		showItem(pItem);
		return;
	}
	if (pItem->parent->parent == NULL)
	{
		step = 1;
		printf("\tStep %d: \n", step);
		showItem(pItem);
		return;
	}
	else
	{
		display(pItem->parent);
		printf("\tStep %d: \n", ++step);
		showItem(pItem);
		return;
	}
}

void showItem(const Item * pItem)
{
	char i, j;
	for (i = 0; i < 3; i++)
	{
		putchar('\t');
		putchar('\t');
		for (j = 0; j < 3; j++)
			printf(" %1d", pItem->chessboard[i][j]);
		putchar('\n');
	}
	putchar('\n');
}

/* local function definition  */
/* copies an item into a node */
static void CopyToNode(Item item, Node * pnode)
{
	pnode->item = item;  /* structure copy */
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
