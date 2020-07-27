////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// DraculaView.c: the DraculaView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"
// add your own #includes here
#include<string.h>
// TODO: ADD YOUR OWN STRUCTS HERE

struct draculaView {
	// TODO: ADD FIELDS HERE
	char **Path;
	int num;
	Map map;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DraculaView DvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	DraculaView new = malloc(sizeof(*new));
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

void DvFree(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free(dv->Path);
	MapFree(dv->map);
	free(dv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int DvGetScore(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int DvGetHealth(DraculaView dv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId DvGetVampireLocation(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numTraps = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// If  Dracula  hasn't  made  a move yet
	if (dv->num < 5) {
		*numReturnedMoves = 0;
		return NULL;
	}
	int round;
	// Get current round
	if (dv->num < 6) {
		round = 1;
	}
	else {
		if (dv->num % 5 == 0) {
			round = dv->num / 5;
		} else {
			round = ((int)(dv->num / 5)) + 1;
		}
	}
	// Get current location of dracula from last round
	char LastMove[1][3];
	LastMove[0][0] = dv->Path[5 * (round - 1) - 1][1];
	LastMove[0][1] = dv->Path[5 * (round - 1) - 1][2];
	LastMove[0][2] = '\0';
	char Past2Move[1][3];
	int from = placeAbbrevToId(LastMove);

	int n = MapNumPlaces(dv->map);
	int *repeated_city = calloc(n, sizeof(int));
	int counter = 0;
	ConnList curr = MapGetConnections(dv->map, from);
	while (curr != NULL) {
		counter++;
		curr = curr->next;
	}
	PlaceId *result = malloc(sizeof(PlaceId) * counter);
	// Get past 5 moves of dracula
	char Past5Move[5][3];
	if (round < 6) {
		for (int i = 0, j = 4; i < round - 1; i++, j+=5) {
			Past5Move[i][0] = dv->Path[j][1];
			Past5Move[i][1] = dv->Path[j][2];
			Past5Move[i][2] = '\0';
		}
	} else {
		for (int i = 0, j = round - 5 - 1; i < 5; i++, j++) {
			Past5Move[i][0] = dv->Path[4 + j * 5][1];
			Past5Move[i][1] = dv->Path[4 + j * 5][2];
			Past5Move[i][2] = '\0';
		}
	}

	curr = MapGetConnections(dv->map, from);

	if (from == HIDE || from == DOUBLE_BACK_1) {
		if (round > 2) {
			char Last2Move[1][3];
			Last2Move[0][0] = dv->Path[5 * (round - 1) - 6][1];
			Last2Move[0][1] = dv->Path[5 * (round - 1) - 6][2];
			Last2Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last2Move));
		}
	}
	if (from == DOUBLE_BACK_2) {
		if (round > 3) {
			char Last3Move[1][3];
			Last3Move[0][0] = dv->Path[5 * (round - 1) - 11][1];
			Last3Move[0][1] = dv->Path[5 * (round - 1) - 11][2];
			Last3Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last3Move));
		}
	}

	if (from == DOUBLE_BACK_3) {
		if (round > 4) {
			char Last4Move[1][3];
			Last4Move[0][0] = dv->Path[5 * (round - 1) - 16][1];
			Last4Move[0][1] = dv->Path[5 * (round - 1) - 16][2];
			Last4Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last4Move));
		}
	}

	if (from == DOUBLE_BACK_4) {
		if (round > 5) {
			char Last5Move[1][3];
			Last5Move[0][0] = dv->Path[5 * (round - 1) - 21][1];
			Last5Move[0][1] = dv->Path[5 * (round - 1) - 21][2];
			Last5Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last5Move));
		}
	}
	if (from == DOUBLE_BACK_5) {
		if (round > 6) {
			char Last6Move[1][3];
			Last6Move[0][0] = dv->Path[5 * (round - 1) - 21][1];
			Last6Move[0][1] = dv->Path[5 * (round - 1) - 21][2];
			Last6Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last6Move));
		}
	}


	int index = 0;
	int round_temp = round;
	// determing whether a connected satisfy condition
	while (curr != NULL) {
		if (curr->p == ST_JOSEPH_AND_ST_MARY) {
			curr = curr->next;
			continue;
		}
		if (curr->type == RAIL) {
			curr = curr->next;
			continue;
		}
		bool hasRepeatedMove = false;
		bool hasRepeatedHide = false;
		bool hasRepeatedDB = false;
		if (round < 6) {
			round_temp = round;
		} else {
			round_temp = 5;
		}
		// Check if dracula has made the same move in past 5 round
		for (int i = 0; i < round_temp; i++) {
			int curr_ID = placeAbbrevToId(Past5Move[i]);
			if (strcmp(Past5Move[i], placeIdToAbbrev(curr->p)) == 0) {
				hasRepeatedMove = true;
			}
			if (curr_ID == HIDE) {
				hasRepeatedHide == true;
			}
			if (curr_ID == DOUBLE_BACK_1 || curr_ID == DOUBLE_BACK_2 
				|| curr_ID == DOUBLE_BACK_3 || curr_ID == DOUBLE_BACK_4 
				|| curr_ID == DOUBLE_BACK_5) {
				hasRepeatedDB = true;
			}
			if (!hasRepeatedHide) {
				if (!placeIsSea(from)) {
					result[index++] = HIDE;
					result[index] = '\0';
				}
			} 
			if (!hasRepeatedDB) {
				result[index++] = DOUBLE_BACK_1;
				result[index++] = DOUBLE_BACK_2;
				result[index++] = DOUBLE_BACK_3;
				result[index++] = DOUBLE_BACK_4;
				result[index++] = DOUBLE_BACK_5;
				result[index] = '\0';
			}
			if ((curr->type == ROAD || curr->type == BOAT) && repeated_city[curr->p] == 0)  {
				result[index++] = curr->p;
				result[index] = '\0';
				repeated_city[curr->p] = 1;
			}
			
		curr = curr->next;
		}
	*numReturnedMoves = GetLenOfList(result);
	return result;
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
// get the number of element in an array
int GetLenOfList(PlaceId *list) {
	int i = 0;
	while (list[i] != '\0') i++;
	return i;
}