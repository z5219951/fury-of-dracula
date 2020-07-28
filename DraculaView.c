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
	return// check dv is not NULL
	if (dv == NULL || dv->num == 0) {
		return NOWHERE;
	}
	int health = DvGetHealth(dv, player);
	if (health == 0) {
		return HOSPITAL_PLACE;
	}
	int numReturnedLocs = 1; 
	bool canFree = 1;
	PlaceId *result; 
	result = GvGetLastLocations(dv, player, 1, &numReturnedLocs, &canFree); 
	if (numReturnedLocs == 0) { // if player has not had a turn yet
		return NOWHERE; 
	}
	PlaceId location = *result;
	if (canFree == 1) { // free array
		free(result);
	}
	return location;
}

PlaceId DvGetVampireLocation(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// check dv is not NULL
	if (dv == NULL || dv->num == 0) {
		return NOWHERE;
	}
	bool canFree = 1;
	int numReturnedLocs;
	PlaceId *locations = GvGetLastLocations(dv, PLAYER_DRACULA, TRAIL_SIZE, 
								   &numReturnedLocs, &canFree);
	// get current round
	Round round = DvGetRound(dv);
	// scan through last 6 rounds, from earliest to most recent
	int curr = (round - TRAIL_SIZE) * NUM_PLAYERS;
	if (curr < 0) {
		curr = 0;
	}
	int vampLoc = 0;
	for (int counter = 1; curr < dv->num; curr++, counter++) {
		if (dv->Path[curr][0] == 'D' && 
			dv->Path[curr][4] == 'V') {
			vampLoc = counter / 5;
		} else if (dv->Path[curr][3] == 'V' ||
				   dv->Path[curr][4] == 'V' ||
				   dv->Path[curr][5] == 'V') { 
			// immature vampire vanquished on hunter's turn
			return NOWHERE;
		}
	}
	PlaceId result = locations[vampLoc-1];
	if (canFree) {
		free(locations);
	}
	return result;
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// check dv is not NULL
	if (dv == NULL || dv->num == 0) {
		return NULL;
	}
	*numTraps = 0;
	// dynamically allocated array for storing trap locations (if any)
	PlaceId *locations = malloc(sizeof(PlaceId)*TRAIL_SIZE); 
	int player = DvGetPlayer(dv);
	// if not Dracula's turn
	if (player != PLAYER_DRACULA) {
		printf("Cannot determine trap locations.\n");
		return locations;
	}

	// get last 6 moves from Dracula
	bool canFree = 1;
	int numReturnedLocs;
	locations = GvGetLastLocations(dv, PLAYER_DRACULA, TRAIL_SIZE, 
								   &numReturnedLocs, &canFree);
	*numTraps = numReturnedLocs;
	Round round = DvGetRound(dv);
	// scan through last 6 rounds, from earliest to most recent
	int curr = (round - TRAIL_SIZE) * NUM_PLAYERS;
	if (curr < 0) {
		curr = 0;
	}
	// array of locations to remove 
	char remLoc[TRAIL_SIZE][3]; 
	int i = 0;
	for (; curr < dv->num; curr++) {
		// if trap encountered by Hunter
		if (dv->Path[curr][3] == 'T' &&
			dv->Path[curr][0] != 'D') {
			remLoc[i][0] = dv->Path[curr][1];
			remLoc[i][1] = dv->Path[curr][2];
			remLoc[i][2] = '\0';
			i++;
		}
		if (dv->Path[curr][4] == 'T' &&
			dv->Path[curr][0] != 'D') {
			remLoc[i][0] = dv->Path[curr][1];
			remLoc[i][1] = dv->Path[curr][2];
			remLoc[i][2] = '\0';
			i++;
		}
		if (dv->Path[curr][5] == 'T' &&
			dv->Path[curr][0] != 'D') {
			remLoc[i][0] = dv->Path[curr][1];
			remLoc[i][1] = dv->Path[curr][2];
			remLoc[i][2] = '\0';
			i++;
		}
		if (dv->Path[curr][4] == 'V' &&
			dv->Path[curr][0] == 'D') {
			remLoc[i][0] = dv->Path[curr][1];
			remLoc[i][1] = dv->Path[curr][2];
			remLoc[i][2] = '\0';
			i++;
		}
	}
	int numLocs = numReturnedLocs;
	for (int j = 0; j < i; j++) {
		PlaceId locId = placeAbbrevToId(remLoc[j]);
		for (int k = 0; k < numLocs; k++) {
			if (locId == locations[k]) {
				// remove loc from array
				numLocs -= 1;
				locations[k] = locations[numLocs];
				locations = realloc(locations, sizeof(PlaceId)*numLocs);
				*numTraps -= 1;
				break;
			}
		}
	}
	return locations;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	return NULL;
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
