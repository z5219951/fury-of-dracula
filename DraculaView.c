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

// change dracula adt into gameview adt
static GameView draculaToGame(DraculaView dv);

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
	assert(dv != NULL);
	return dv->num / 5;
}

int DvGetScore(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	assert(dv != NULL);
	if (dv->num == 0) return GAME_START_SCORE;
	int score = GAME_START_SCORE - dv->num / NUM_PLAYERS; 
	//Caculate loss score in DRACULA turn	  
	int Hunter_health[4]; //store hunter's health
	for (int i = 0; i < 4; i++) // Initialize the hunter's health
	Hunter_health[i] = GAME_START_HUNTER_LIFE_POINTS;  
	for (int i = 0; i < dv->num; i++) {
		if (i % 5 == PLAYER_DRACULA) {
			// Caculate loss score when Vampire mature
			if (dv->Path[i][5] == 'V') 
			score -= SCORE_LOSS_VAMPIRE_MATURES;
		} else {
			Hunter_health[i % 5]
			= dvGetRoundHealth(dv, i % 5, Hunter_health[i % 5], i);
			// Cacualte loss score when hunter are in hospital
			if (Hunter_health[i % 5] <= 0) {
				score -= SCORE_LOSS_HUNTER_HOSPITAL;
			}
		}
	}
	return score;
}

