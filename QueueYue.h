// Queue.h ... interface to Queue ADT
// Written by John Shepherd, March 2013

#ifndef QUEUE_H
#define QUEUE_H
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h"
#include "Places.h"
typedef PlaceId Item;

typedef struct QueueYueRep *QueueYue;

QueueYue newQueueYue (void);			// create new empty queueYue
void dropQueueYue (QueueYue);			// free memory used by queueYue
void showQueueYue (QueueYue);			// display as 3 > 5 > 4 > ...
void QueueYueJoin (QueueYue, Item);	// add item on queueYue
Item QueueYueLeave (QueueYue);		// remove item from queueYue
int QueueYueIsEmpty (QueueYue);		// check for no items

#endif
