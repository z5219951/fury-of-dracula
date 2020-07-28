// QueueYue.c ... implementation of QueueYue ADT
// Written by John Shepherd, March 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "QueueYue.h"
#include "Map.h"
#include "Places.h"
typedef struct QueueYueNode {
	Item value;
	struct QueueYueNode *next;
} QueueYueNode;

typedef struct QueueYueRep {
	QueueYueNode *head; // ptr to first node
	QueueYueNode *tail; // ptr to last node
} QueueYueRep;

// create new empty QueueYue
QueueYue newQueueYue (void)
{
	QueueYueRep *new = malloc (sizeof *new);
	*new = (QueueYueRep){ .head = NULL, .tail = NULL };
	return new;
}

// free memory used by QueueYue
void dropQueueYue (QueueYue Q)
{
	assert (Q != NULL);
	for (QueueYueNode *curr = Q->head, *next; curr != NULL; curr = next) {
		next = curr->next;
		free (curr);
	}
	free (Q);
}

// display as 3 > 5 > 4 > ...
void showQueueYue (QueueYue Q)
{
	assert (Q != NULL);

	for (QueueYueNode *curr = Q->head; curr != NULL; curr = curr->next) {
		printf("%s", placeIdToAbbrev(curr->value));
		if (curr->next != NULL)
			printf (">");
	}
	printf ("\n");
}

// add item at end of QueueYue
void QueueYueJoin (QueueYue Q, Item it)
{
	assert (Q != NULL);

	QueueYueNode *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (QueueYueNode){ .value = it, .next = NULL };

	if (Q->head == NULL)
		Q->head = new;
	if (Q->tail != NULL)
		Q->tail->next = new;
	Q->tail = new;
}

// remove item from front of QueueYue
Item QueueYueLeave (QueueYue Q)
{
	assert (Q != NULL);
	assert (Q->head != NULL);
	Item it = Q->head->value;
	QueueYueNode *old = Q->head;
	Q->head = old->next;
	if (Q->head == NULL)
		Q->tail = NULL;
	free (old);
	return it;
}

// check for no items
int QueueYueIsEmpty (QueueYue Q)
{
	return (Q->head == NULL);
}
