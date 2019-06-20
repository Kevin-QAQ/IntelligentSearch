#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdbool.h>     /* C99 feature         */

#define Astar 1
#define BFS 2
#define DFS 3

typedef struct status
{
	char chessboard[3][3];
	int deep;
	int f;
	struct status * parent;
} Item;

typedef struct node
{
	Item item;
	struct node * next;
} Node;

typedef Node * List;

/* function prototypes */

/* operation:        initialize a list
/* preconditions:    plist points to a list
/* postconditions:   the list is initialized to empty           */
void InitializeList(List * plist);

/* operation:        determine if list is empty
/*                   plist points to an initialized list
/* postconditions:   function returns True if list is empty
/*                   and returns False otherwise                */
bool ListIsEmpty(const List * plist);

/* operation:        determine if list is full
/*                   plist points to an initialized list
/* postconditions:   function returns True if list is full
/*                   and returns False otherwise                */
bool ListIsFull(const List *plist);

/* operation:        determine number of items in list
/*                   plist points to an initialized list
/* postconditions:   function returns number of items in list   */
unsigned int ListItemCount(const List *plist);

/* operation:        add item to end of list
/* preconditions:    item is an item to be added to list
/*                   plist points to an initialized list
/* postconditions:   if possible, function adds item to end
/*                   of list and returns True; otherwise the
/*                   function returns False                     */
bool AddItem(const Item * pItem, List * plist);

bool headInserted(const Item * pItem, List * plist);

bool tailInserted(const Item * pItem, List * plist);

bool DeleteItem(List * plist);

/* operation:        apply a function to each item in list
/*                   plist points to an initialized list
/*                   pfun points to a function that takes an
/*                   Item argument and has no return value
/* postcondition:    the function pointed to by pfun is
/*                   executed once for each item in the list    */
bool Traverse(const List * plist, const Item * pItem, char(*pfun)(const Item * pItem, const Item * pTarget));

/* operation:        free allocated memory, if any
/*                   plist points to an initialized list
/* postconditions:   any memory allocated for the list is freed
/*                   and the list is set to empty               */
void EmptyTheList(List * plist);

char calculateReverseOrder(Item * pItem);

char setF(Item * pItem, const Item * pTarget);

void update(Item * pItem, Item * father, const Item * pTarget, List * pOpenList, char method);

char count(const Item * pItem, const Item * ptarget);

void getRowAndColumn(const Item * pItem, char * pRow0, char * pColumn0, char index);

void display(const Item * pitem);

void showItem(const Item * pItem);

bool Expand(Item * pItem, List * pOpenList, const List * pCloseList, const Item * pTarget, char method);

#endif // LIST_H_INCLUDED
