#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdbool.h>     /* C99 feature         */

// 定义结构体保存本岸的修道士人数、野人人数和船只数目的状态信息
typedef struct status
{
	int missionaries;
	int cannibals;
	char boat;
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

/* operation:        initialize a list                          */
/* preconditions:    plist points to a list                     */
/* postconditions:   the list is initialized to empty           */
void InitializeList(List * plist);

/* operation:        determine if list is empty                 */
/*                   plist points to an initialized list        */
/* postconditions:   function returns True if list is empty     */
/*                   and returns False otherwise                */
bool ListIsEmpty(const List * plist);

/* operation:        determine if list is full                  */
/*                   plist points to an initialized list        */
/* postconditions:   function returns True if list is full      */
/*                   and returns False otherwise                */
bool ListIsFull(const List *plist);

/* operation:        determine number of items in list          */
/*                   plist points to an initialized list        */
/* postconditions:   function returns number of items in list   */
unsigned int ListItemCount(const List *plist);

/* operation:        add item to end of list                    */
/* preconditions:    item is an item to be added to list        */
/*                   plist points to an initialized list        */
/* postconditions:   if possible, function adds item to end     */
/*                   of list and returns True; otherwise the    */
/*                   function returns False                     */
bool AddItem(const Item * pItem, List * plist);

bool headInserted(const Item * pItem, List * plist);

bool DeleteItem(List * plist);

/* operation:        apply a function to each item in list      */
/*                   plist points to an initialized list        */
/*                   pfun points to a function that takes an    */
/*                   Item argument and has no return value      */
/* postcondition:    the function pointed to by pfun is         */
/*                   executed once for each item in the list    */
bool Traverse(const List * plist, const Item * pItem);

/* operation:        free allocated memory, if any              */
/*                   plist points to an initialized list        */
/* postconditions:   any memory allocated for the list is freed */
/*                   and the list is set to empty               */
void EmptyTheList(List * plist);

#endif // LIST_H_INCLUDED