int DvGetHealth(DraculaView dv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	assert(dv != NULL);
	int health = 0;
	// Intialize the health value
	if (player == PLAYER_DRACULA) health = GAME_START_BLOOD_POINTS;
	else health = GAME_START_HUNTER_LIFE_POINTS;
	for (int i = player; i < dv->num; i += NUM_PLAYERS) {
		// recovre the hunter's health
		if (health < 0 && player != PLAYER_DRACULA) 
		health = GAME_START_HUNTER_LIFE_POINTS;
		health = dvGetRoundHealth(dv, player, health, i);
	}
	// return health when the hunter in hospital
	if (health <= 0 && player != PLAYER_DRACULA) return 0;

	return health;
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// check dv is not NULL
	assert(dv != NULL);
	int health = DvGetHealth(dv, player);
	if (health == 0) {
		return HOSPITAL_PLACE;
	}
	int numReturnedLocs = 1; 
	bool canFree = 1;
	PlaceId *result; 
	GameView trans = draculaToGame(dv);
	result = GvGetLastLocations(trans, player, 1, &numReturnedLocs, &canFree); 
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
	assert(dv != NULL);
	if (dv->num < 5) {
		return NOWHERE;
	}
	bool canFree = 1;
	int numReturnedLocs;
	GameView trans = draculaToGame(dv);
	PlaceId *locations = GvGetLastLocations(trans, PLAYER_DRACULA, TRAIL_SIZE, 
								   &numReturnedLocs, &canFree);
	if (numReturnedLocs == 0) {
		return NOWHERE;
	}
	Round round = DvGetRound(dv);
	// scan through last 6 rounds, from earliest to most recent
	int curr = (round - TRAIL_SIZE) * NUM_PLAYERS;
	if (curr < 0) {
		curr = 0;
	}
	int vampLoc = -1;
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
	if (vampLoc == -1) {
		return NOWHERE;
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
	PlaceId *locations = malloc(sizeof(PlaceId)*TRAIL_SIZE); 
	// get last 6 Dracula locations
	bool canFree = 1;
	int numReturnedLocs;
	GameView trans = draculaToGame(dv);
	locations = GvGetLastLocations(trans, PLAYER_DRACULA, TRAIL_SIZE, 
								   &numReturnedLocs, &canFree);
	*numTraps = numReturnedLocs;
	Round round = DvGetRound(dv);
	// scan through last 6 rounds, from earliest to most recent
	int curr = (round - TRAIL_SIZE) * NUM_PLAYERS;
	if (curr < 0) {
		curr = 0;
	}
	// array of locations to remove 
	PlaceId *remLoc = malloc(sizeof(PlaceId)*TRAIL_SIZE); 
	char abbrevLoc[1][3];
	int i = 0;
	for (; curr < dv->num; curr++) {
		if (dv->Path[curr][3] == 'T' &&
			dv->Path[curr][0] != 'D') { // trap encountered by Hunter
			abbrevLoc[1][0] = dv->Path[curr][1];
			abbrevLoc[1][1] = dv->Path[curr][2];	
			abbrevLoc[1][2] = '\0';
			remLoc[i] = placeAbbrevToId(abbrevLoc[1]);
			i++;
		}
		if (dv->Path[curr][4] == 'T' &&
			dv->Path[curr][0] != 'D') {
			abbrevLoc[1][0] = dv->Path[curr][1];
			abbrevLoc[1][1] = dv->Path[curr][2];	
			abbrevLoc[1][2] = '\0';
			remLoc[i] = placeAbbrevToId(abbrevLoc[1]);
			i++;
		}
		if (dv->Path[curr][5] == 'T' &&
			dv->Path[curr][0] != 'D') {
			abbrevLoc[1][0] = dv->Path[curr][1];
			abbrevLoc[1][1] = dv->Path[curr][2];	
			abbrevLoc[1][2] = '\0';
			remLoc[i] = placeAbbrevToId(abbrevLoc[1]);
			i++;
		}
		if (dv->Path[curr][4] == 'V' &&
			dv->Path[curr][0] == 'D') { // loc of immature vampire
			int roundCount = (curr / 5) - (round - numReturnedLocs);
			locations[roundCount] = locations[*numTraps-1];
			locations = realloc(locations, sizeof(PlaceId)*(*numTraps-1));
			*numTraps -= 1;
		}
		else if (dv->Path[curr][3] == '.' &&
				 dv->Path[curr][0] == 'D') { // no trap placed
			int roundCount = curr / 5 - (round - numReturnedLocs);
			locations[roundCount] = locations[*numTraps-1];
			locations = realloc(locations, sizeof(PlaceId)*(*numTraps-1));
			*numTraps -= 1;
		} 
		else if (dv->Path[curr][0] == 'D') {
			abbrevLoc[1][0] = dv->Path[curr][1];
			abbrevLoc[1][1] = dv->Path[curr][2];	
			abbrevLoc[1][2] = '\0';
			PlaceId atSea = placeAbbrevToId(abbrevLoc[1]);
			if (placeIsSea(atSea)) {  // Dracula is at sea
				int roundCount = curr / 5 - (round - numReturnedLocs);
				locations[roundCount] = locations[*numTraps-1];
				locations = realloc(locations, sizeof(PlaceId)*(*numTraps-1));
				*numTraps -= 1;
				
			}
		}
	}
	// remove encountered traps
	for (int j = 0; j < i; j++) {
		for (int k = 0; k < *numTraps; k++) {
			if (remLoc[j] == locations[k]) {
				*numTraps -= 1;
				locations[k] = locations[*numTraps];
				locations = realloc(locations, sizeof(PlaceId)*(*numTraps));
				break;
			}
		}
	}
	free(remLoc);
	return locations;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Player currPlayer = (dv->num) % 5;
	int round = (dv->num) / 5;
	if (currPlayer > PLAYER_DRACULA) {
		round++;
	}
	PlaceId from = DvGetPlayerLocation(dv, PLAYER_DRACULA);
	if (from == NOWHERE) {
		*numReturnedMoves = 0;
		return NULL;
	}
	int n = MapNumPlaces(dv->map);
	int *repeated_city = calloc(n, sizeof(int));
	int counter = 0;
	ConnList curr = MapGetConnections(dv->map, from);
	while (curr != NULL) {
		counter++;
		curr = curr->next;
	}
	PlaceId *result = malloc(sizeof(PlaceId) * (counter + 1));
	char Past5Move[5][3];
	if (round < 5) {
		for (int i = 0, j = 4; i < round; i++, j+=5) {
			Past5Move[i][0] = dv->Path[j][1];
			Past5Move[i][1] = dv->Path[j][2];
			Past5Move[i][2] = '\0';
		}
	} else {
		for (int i = 0, j = round - 5; i < 5; i++, j++) {
			Past5Move[i][0] = dv->Path[4 + j * 5][1];
			Past5Move[i][1] = dv->Path[4 + j * 5][2];
			Past5Move[i][2] = '\0';
		}
	}
	curr = MapGetConnections(dv->map, from);
	if (from == HIDE || from == DOUBLE_BACK_1) {
		if (round > 1) {
			char Last2Move[1][3];
			Last2Move[0][0] = dv->Path[5 * round - 6][1];
			Last2Move[0][1] = dv->Path[5 * round - 6][2];
			Last2Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last2Move[0]));
		}
	}
	else if (from == DOUBLE_BACK_2) {
		if (round > 2) {
			char Last3Move[1][3];
			Last3Move[0][0] = dv->Path[5 * round - 11][1];
			Last3Move[0][1] = dv->Path[5 * round - 11][2];
			Last3Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last3Move[0]));
		}
	}
	else if (from == DOUBLE_BACK_3) {
		if (round > 3) {
			char Last4Move[1][3];
			Last4Move[0][0] = dv->Path[5 * round - 16][1];
			Last4Move[0][1] = dv->Path[5 * round - 16][2];
			Last4Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last4Move[0]));
		}
	}
	else if (from == DOUBLE_BACK_4) {
		if (round > 4) {
			char Last5Move[1][3];
			Last5Move[0][0] = dv->Path[5 * round - 21][1];
			Last5Move[0][1] = dv->Path[5 * round - 21][2];
			Last5Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last5Move[0]));
		}
	}
	else if (from == DOUBLE_BACK_5) {
		if (round > 5) {
			char Last6Move[1][3];
			Last6Move[0][0] = dv->Path[5 * round - 26][1];
			Last6Move[0][1] = dv->Path[5 * round - 26][2];
			Last6Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last6Move[0]));
		}
	}

	int index = 0;
	int round_temp;
	bool hasRepeatedHide = false;
	bool hasRepeatedDB = false;
	// Check whether a connected city satisfy condition
	while (curr != NULL) {
		// Dracula can not move to hospital
		if (curr->p == ST_JOSEPH_AND_ST_MARY) {
			curr = curr->next;
			continue;
		}
		// Dracula can not travel through rail
		if (curr->type == RAIL) {
			curr = curr->next;
			continue;
		}

		bool hasRepeatedMove = false;

		if (round < 5) {
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
				hasRepeatedHide = true;
			}
			if (curr_ID == DOUBLE_BACK_1 || curr_ID == DOUBLE_BACK_2 
				|| curr_ID == DOUBLE_BACK_3 || curr_ID == DOUBLE_BACK_4 
				|| curr_ID == DOUBLE_BACK_5) {
				hasRepeatedDB = true;
			}
		}
		// Skip to next adjacent city if dracula made the same move in the past 5 rounds
		if (hasRepeatedMove) {
			curr=curr->next;
			continue;
		}
		// If current place satisfy condition
		if ((curr->type == ROAD || curr->type == BOAT) && repeated_city[curr->p] == 0)  {
			result[index++] = curr->p;
			result[index] = 999;
			repeated_city[curr->p] = 1;
		}
		curr = curr->next;
	}
	// If dracula did not made hide move in the past 5 rounds
	if (!hasRepeatedHide) {
		if (!placeIsSea(from)) {
			result[index++] = HIDE;
			result[index] = 999;
		}
	} 
	// If dracula did not made double back in the past 5 rounds
	if (!hasRepeatedDB) {
		if (round > 0) result[index++] = DOUBLE_BACK_1;
		if (round > 1) result[index++] = DOUBLE_BACK_2;
		if (round > 2) result[index++] = DOUBLE_BACK_3;
		if (round > 3) result[index++] = DOUBLE_BACK_4;
		if (round > 4) result[index++] = DOUBLE_BACK_5;
		result[index] = 999;
	}
	// If there is no legal move for dracula
	if (index == 0) {
		*numReturnedMoves = 0;
		return NULL;
	}
	*numReturnedMoves = GetLenOfList(result);
	return result;
	
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Player currPlayer = (dv->num) % 5;
	int round = (dv->num) / 5;
	if (currPlayer > PLAYER_DRACULA) {
		round++;
	}
	PlaceId from = DvGetPlayerLocation(dv, PLAYER_DRACULA);
	if (from == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}
	if (from == HIDE || from == DOUBLE_BACK_1) {
		if (round > 1) {
			char Last2Move[1][3];
			Last2Move[0][0] = dv->Path[5 * round - 6][1];
			Last2Move[0][1] = dv->Path[5 * round - 6][2];
			Last2Move[0][2] = '\0';
			from = placeAbbrevToId(Last2Move[0]);
		}
	}
	else if (from == DOUBLE_BACK_2) {
		if (round > 2) {
			char Last3Move[1][3];
			Last3Move[0][0] = dv->Path[5 * round - 11][1];
			Last3Move[0][1] = dv->Path[5 * round - 11][2];
			Last3Move[0][2] = '\0';
			from = placeAbbrevToId(Last3Move[0]);
		}
	}
	else if (from == DOUBLE_BACK_3) {
		if (round > 3) {
			char Last4Move[1][3];
			Last4Move[0][0] = dv->Path[5 * round - 16][1];
			Last4Move[0][1] = dv->Path[5 * round - 16][2];
			Last4Move[0][2] = '\0';
			from = placeAbbrevToId(Last4Move[0]);
		}
	}
	else if (from == DOUBLE_BACK_4) {
		if (round > 4) {
			char Last5Move[1][3];
			Last5Move[0][0] = dv->Path[5 * round - 21][1];
			Last5Move[0][1] = dv->Path[5 * round - 21][2];
			Last5Move[0][2] = '\0';
			from = placeAbbrevToId(Last5Move[0]);
		}
	}
	else if (from == DOUBLE_BACK_5) {
		if (round > 5) {
			char Last6Move[1][3];
			Last6Move[0][0] = dv->Path[5 * round - 26][1];
			Last6Move[0][1] = dv->Path[5 * round - 26][2];
			Last6Move[0][2] = '\0';
			from = placeAbbrevToId(Last6Move[0]);
		}
	}

	// The following array can avoid the same city be added
	int n = MapNumPlaces(dv->map);
	int *repeated_city = calloc(n, sizeof(int));
	// Counter is the number of conncection
	int counter = 0;
	ConnList curr = MapGetConnections(dv->map, from);
	while (curr != NULL) {
		counter++;
		curr = curr->next;
	}
	PlaceId *result = malloc(sizeof(PlaceId) * (counter + 1));
	// Get past 5 moves of dracula
	char Past5Move[5][3];
	if (round < 6) {
		for (int i = 0, j = 4; i < round; i++, j+=5) {
			Past5Move[i][0] = dv->Path[j][1];
			Past5Move[i][1] = dv->Path[j][2];
			Past5Move[i][2] = '\0';
		}
	} else {
		for (int i = 0, j = round - 5; i < 5; i++, j++) {
			Past5Move[i][0] = dv->Path[4 + j * 5][1];
			Past5Move[i][1] = dv->Path[4 + j * 5][2];
			Past5Move[i][2] = '\0';
		}
	}
	curr = MapGetConnections(dv->map, from);
	int index = 0;
	int round_temp;
	// Check whether a adjacent city satisfy condition
	while(curr != NULL) {
		// Dracula can not move to hospital
		if (curr->p == ST_JOSEPH_AND_ST_MARY) {
			curr = curr->next;
			continue;
		}
		// Dracula can not travel through rail
		if (curr->type == RAIL) {
			curr = curr->next;
			continue;
		}
		bool hasRepeatedMove = false;
		if (round < 5) {
			round_temp = round;
		} else {
			round_temp = 5;
		}
		// Check if dracula has made the same move in past 5 round
		for (int i = 0; i < round_temp; i++) {
			if (strcmp(Past5Move[i], placeIdToAbbrev(curr->p)) == 0) {
				hasRepeatedMove = true;
			}
		}
		// Skip to next adjacent place
		if (hasRepeatedMove) {
			curr = curr->next;
			continue;
		}
		// If current adjacent place satisfy condition
		if ((curr->type == ROAD || curr->type == BOAT) && repeated_city[curr->p] == 0)  {
				result[index++] = curr->p;
				result[index] = 999;
				repeated_city[curr->p] = 1;
		}

		curr = curr->next;
	}
	// If there is no valid move for dracula
	if (index == 0) {
		*numReturnedLocs = 0;
		return NULL;
	}
	*numReturnedLocs = GetLenOfList(result);
	return result;
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Player currPlayer = (dv->num) % 5;
	int round = (dv->num) / 5;
	if (currPlayer > PLAYER_DRACULA) {
		round++;
	}
	PlaceId from = DvGetPlayerLocation(dv, PLAYER_DRACULA);
	if (from == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}
	if (from == HIDE || from == DOUBLE_BACK_1) {
		if (round > 1) {
			char Last2Move[1][3];
			Last2Move[0][0] = dv->Path[5 * round - 6][1];
			Last2Move[0][1] = dv->Path[5 * round - 6][2];
			Last2Move[0][2] = '\0';
			from = placeAbbrevToId(Last2Move[0]);
		}
	}
	else if (from == DOUBLE_BACK_2) {
		if (round > 2) {
			char Last3Move[1][3];
			Last3Move[0][0] = dv->Path[5 * round - 11][1];
			Last3Move[0][1] = dv->Path[5 * round - 11][2];
			Last3Move[0][2] = '\0';
			from = placeAbbrevToId(Last3Move[0]);
		}
	}
	else if (from == DOUBLE_BACK_3) {
		if (round > 3) {
			char Last4Move[1][3];
			Last4Move[0][0] = dv->Path[5 * round - 16][1];
			Last4Move[0][1] = dv->Path[5 * round - 16][2];
			Last4Move[0][2] = '\0';
			from = placeAbbrevToId(Last4Move[0]);
		}
	}
	else if (from == DOUBLE_BACK_4) {
		if (round > 4) {
			char Last5Move[1][3];
			Last5Move[0][0] = dv->Path[5 * round - 21][1];
			Last5Move[0][1] = dv->Path[5 * round - 21][2];
			Last5Move[0][2] = '\0';
			from = placeAbbrevToId(Last5Move[0]);
		}
	}
	else if (from == DOUBLE_BACK_5) {
		if (round > 5) {
			char Last6Move[1][3];
			Last6Move[0][0] = dv->Path[5 * round - 26][1];
			Last6Move[0][1] = dv->Path[5 * round - 26][2];
			Last6Move[0][2] = '\0';
			from = placeAbbrevToId(Last6Move[0]);
		}
	}
	// The following array can avoid the same place be added
	int n = MapNumPlaces(dv->map);
	int *repeated_city = calloc(n, sizeof(int));
	// Counter is the number of connection
	int counter = 0;
	ConnList curr = MapGetConnections(dv->map, from);
	while (curr != NULL) {
		counter++;
		curr = curr->next;
	}
	PlaceId *result = malloc(sizeof(PlaceId) * (counter + 1));
	// Get past 5 moves of dracula
	char Past5Move[5][3];
	if (round < 5) {
		for (int i = 0, j = 4; i < round; i++, j+=5) {
			Past5Move[i][0] = dv->Path[j][1];
			Past5Move[i][1] = dv->Path[j][2];
			Past5Move[i][2] = '\0';
		}
	} else {
		for (int i = 0, j = round - 5; i < 5; i++, j++) {
			Past5Move[i][0] = dv->Path[4 + j * 5][1];
			Past5Move[i][1] = dv->Path[4 + j * 5][2];
			Past5Move[i][2] = '\0';
		}
	}

	curr = MapGetConnections(dv->map, from);
	int index = 0;
	int round_temp;
	if (round < 6) {
		round_temp = round;
	} else {
		round_temp = 5;
	}
	// Check if current adjacent city satisfy condition
	while(curr != NULL) {
		// Dracula can not move to hospital
		if (curr->p == ST_JOSEPH_AND_ST_MARY) {
			curr = curr->next;
			continue;
		}
		// Dracula can not travel through rail
		if (curr->type == RAIL) {
			curr = curr->next;
			continue;
		}
		bool hasRepeatedMove = false;
		// Check if dracula has made the same move in past 5 round
		for (int i = 0; i < round_temp; i++) {
			if (strcmp(Past5Move[i], placeIdToAbbrev(curr->p)) == 0) {
				hasRepeatedMove = true;
			}
		}
		// Skip to next adjacent place
		if (hasRepeatedMove) {
			curr = curr->next;
			continue;
		}
		// If current place satisfy conditon
		if (curr->type == ROAD  && road == true && repeated_city[curr->p] == 0) {
			result[index++] = curr->p;
			result[index] = 999;
			repeated_city[curr->p] = 1;

		} else if (curr->type == BOAT && boat == true && repeated_city[curr->p] == 0) {
			result[index++] = curr->p;
			result[index] = 999;
			repeated_city[curr->p] = 1;
		}

		curr = curr->next;
	}
	// If there is no valid move for dracula
	if (index == 0) {
		*numReturnedLocs = 0;
		return NULL;
	}
	*numReturnedLocs = GetLenOfList(result);
	return result;
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION	
	Player currPlayer = (dv->num) % 5;
	int round = (dv->num) / 5;
	if (currPlayer > player) {
		round++;
	}
	PlaceId from;
	int n = MapNumPlaces(dv->map);
	// Two cities may be connected through multiple ways
	// The following array is to avoid the same city be added
	int *repeated_city = calloc(n, sizeof(int));
	// If current player is dracula
	if (player == PLAYER_DRACULA) {
		from = DvGetPlayerLocation(dv, PLAYER_DRACULA);
		if (from == HIDE || from == DOUBLE_BACK_1) {
			if (round > 1) {
				char Last2Move[1][3];
				Last2Move[0][0] = dv->Path[5 * round - 6][1];
				Last2Move[0][1] = dv->Path[5 * round - 6][2];
				Last2Move[0][2] = '\0';
				from = placeAbbrevToId(Last2Move[0]);
			}
		}
		else if (from == DOUBLE_BACK_2) {
			if (round > 2) {
				char Last3Move[1][3];
				Last3Move[0][0] = dv->Path[5 * round - 11][1];
				Last3Move[0][1] = dv->Path[5 * round - 11][2];
				Last3Move[0][2] = '\0';
				from = placeAbbrevToId(Last3Move[0]);
			}
		}
		else if (from == DOUBLE_BACK_3) {
			if (round > 3) {
				char Last4Move[1][3];
				Last4Move[0][0] = dv->Path[5 * round - 16][1];
				Last4Move[0][1] = dv->Path[5 * round - 16][2];
				Last4Move[0][2] = '\0';
				from = placeAbbrevToId(Last4Move[0]);
			}
		}
		else if (from == DOUBLE_BACK_4) {
			if (round > 4) {
				char Last5Move[1][3];
				Last5Move[0][0] = dv->Path[5 * round - 21][1];
				Last5Move[0][1] = dv->Path[5 * round - 21][2];
				Last5Move[0][2] = '\0';
				from = placeAbbrevToId(Last5Move[0]);
			}
		}
		else if (from == DOUBLE_BACK_5) {
			if (round > 5) {
				char Last6Move[1][3];
				Last6Move[0][0] = dv->Path[5 * round - 26][1];
				Last6Move[0][1] = dv->Path[5 * round - 26][2];
				Last6Move[0][2] = '\0';
				from = placeAbbrevToId(Last6Move[0]);
			}
		}
		// Counter is the number of connection
		int counter = 0;
		ConnList curr = MapGetConnections(dv->map, from);
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * (counter + 1));
		// Get past 5 moves of dracula
		char Past5Move[5][3];
		if (round < 6) {
			for (int i = 0, j = 4; i < round; i++, j+=5) {
				Past5Move[i][0] = dv->Path[j][1];
				Past5Move[i][1] = dv->Path[j][2];
				Past5Move[i][2] = '\0';
			}
		} else {
			for (int i = 0, j = round - 5; i < 5; i++, j++) {
				Past5Move[i][0] = dv->Path[4 + j * 5][1];
				Past5Move[i][1] = dv->Path[4 + j * 5][2];
				Past5Move[i][2] = '\0';
			}
		}

		curr = MapGetConnections(dv->map, from);
		int index = 0;
		int round_temp;
		if (round < 5) {
			round_temp = round;
		} else {
			round_temp = 5;
		}
		// Check if current adjacent place satisfy condition
		while(curr != NULL) {
		// Dracula can not move to hospital
		if (curr->p == ST_JOSEPH_AND_ST_MARY) {
			curr = curr->next;
			continue;
		}
		// Dracula can not travel through rail
		if (curr->type == RAIL) {
			curr = curr->next;
			continue;
		}
		bool hasRepeatedMove = false;
		// Check if dracula has made the same move in past 5 round
		for (int i = 0; i < round_temp; i++) {
			if (strcmp(Past5Move[i], placeIdToAbbrev(curr->p)) == 0) {
				hasRepeatedMove = true;
			}
		}
		// Skip to next adjacent place
		if (hasRepeatedMove) {
			curr = curr->next;
			continue;
		}
		// If current place satisfy condition
		if ((curr->type == ROAD || curr->type == BOAT) && repeated_city[curr->p] == 0)  {
			result[index++] = curr->p;
			result[index] = 999;
			repeated_city[curr->p] = 1;
		}

		curr = curr->next;
		}
	// If there is no valid move for dracula
	if (index == 0) {
		*numReturnedLocs = 0;
		return NULL;
	}
	*numReturnedLocs = GetLenOfList(result);
	return result;
	// If current player is hunter
	} else {
		if (player == PLAYER_LORD_GODALMING) {
			from = DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING);
		} 
		else if (player == PLAYER_DR_SEWARD) {
			from = DvGetPlayerLocation(dv, PLAYER_DR_SEWARD);
		} 
		else if (player == PLAYER_VAN_HELSING) {
			from = DvGetPlayerLocation(dv, PLAYER_VAN_HELSING);
		} 
		else if (player == PLAYER_MINA_HARKER) {
			from = DvGetPlayerLocation(dv, PLAYER_MINA_HARKER);
		}
		if (from == NOWHERE) {
			*numReturnedLocs = 0;
			return NULL;
		}
		ConnList curr = MapGetConnections(dv->map, from);
		// Counter is the number of connection
		int counter = 0;
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * (counter + 1));
		curr = MapGetConnections(dv->map, from);
		int index = 0;
		int max_distance = round % 4;
		// Get the places connected through rail
		PlaceId *RailList = GetConnRail(dv->map, from, max_distance, repeated_city);
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
			if (index == 0) {
				*numReturnedLocs = GetLenOfList(RailList);
				return RailList;
			}
			result = MergeList(result, RailList);
		}
		*numReturnedLocs = GetLenOfList(result);
		return result;
	}
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Player currPlayer = (dv->num) % 5;
	int round = (dv->num) / 5;
	if (currPlayer > player) {
		round++;
	}
	PlaceId from;
	int n = MapNumPlaces(dv->map);
	int *repeated_city = calloc(n, sizeof(int));
	if (player == PLAYER_DRACULA) {
		from = DvGetPlayerLocation(dv, PLAYER_DRACULA);
		if (from == NOWHERE) {
			*numReturnedLocs = 0;
			return NULL;
		}
		if (from == HIDE || from == DOUBLE_BACK_1) {
			if (round > 1) {
				char Last2Move[1][3];
				Last2Move[0][0] = dv->Path[5 * round - 6][1];
				Last2Move[0][1] = dv->Path[5 * round - 6][2];
				Last2Move[0][2] = '\0';
				from = placeAbbrevToId(Last2Move[0]);
			}
		}
		else if (from == DOUBLE_BACK_2) {
			if (round > 2) {
				char Last3Move[1][3];
				Last3Move[0][0] = dv->Path[5 * round - 11][1];
				Last3Move[0][1] = dv->Path[5 * round - 11][2];
				Last3Move[0][2] = '\0';
				from = placeAbbrevToId(Last3Move[0]);
			}
		}
		else if (from == DOUBLE_BACK_3) {
			if (round > 3) {
				char Last4Move[1][3];
				Last4Move[0][0] = dv->Path[5 * round - 16][1];
				Last4Move[0][1] = dv->Path[5 * round - 16][2];
				Last4Move[0][2] = '\0';
				from = placeAbbrevToId(Last4Move[0]);
			}
		}
		else if (from == DOUBLE_BACK_4) {
			if (round > 5) {
				char Last5Move[1][3];
				Last5Move[0][0] = dv->Path[5 * round - 21][1];
				Last5Move[0][1] = dv->Path[5 * round - 21][2];
				Last5Move[0][2] = '\0';
				from = placeAbbrevToId(Last5Move[0]);
			}
		}
		else if (from == DOUBLE_BACK_5) {
			if (round > 6) {
				char Last6Move[1][3];
				Last6Move[0][0] = dv->Path[5 * round - 26][1];
				Last6Move[0][1] = dv->Path[5 * round - 26][2];
				Last6Move[0][2] = '\0';
				from = placeAbbrevToId(Last6Move[0]);
			}
		}
		int counter = 0;
		ConnList curr = MapGetConnections(dv->map, from);
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * (counter + 1));
		char Past5Move[5][3];
		if (round < 5) {
			for (int i = 0, j = 4; i < round; i++, j+=5) {
				Past5Move[i][0] = dv->Path[j][1];
				Past5Move[i][1] = dv->Path[j][2];
				Past5Move[i][2] = '\0';
			}
		} else {
			for (int i = 0, j = round - 5; i < 5; i++, j++) {
				Past5Move[i][0] = dv->Path[4 + j * 5][1];
				Past5Move[i][1] = dv->Path[4 + j * 5][2];
				Past5Move[i][2] = '\0';
			}
		}

		curr = MapGetConnections(dv->map, from);
		int index = 0;
		int round_temp;
		if (round < 5) {
			round_temp = round;
		} else {
			round_temp = 5;
		}
		curr = MapGetConnections(dv->map, from);
		while(curr != NULL) {
			if (curr->type == RAIL 
			|| repeated_city[curr->p] == 1 
			|| curr->p == ST_JOSEPH_AND_ST_MARY) {
				curr = curr->next;
				continue;
			}
			bool hasRepeatedMove = false;
			// Check if dracula has made the same move in past 5 round
			for (int i = 0; i < round_temp; i++) {
				if (strcmp(Past5Move[i], placeIdToAbbrev(curr->p)) == 0) {
					hasRepeatedMove = true;
				}
			}
			// Skip to next adjacent place
			if (hasRepeatedMove) {
				curr = curr->next;
				continue;
			}
			// If current place satisfy condition
			if (curr->type == ROAD && road == true) {
				result[index++] = curr->p;
				result[index] = 999;
				repeated_city[curr->p] = 1;
			}
			else if (curr->type == BOAT && boat == true) {
				result[index++] = curr->p;
				result[index] = 999;
				repeated_city[curr->p] = 1;
			}
			curr = curr->next;
		}
		// If there is no valid move for dracula
		if (index == 0) {
				*numReturnedLocs = 0;
		return NULL;
		}
		*numReturnedLocs = GetLenOfList(result);
		return result;
		// If current player is hunter
	} else {
		if (player == PLAYER_LORD_GODALMING) {
			from = DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING);
		} 
		else if (player == PLAYER_DR_SEWARD) {
			from = DvGetPlayerLocation(dv, PLAYER_DR_SEWARD);
		} 
		else if (player == PLAYER_VAN_HELSING) {
			from = DvGetPlayerLocation(dv, PLAYER_VAN_HELSING);
		} 
		else if (player == PLAYER_MINA_HARKER) {
			from = DvGetPlayerLocation(dv, PLAYER_MINA_HARKER);
		}
		if (from == NOWHERE) {
			*numReturnedLocs = 0;
			return NULL;
		}
		ConnList curr = MapGetConnections(dv->map, from);
		int counter = 0;
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * (counter + 1));
		int index = 0;
		int max_distance = round % 4;
		// Get the places connected through rail
		PlaceId *RailList = NULL;
		if (rail == true) {
			RailList = GetConnRail(dv->map, from, max_distance, repeated_city);
		}
		curr = MapGetConnections(dv->map, from);
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
			if (curr->type == BOAT && boat == true) {
				// From sea to sea or port city
				if (placeIsSea(from)) {
					result[index++] = curr->p;
					result[index] = 999;
					repeated_city[curr->p] = 1;
				}
				// From port city to adjacent sea
				if (placeIsLand(from) && placeIsSea(curr->p)) {
					result[index++] = curr->p;
					result[index] = 999;
					repeated_city[curr->p] = 1;
				}
			}
			curr = curr->next;
		}
		if (RailList != NULL && rail == true) {
			if (index == 0) {
				*numReturnedLocs = GetLenOfList(RailList);
				return RailList;
			}
			result = MergeList(result, RailList);
		}
		*numReturnedLocs = GetLenOfList(result);
		return result;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions
