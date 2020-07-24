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
			if (curr->type == RAIL) continue;
			// if current adjacent city satisfy conditon
			if (curr->type == ROAD || curr->type == BOAT) {
				result[index++] = curr->p;
			}
			curr = curr->next;
		}
		*numReturnedLocs = counter;		
	} else {
		curr = MapGetConnections(gv->map, from);
		int index = 0;
		int determinant = round % 4;
		while (curr != NULL) {
			if (curr->type == RAIL) {
				if (determinant == 0) {
					counter--;
				} else if (determinant == 1) {
					result[index++] = curr->p;
				} else if (determinant == 2) {
					
				} else if (determinant == 3) {

				}
			}

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
			if (curr->type == RAIL) continue;
			// if current adjacent city satisfy conditon
			if (curr->type == ROAD  && road == true) {
				result[index++] = curr->p;
			} else if (curr->type == BOAT && boat == true) {
				result[index++] = curr->p;
			}
			curr = curr->next;
		}
		*numReturnedLocs = counter;		
	} else {
		curr = MapGetConnections(gv->map, from);
		int index = 0;
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
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO

