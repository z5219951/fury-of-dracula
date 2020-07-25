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
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
