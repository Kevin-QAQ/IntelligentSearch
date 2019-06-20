#include <stdio.h>
#include <stdlib.h>

#include "list.h"

#define SMALLER(X, Y) ((X) > (Y) ? (Y) : (X))
#define MIN(X, Y, Z) SMALLER(X, SMALLER(Y, Z))

int NUMBER = 3;
int CAPACITY = 2;

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

//删除链表的一个结点
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

/* visit each node */
bool Traverse(const List * plist, const Item * pItem)
{
	Node * pnode = *plist;    /* set to start of list   */
	Item temp;

	while (pnode != NULL)
	{
		temp = pnode->item;
		if ((pItem->missionaries == temp.missionaries) && (pItem->cannibals == temp.cannibals) && (pItem->boat == temp.boat) && (pItem->deep > temp.deep))
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

char setF(Item * pItem)		//更新结点的估价函数值
{
	pItem->f = pItem->deep + (pItem->missionaries + pItem->cannibals - 2 * pItem->boat);
	return pItem->f;
}

void update(Item * pItem, List * pOpenList)		//更新链表
{
	setF(pItem);
	if (AddItem(pItem, pOpenList) == false)
	{
		fprintf(stderr, "Problem allocating memory\n\n");
		exit(EXIT_FAILURE);
	}
}

void display(const Item * pItem)	//输出安全过河方案
{
	static int step;
	if (pItem->parent->parent == NULL)
	{
		step = 1;
		printf("\tStep %3d: ", step);
		if (pItem->boat == 0)
			printf(" --->(%1d, %1d)   >>> ", pItem->parent->missionaries - pItem->missionaries, pItem->parent->cannibals - pItem->cannibals);
		else
			printf(" <---(%1d, %1d)   >>> ", pItem->missionaries - pItem->parent->missionaries, pItem->cannibals - pItem->parent->cannibals);
		showItem(pItem);
	}
	else
	{
		display(pItem->parent);
		printf("\tStep %3d: ", ++step);
		if (pItem->boat == 0)
			printf(" --->(%1d, %1d)   >>> ", pItem->parent->missionaries - pItem->missionaries, pItem->parent->cannibals - pItem->cannibals);
		else
			printf(" <---(%1d, %1d)   >>> ", pItem->missionaries - pItem->parent->missionaries, pItem->cannibals - pItem->parent->cannibals);
		showItem(pItem);
	}
}

void showItem(const Item * pItem)	//输出结点的信息
{
	printf(" ( %1d, %1d, %1d ) \n", pItem->missionaries, pItem->cannibals, pItem->boat);
}

/* local function definition  */
/* copies an item into a node */
static void CopyToNode(Item item, Node * pnode)
{
	pnode->item = item;  /* structure copy */
}

//从当前结点扩展出合法的新结点
bool Expand(Item * pItem, List * pOpenList, const List * pCloseList)
{
	bool boolFlag = false;
	int deltaM, deltaC;
	Item temp;

	if (pItem->boat == 0)	//船在河对岸，需要从对岸运到本岸
	{
		//船上至少一个修道士
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

		//船上没有修道士
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
	else	//船在本岸，需要运到河对岸
	{
		//船上至少一个修道士
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

		//船上没有修道士
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

bool isLegal(const Item * pItem)	//判断结点状态是否合法
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