int dvGetRoundHealth(DraculaView dv, Player player, int health, int round) {
	assert(dv != NULL);
	// Convert the palce in path char
	char place1[3];
	place1[0] = dv->Path[round][1];
	place1[1] = dv->Path[round][2];
	place1[2] = '\0';
	// Caculate the health value of Dracula
	if (player != PLAYER_DRACULA) {
		// Encounter the trap or Vampire
		for (int j = 3; j < 7; j++) {
			if (dv->Path[round][j] == 'T') health -= LIFE_LOSS_TRAP_ENCOUNTER;
			if (dv->Path[round][j] == 'D') health -= LIFE_LOSS_DRACULA_ENCOUNTER;
		}
		if (round >= NUM_PLAYERS){
			char place2[8];
			place2[0] = dv->Path[round - NUM_PLAYERS][1];
			place2[1] = dv->Path[round - NUM_PLAYERS][2];
			place2[2] = '\0';
			if (PlaceCmp(place1,place2))
			health += LIFE_GAIN_REST;
			if (health > GAME_START_HUNTER_LIFE_POINTS)
			health = GAME_START_HUNTER_LIFE_POINTS;
		}
	}
	// Caculate the health value of Hunter
	if (player == PLAYER_DRACULA) {
		// Double back move check
		if (DnumCheck(placeAbbrevToId(place1))) {
			int backmove = placeAbbrevToId(place1) - DOUBLE_BACK_1 + 1;
			/*PlaceId *move =
			dvGetLastLocations(dv, player, backmove, false);
			place1 = placeIdToAbbrev(move[backmove])*/
			backmove *= NUM_PLAYERS;
			backmove = round - backmove;
			place1[0] = dv->Path[backmove][1];
			place1[1] = dv->Path[backmove][2];
			place1[2] = '\0'; 
		}
		// Caculate the health when encounter the hunter
		for (int j = round + 1; j < dv->num 
		&& j < round + NUM_PLAYERS; j++) {
			char place2[3];
			place2[0] = dv->Path[j][1];
			place2[1] = dv->Path[j][2];
			place2[2] = '\0';
			if (PlaceCmp(place2, place1))
			health -= LIFE_LOSS_HUNTER_ENCOUNTER;
		}
		if (placeIsSea(placeAbbrevToId(place1))) {
			health -= LIFE_LOSS_SEA;
		}
		//Gain the health in Castle Dracula
		if (PlaceCmp(place1, "TP") || PlaceCmp(place1, "CD"))
		health += LIFE_GAIN_CASTLE_DRACULA;
	}
	return health;
}

// help function
// change hunter adt into gameview adt
static GameView draculaToGame(DraculaView dv) {
	Message messages[] = {};
	char *originPast = malloc(8*dv->num);
	for (int i = 0; i < dv->num; i++) {
		strcat(originPast, dv->Path[i]);
		if (i != dv->num-1) {
			strcat(originPast, " ");
		}
	}
	return GvNew(originPast, messages);
}

// TODO
// get the number of element in an array

