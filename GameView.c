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
    while (word!= NULL)
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
	*numReturnedMoves = 0;
	*canFree = false;
	// check the gv is not null
	if (gv == NULL || gv->num == 0) {
		return NULL;
	}
	int maxLen = gv->num/5+1*(gv->num%5 > 0);
	PlaceId *result = malloc(sizeof(PlaceId)*maxLen);
	// record the move
	int totalNum = 0;
	// record place
	char collect[maxLen][3];
	for (int i = player; i < gv->num;  i+=5) {
		collect[totalNum][0] = gv->Path[i][1];
		collect[totalNum][1] = gv->Path[i][2];
		collect[totalNum][2] = '\0';
		totalNum++;
	}
	// change the reord into placeid
	for (int i = 0; i < totalNum; i++) {
		result[i] = placeAbbrevToId(collect[i]);
	}
	*numReturnedMoves = totalNum;
	*canFree = true;
	return result;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	*numReturnedMoves = 0;
	*canFree = true;
	// check the gv is not null
	// return NULL if numMoves =0 
	if (gv == NULL || gv->num == 0 || numMoves == 0) {
		return NULL;
	}
	int maxLen = gv->num/5+1*(gv->num%5 > 0);
	PlaceId *result = malloc(sizeof(PlaceId)*maxLen);
	// record the move
	int totalNum = 0;
	// record place
	char collect[maxLen][3];
	for (int i = player; i < gv->num;  i+=5) {
		collect[totalNum][0] = gv->Path[i][1];
		collect[totalNum][1] = gv->Path[i][2];
		collect[totalNum][2] = '\0';
		totalNum++;
	}
	*numReturnedMoves = totalNum;
	// change the reord into placeid
	int startPoint = 0;
	if (totalNum > numMoves) {
		startPoint = totalNum - numMoves;
		*numReturnedMoves = numMoves;
	}
	for (int i = 0; i < totalNum && startPoint < maxLen; i++) {
		result[i] = placeAbbrevToId(collect[startPoint]);
		startPoint++;
	}
	return result;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	*numReturnedLocs = 0;
	*canFree = false;
	// check the gv is not null
	if (gv == NULL || gv->num == 0) {
		return NULL;
	}
	PlaceId *result;
	// there is no differences between GvGetLocationHistory and GvGetMoveHistory
	// when hunter player use this function.
	result = GvGetMoveHistory(gv, player, numReturnedLocs, canFree);

	//modify the result when DRACULA
	if (player == PLAYER_DRACULA) {
		for (int i = 0; i < *numReturnedLocs; i++) {
			if (DOUBLE_BACK_1 <= result[i] && DOUBLE_BACK_5>= result[i]) {
				int back = result[i] - DOUBLE_BACK_1 + 1;
				result[i] = result[i-back];
				// check whether i place is CASTLE_DRACULA
				if (result[i] == TELEPORT) {
					result[i] = CASTLE_DRACULA;
				}
			}
			if (HIDE == result[i]) {
				result[i] = result[i-1];
				if (result[i] == TELEPORT) {
					result[i] = CASTLE_DRACULA;
				}
			}
			if (result[i] == TELEPORT) {
				result[i] = CASTLE_DRACULA;
			}
		}
	}
	return result;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	*numReturnedLocs = 0;
	*canFree = false;
	// check the gv is not null
	// return NULL if numLocs =0 
	if (gv == NULL || gv->num == 0 || numLocs == 0) {
		return NULL;
	}
	// get result from GvGetLocationHistory
	PlaceId *result = GvGetLocationHistory(gv, player, numReturnedLocs, canFree);
	// change the reord into placeid
	int totalNum = *numReturnedLocs;
	int startPoint = 0;
	if (totalNum > numLocs) {
		startPoint = totalNum - numLocs;
		*numReturnedLocs = numLocs;
	}
	for (int i = 0; i < totalNum && startPoint < totalNum; i++) {
		result[i] = result[startPoint];
		startPoint++;
	}
	return result;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	if (round == 0) {
		return NULL;
	}
	int n = MapNumPlaces(gv->map);
	// Two cities may be connected through multiple ways
	// The following array is to avoid the same city be added
	int *repeated_city = calloc(n, sizeof(int));
	// Counter is the number of connection
	int counter = 0;
	ConnList curr = MapGetConnections(gv->map, from);
	while (curr != NULL) {
		counter++;
		curr = curr->next;
	}
	PlaceId *result = malloc(sizeof(PlaceId) * (counter + 2));
	// Add the given place into array
	result[0] = from;
	result[1] = 999;
	// If current player is dracula
	if (player == PLAYER_DRACULA) {
		// Test Connection
		if (round == 1) {
			int index = 1;
			curr = MapGetConnections(gv->map, from);
			while (curr != NULL) {
				if ((curr->type == ROAD || curr->type == BOAT) && repeated_city[curr->p] == 0) {
					result[index++] = curr->p;
					result[index] = 999;
					repeated_city[curr->p] = 1;
				}
				curr = curr->next;
			}
			*numReturnedLocs = index;
			return result;
		}
		// Get past 5 move of dracula
		char Past5Move[5][3];
		if (round < 6) {
			for (int i = 0, j = 4; i < round - 1; i++, j+=5) {
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
		// Check whether a adjacent city satisfy condition
		int index = 1;
		int round_temp;
		if (round < 6) {
			round_temp = round - 1;
		} else {
			round_temp = 5;
		}
		curr = MapGetConnections(gv->map, from);
		while (curr != NULL) {
			// Dracula can not move through rail
			if (curr->type == RAIL) {
				curr = curr->next;
				continue;
			}
			/// If the adjacent place is hospital
			if (curr->p == ST_JOSEPH_AND_ST_MARY) {
				curr = curr->next;
				continue;
			} 
			bool hasRepeatedMove = false;
			// Check if dracula has made the same location move in the past 5 rounds
			for (int i = 0; i < round_temp; i++) {
				if (strcmp(Past5Move[i], placeIdToAbbrev(curr->p)) == 0) {
					hasRepeatedMove = true;
				}
			}
			// If dracula has made the same location move in the past 5 rounds
			// Then skip current location to next adjacent location
			if (hasRepeatedMove) {
				curr = curr->next;
				continue;
			}
			// If current adjacent city satisfy conditon
			if (curr->type == ROAD && repeated_city[curr->p] == 0) {
				result[index++] = curr->p;
				result[index] =	999;
				repeated_city[curr->p] = 1;
			}
			else if (curr->type == BOAT && repeated_city[curr->p] == 0)  {
				result[index++] = curr->p;
				result[index] = 999;
				repeated_city[curr->p] = 1;
			}
			curr = curr->next;
		}
		*numReturnedLocs = GetLenOfList(result);		
	} else {
		curr = MapGetConnections(gv->map, from);
		int index = 1;
		int max_distance = (round + player) % 4;
		PlaceId *RailList = GetConnRail(gv->map, from, max_distance, repeated_city);
		while (curr != NULL) {
			if (repeated_city[curr->p] == 1) {
				curr=curr->next;
				continue;
			}
			if (curr->type == ROAD) {
				result[index++] = curr->p;
				result[index] = 999;
				repeated_city[curr->p] = 1;
			}
			else if (curr->type == BOAT) {
				// From sea to sea or port city
				if (placeIsSea(from)) {
					result[index++] = curr->p;
					result[index] = 999;
					repeated_city[curr->p] = 1;
				}
				// From port city to adjacent sea
				else if (placeIsLand(from) && placeIsSea(curr->p)) {
					result[index++] = curr->p;
					result[index] = 999;
					repeated_city[curr->p] = 1;
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
	if (round == 0) {
		return NULL;
	}
	if (road == false && rail == false && boat == false) {
		PlaceId *result = malloc(sizeof(PlaceId) * 2);
		result[0] = from;
		result[1] = 999;
		return result;
	}
	int n = MapNumPlaces(gv->map);
	// Two cities may be connected through multiple ways
	// The following array is to avoid the same city be added
	int *repeated_city = calloc(n, sizeof(int));
	// Counter is the number of conncetion
	int counter = 0;
	ConnList curr = MapGetConnections(gv->map, from);
	// Get the number of adjacent cities
	while (curr != NULL) {
		counter++;
		curr = curr->next;
	}
	PlaceId *result = malloc(sizeof(PlaceId) * (counter + 2));
	// Add the given place into array
	result[0] = from;
	result[1] = 999;
	if (player == PLAYER_DRACULA) {
		// Test connection
		if (round == 1) {
			int index = 1;
			curr = MapGetConnections(gv->map, from);
			while (curr != NULL) {
				if (curr->type == ROAD && road == true) {
					result[index++] = curr->p;
					result[index] = 999; 
				} else if (curr->type == BOAT && boat == true) {
					result[index++] = curr->p;
					result[index] = 999; 
				}
				curr = curr->next;
			}
			*numReturnedLocs = index;
			return result;
		}
		char Past5Move[5][3];
		// Get past 5 move of dracula
		if (round < 6) {
			for (int i = 0, j = 4; i < round - 1; i++, j+=5) {
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
		// Check whether a adjacent city satisfy condition
		int index = 1;
		int round_temp;
		if (round < 6) {
			round_temp = round;
		} else {
			round_temp = 5;
		}
		curr = MapGetConnections(gv->map, from);
		while (curr != NULL) {
			// If the adjacent city is hospital
			if (curr->p == ST_JOSEPH_AND_ST_MARY) {
				curr = curr->next;
				continue;
			} 
			// Dracula can not move through rail
			if (curr->type == RAIL) {
				curr = curr->next;
				continue;
			}
			bool hasRepeatedMove = false;
			// Check if dracula has made the same location move in the past 5 rounds
			for (int i = 0; i < round_temp; i++) {
				if (strcmp(Past5Move[i], placeIdToAbbrev(curr->p)) == 0) {
					hasRepeatedMove = true;
				}
			}
			// If dracula has made the same move in the past 5 rounds
			if (hasRepeatedMove) {
				curr = curr->next;
				continue;
			}	
			// If dracula has made the same location move in the past 5 rounds
			// Then skip current location to next adjacent location
			if (curr->type == ROAD  && road == true) {
				result[index++] = curr->p;
				result[index] = 999; 
				repeated_city[curr->p] = 1;
			} else if (curr->type == BOAT && boat == true) {
				result[index++] = curr->p;
				result[index] = 999; 
				repeated_city[curr->p] = 1;
			}
			curr = curr->next;
		}
		*numReturnedLocs = GetLenOfList(result);
	} else {
		curr = MapGetConnections(gv->map, from);
		int index = 1;
		int max_distance = (round + player) % 4;
		PlaceId *RailList = NULL;
		if (rail == true) {
			RailList = GetConnRail(gv->map, from, max_distance, repeated_city);
		}
		while (curr != NULL) {
			if (repeated_city[curr->p] == 1) {
				curr=curr->next;
				continue;
			}
			if (curr->type == ROAD && road == true) {
				result[index++] = curr->p;
				result[index] = 999;
				repeated_city[curr->p] = 1;
			}
			else if (curr->type == BOAT && boat == true) {
				// From sea to sea or port city
				if (placeIsSea(from)) {
					result[index++] = curr->p;
					result[index] = 999;
					repeated_city[curr->p] = 1;
				}
				// From port city to adjaceent sea
				else if (placeIsLand(from) && placeIsSea(curr->p)) {
					result[index++] = curr->p;
					result[index] = 999;
					repeated_city[curr->p] = 1;
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

// get the cities conncected with place "from" through rail
PlaceId *GetConnRail(Map map, PlaceId from, int max_distance, int *repeated_city)
{
	if (max_distance == 0) {
		return NULL;
	}
	
	int n = MapNumPlaces(map);
	int *visited = calloc(n, sizeof(int));
	int *distance = calloc(n, sizeof(int));

	Queue q = newQueue();
	QueueJoin(q, from);
	visited[from] = 1;

	while(!QueueIsEmpty(q)) {
		int x = QueueLeave(q);
		ConnList curr = MapGetConnections(map, x);
		while (curr != NULL) {
			if (visited[curr->p] == 1 || curr->type != RAIL) {
				curr = curr->next;
				continue;
			}
			visited[curr->p] = 1;
			distance[curr->p] = distance[x] + 1;
			QueueJoin(q, curr->p);
			curr = curr->next;
		}
	}

	PlaceId *list = malloc(sizeof(PlaceId) * (n + 1));
	int counter = 0;
	// Make sure that there is no repeated city in returned array of GvGetReachable
	for (int i = 0; i < n; i++) {
		if (distance[i] == 1) {
			repeated_city[i] = 1;
		}
	}

	if (max_distance == 1) {
		for (int i = 0; i < n; i++) {
			if (distance[i] == 1) {
				list[counter++] = i;
			}
		}
	} else if (max_distance == 2) {
		for (int i = 0; i < n; i++) {
			if (distance[i] == 1 || distance[i] == 2) {
				list[counter++] = i;
			}
		}
	} else if (max_distance == 3) {
		for (int i = 0; i < n; i++) {
			if (distance[i] == 1 || distance[i] == 2 || distance[i] == 3)  {
				list[counter++] = i;
			}
		}
	}

	list[counter] = 999;
	dropQueue(q);
	free(visited);
	free(distance);
	return list;
}
// merge two list
PlaceId *MergeList(PlaceId *list1, PlaceId *list2) {
	int len1 = GetLenOfList(list1);
	int len2 = GetLenOfList(list2);
	PlaceId *newList = malloc(sizeof(PlaceId) * (len1 + len2 + 1));
	for (int i = 0; i < len1; i++) {
		newList[i] = list1[i];
	}
	for (int i = len1, j = 0; i < len1 + len2; i++, j++) {
		newList[i] = list2[j];
	}
	newList[len1 + len2] = 999;
	return newList;
}
// Get the number of element in an array
// The list should end with '\0'
int GetLenOfList(PlaceId *list) {
	int i = 0;
	while (list[i] < 999) {
		i++;
	}
	return i;
}