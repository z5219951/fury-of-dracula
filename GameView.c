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

// added functions
void removeLoc(GameView gv, char tempLocs[TRAIL_SIZE][3], Round curr, int *numTraps);

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
	// check gv is not NULL
	if (gv == NULL || gv->num == 0) {
		return NOWHERE;
	}
	// int health = GvGetHealth(gv, player);
	int health = 9;
	if (health == 0) {
		return HOSPITAL_PLACE;
	}
	int numReturnedLocs = 1; 
	// bool canFree = 1;
	// PlaceId *result; 
	// result = GvGetLastLocations(gv, player, 1, &numReturnedLocs, &canFree); 
	if (numReturnedLocs == 0) { // if player has not had a turn yet
		return NOWHERE; 
	}
	// PlaceId location = *result;
	// if (canFree == 1) { // free array
	// 	free(result);
	// }
	char testloc[3] = "C?"; 
	return placeAbbrevToId(testloc);
}

PlaceId GvGetVampireLocation(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// check gv is not NULL
	if (gv == NULL || gv->num == 0) {
		return NOWHERE;
	}
	// char array for storing vampire's location (if exists)
	char location[3] = {};
	// get current round
	// Round round = GvGetRound(gv);
	Round round = 6;
	// scan through last 6 rounds, from earliest to most recent
	int curr = (round - TRAIL_SIZE) * NUM_PLAYERS;
	if (curr < 0) {
		curr = 0;
	}
	for (; curr < gv->num; curr++) {
		if (gv->Path[curr][0] == 'D' && 
			gv->Path[curr][4] == 'V') {
			// immature vampire placed on Dracula's turn
			location[0] = gv->Path[curr][1];
			location[1] = gv->Path[curr][2];
			location[2] = '\0';
		} else if (gv->Path[curr][3] == 'V' ||
				   gv->Path[curr][4] == 'V' ||
				   gv->Path[curr][5] == 'V') { 
			// immature vampire vanquished on hunter's turn
			location[0] = '\0';
		}
	}
	if (location[0] == '\0') { // immature vampire doesn't exist
		return NOWHERE;
	} 
	return placeAbbrevToId(location);
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// check gv is not NULL
	if (gv == NULL || gv->num == 0) {
		return NULL;
	}
	*numTraps = 0;
	// dynamically allocated array for storing trap locations (if any)
	PlaceId *locations = malloc(sizeof(PlaceId)*TRAIL_SIZE); 
	// array for storing potential trap locations 
	char tempLocs[TRAIL_SIZE][3];
	// get round and current player
	// int round = GvGetRound(gv);
	int round = 4;
	// int player = GvGetPlayer(gv);
	// if not Dracula's turn
	// if (player != PLAYER_DRACULA) {
	// 	printf("Cannot determine trap locations.\n");
	// 	return locations;
	// }
	// scan through last 6 rounds, from earliest to most recent
	int curr = (round - TRAIL_SIZE) * NUM_PLAYERS;
	if (curr < 0) {
		curr = 0;
	}
	for (; curr < gv->num; curr++) {
		if (gv->Path[curr][0] == 'D' &&
			gv->Path[curr][3] == 'T') { 
			// trap placed on Dracula's turn
			tempLocs[*numTraps][0] = gv->Path[curr][1];
			tempLocs[*numTraps][1] = gv->Path[curr][2];
			tempLocs[*numTraps][2] = '\0'; 
			*numTraps += 1;
		} else {
			// if trap encountered by Hunter
			if (gv->Path[curr][3] == 'T') { 
				removeLoc(gv, tempLocs, curr, numTraps);
			}
			if (gv->Path[curr][4] == 'T') { // trap encountered
				removeLoc(gv, tempLocs, curr, numTraps);
			}
			if (gv->Path[curr][5] == 'T') { // trap encountered
				removeLoc(gv, tempLocs, curr, numTraps);
			}
		}
	}
	// convert abbrievs to PlaceId
	for (int i = 0; i < *numTraps; i++) {
		locations[i] = placeAbbrevToId(tempLocs[i]); 
	}
	return locations;
}

// find and remove corresponding location from templocs 
void removeLoc(GameView gv, char tempLocs[TRAIL_SIZE][3], Round curr, int *numTraps) {
	for (int i = 0; i < *numTraps; i++) {
		if (strncmp(&(gv->Path[curr][1]), tempLocs[i], 2) == 0) {
			if (i != *numTraps-1) {
				// replace location to be removed with last location in array
				tempLocs[i][0] = tempLocs[*numTraps-1][0];
				tempLocs[i][1] = tempLocs[*numTraps-1][1];
				tempLocs[*numTraps-1][0] = '\0';
				*numTraps -= 1;
			} else {
				// remove last location
				tempLocs[*numTraps][0] = '\0';
				*numTraps -= 1;
			}
			break;
		}
	}
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
