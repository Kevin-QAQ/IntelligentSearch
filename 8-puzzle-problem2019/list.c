#include <stdio.h>
#include <stdlib.h>
#include "list.h"

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

//���뵽һ���������������Ĳ�����һ��ָ�������һ���ڵ��ָ���ָ�룬�Լ�һ����Ҫ�������ֵ��
bool AddItem(const Item * pItem, List * plist)
{	// Ϊ�½ڵ�����ڴ棬������ֵ�洢���½ڵ��У�����ڴ����ʧ�ܣ���������FALSE��
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
		{	// Ѱ����ȷ�Ĳ���λ�ã������ǰ����������ֱ������һ����ֵ���ڻ������ֵ�Ľڵ㡣
			while (p->next != NULL && p->next->item.f < pItem->f)
				p = p->next;
			// �������в����½ڵ�
			temp->next = p->next;
			p->next = temp;
		}
	}
	return true;
}

//����ͷ�巨
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

//����β�巨
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

/* local function definition  */
/* copies an item into a node */
static void CopyToNode(Item item, Node * pnode)
{
	pnode->item = item;  /* structure copy */
}
