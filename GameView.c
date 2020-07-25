////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// GameView.c: GameView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "Places.h"
// add your own #includes here
#include<string.h>
#include "Queue.h"
// TODO: ADD YOUR OWN STRUCTS HERE

struct gameView {
	// TODO: ADD FIELDS HERE
	char **Path;
	int num;
	Map map;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

GameView GvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	GameView new = malloc(sizeof(GameView));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate GameView!\n");
		exit(EXIT_FAILURE);
	}
	char delim[2] = " "; 

    char cp[strlen(pastPlays)+1];
    strcpy(cp, pastPlays);

    int len = (strlen(pastPlays)+1)/8;
    new->Path = malloc(sizeof(char *)*len);
    for (int i = 0; i < len; i++) {
        new->Path[i] = malloc(sizeof(char)*8);
    }
    int index = 0;
	char *word;
    word  = strtok(cp,delim);
    while (new->Path[index]!= NULL)
    {
        strcpy(new->Path[index], word);
		index++;
        word = strtok(NULL,delim);
    }
	new->num = index;
	new->map = MapNew();
	return new;
}

void GvFree(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free(gv->Path);
	MapFree(gv->map);
	free(gv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round GvGetRound(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

Player GvGetPlayer(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return PLAYER_LORD_GODALMING;
}

int GvGetScore(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int GvGetHealth(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId GvGetVampireLocation(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numTraps = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	if (gv == NULL || gv->num == 0) {
		return NULL;
	}
	// counter is the number of adjacent cities
	int counter = 0;
	ConnList curr = MapGetConnections(gv->map, from);
	while (curr != NULL) {
		counter++;
		curr = curr->next;
	}
	PlaceId *result = malloc(sizeof(PlaceId) * counter);
	// if player is dracula
	if (player == PLAYER_DRACULA) {
		// gain past 5 moves of dracula
		char Past5Move[5][3];
		if (round < 6) {
			for (int i = 0, j = 4; i < round; i++, j+=5) {
				Past5Move[i][0] = gv->Path[j][1];
				Past5Move[i][1] = gv->Path[j][2];
				Past5Move[i][2] = '\0';
			}
		} else {
			for (int i = 0, j = round - 5 - 1; i < 5; i++, j++) {
				Past5Move[i][0] = gv->Path[4 + j * 5][1];
				Past5Move[i][1] = gv->Path[4 + j * 5][2];
				Past5Move[i][2] = '\0';
			}
		}
		// check whether a adjacent city satisfy condition
		curr = MapGetConnections(gv->map, from);
		int index = 0;
		int round_temp = round;
		while (curr != NULL) {
			bool hasRepeatedMove = false;
			bool hasRepeatedDB = false;
			if (round < 6) {
				round_temp = round;
			} else {
				round_temp = 5;
			}
			// check if dracula has made same move in the past 5 rounds
			for (int i = 0; i < round_temp; i++) {
				int curr_ID = placeAbbrevToId(Past5Move[i]);
				if (strcmp(Past5Move[i], placeIdToAbbrev(curr->p) == 0)) {
					hasRepeatedMove = true;
				}
				if (curr_ID == DOUBLE_BACK_1 || curr_ID == DOUBLE_BACK_2 
					|| curr_ID == DOUBLE_BACK_3 || curr_ID == DOUBLE_BACK_4 
					|| curr_ID == DOUBLE_BACK_5) {
					hasRepeatedDB = true;
				}
			}
			// if dracula has made the same move in the past 5 rounds
			if (hasRepeatedMove) {
				result[counter - 1] = '\0';
				counter--;
				continue;
			}
			// if dracula has made DOUBLE_BACK in the past 5 rounds
			if (hasRepeatedDB) {
				result[counter - 1] = '\0';
				counter--;
				continue;
			}			

			// if the adjacent city is hospital
			if (curr->p == ST_JOSEPH_AND_ST_MARY) {
				result[counter - 1] = '\0';
				counter--;
				continue;
			} 
			// dracula hates rail
			if (curr->type == RAIL) {
				result[counter - 1] = '\0';
				counter--;
				continue;
			}
			// if current adjacent city satisfy conditon
			if (curr->type == ROAD || curr->type == BOAT) {
				result[index++] = curr->p;
			}
			curr = curr->next;
		}
		*numReturnedLocs = GetLenOfList(result);		
	} else {
		curr = MapGetConnections(gv->map, from);
		int index = 0;
		int max_distance = round % 4;
		PlaceId *RailList = GetConnRail(gv, from, max_distance);
		while (curr != NULL) {
			if (curr->type == ROAD) {
				result[index++] = curr->p;
			}
			if (curr->type == BOAT) {
				// from sea to sea or port city
				if (placeIsSea(from)) {
					result[index++] = curr->p;
				}
				// from port city to adjaceent sea
				if (placeIsLand(from) && placeIsSea(curr->p)) {
					result[index++] = curr->p;
				}
			}
			curr = curr->next;
		}
		if (RailList != NULL) {
			result = MergeList(result, RailList);
		}
		*numReturnedLocs = GetLenOfList(result);
	}
	return result;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	if (gv == NULL || gv->num == 0) {
		return NULL;
	}
	// counter is the number of adjacent cities
	int counter = 0;
	ConnList curr = MapGetConnections(gv->map, from);
	// get the number of adjacent cities
	while (curr != NULL) {
		counter++;
		curr = curr->next;
	}
	PlaceId *result = malloc(sizeof(PlaceId) * counter);
	
	if (player == PLAYER_DRACULA) {
		char Past5Move[5][3];
		int round_temp = round;
		// gain past 5 moves of dracula
		if (round < 6) {
			for (int i = 0, j = 4; i < round; i++, j+=5) {
				Past5Move[i][0] = gv->Path[j][1];
				Past5Move[i][1] = gv->Path[j][2];
				Past5Move[i][2] = '\0';
			}
		} else {
			for (int i = 0, j = round - 5 - 1; i < 5; i++, j++) {
				Past5Move[i][0] = gv->Path[4 + j * 5][1];
				Past5Move[i][1] = gv->Path[4 + j * 5][2];
				Past5Move[i][2] = '\0';
			}
		}

		curr = MapGetConnections(gv->map, from);
		int index = 0;
		while (curr != NULL) {
			bool hasRepeatedMove = false;
			bool hasRepeatedDB = false;
			int round_temp;
			if (round < 6) {
				round_temp = round;
			} else {
				round_temp = 5;
			}
			// check if dracula has made same move in the past 5 rounds
			for (int i = 0; i < round_temp; i++) {
				int curr_ID = placeAbbrevToId(Past5Move[i]);
				if (strcmp(Past5Move[i], placeIdToAbbrev(curr->p) == 0)) {
					hasRepeatedMove = true;
				}
				if (curr_ID == DOUBLE_BACK_1 || curr_ID == DOUBLE_BACK_2 
					|| curr_ID == DOUBLE_BACK_3 || curr_ID == DOUBLE_BACK_4 
					|| curr_ID == DOUBLE_BACK_5) {
					hasRepeatedDB = true;
				}
			}
			// if dracula has made the same move in the past 5 rounds
			if (hasRepeatedMove) {
				result[counter - 1] = '\0';
				counter--;
				continue;
			}
			// if dracula has made DOUBLE_BACK in the past 5 rounds
			if (hasRepeatedDB) {
				result[counter - 1] = '\0';
				counter--;
				continue;
			}			

			// if the adjacent city is hospital
			if (curr->p == ST_JOSEPH_AND_ST_MARY) {
				result[counter - 1] = '\0';
				counter--;
				continue;
			} 
			// dracula hates rail
			if (curr->type == RAIL) {
				result[counter - 1] = '\0';
				counter--;
				continue;
			}
			// if current adjacent city satisfy conditon
			if (curr->type == ROAD  && road == true) {
				result[index++] = curr->p;
			} else if (curr->type == BOAT && boat == true) {
				result[index++] = curr->p;
			}
			curr = curr->next;
		}
		*numReturnedLocs = GetLenOfList(result);
	} else {
		curr = MapGetConnections(gv->map, from);
		int index = 0;
		int max_distance = round % 4;
		PlaceId *RailList = RailList = GetConnRail(gv, from, max_distance);
		while (curr != NULL) {
			if (curr->type == ROAD && road == true) {
				result[index++] = curr->p;
			}
			if (curr->type == BOAT && boat == true) {
				// from sea to sea or port city
				if (placeIsSea(from)) {
					result[index++] = curr->p;
				}
				// from port city to adjaceent sea
				if (placeIsLand(from) && placeIsSea(curr->p)) {
					result[index++] = curr->p;
				}
			}
			curr = curr->next;
		}
		if (rail == true && RailList != NULL) {
			result = MergeList(result, RailList);
		}
		*numReturnedLocs = GetLenOfList(result);
	}
	return result;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO

// get the adjacent cities which can travel throught rail  
PlaceId *GetConnRail(GameView gv, PlaceId from, int max_distance)
{
	if (max_distance == 0) return NULL;
	
	int n = MapNumPlaces(gv->map);
	int *visited = calloc(n, sizeof(int));
	int *distance = calloc(n, sizeof(int));

	Queue q = newQueue();
	QueueJoin(q, from);
	visited[from] = 1;
	while(!QueueIsEmpty(q)) {
		int x = QueueLeave(q);
		ConnList curr = MapGetConnections(gv->map, x);
		while (curr != NULL) {
			if (visited[curr->p] == 1) continue;
			if (curr->type == RAIL) {
				visited[curr->p] = 1;
				distance[curr->p] = distance[x] + 1;
				QueueJoin(q, curr->p);
			}
			curr = curr->next;
		}
	}
	PlaceId *list = malloc(sizeof(PlaceId) * n + 1);
	int counter = 0;
	if (max_distance == 1) {
		for (int i = 0; i < n; i++) {
			if (distance[i] == 1) {
				list[counter++] = distance[i];
			}
		}
	} else if (max_distance == 2) {
		for (int i = 0; i < n; i++) {
			if (distance[i] == 1 || distance[i] == 2) {
				list[counter++] = distance[i];
			}
		}
	} else if (max_distance == 3) {
		for (int i = 0; i < n; i++) {
			if (distance[i] == 1 || distance[i] == 2 || distance[i] == 3)  {
				list[counter++] = distance[i];
			}
		}
	}
	list[counter] = '\0';
	dropQueue(q);
	free(visited);
	free(distance);
	return list;
}
// merge two list
PlaceId *MergeList(PlaceId *list1, PlaceId *list2) {
	int len = GetLenOfList(list1) + GetLenOfList(list2);
	PlaceId *newList = malloc(sizeof(PlaceId) * len + 1);
	for (int i = 0; i < len; i++) {
		if (i < strlen(list1)) {
			newList[i] = list1[i];
		} else {
			newList[i] = list2[i];
		}
	}
	newList[len] = '\0';
	return newList;
}
// get the number of element in an array
int GetLenOfList(PlaceId *list) {
	int i = 0;
	while (list[i] != '\0') i++;
	return i;
}