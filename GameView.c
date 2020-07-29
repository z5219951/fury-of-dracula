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
    new->Path = malloc(sizeof(char *)*(len + 1));
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
	assert(gv != NULL);
	return gv->num / 5;
}

Player GvGetPlayer(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	assert(gv != NULL);
	if (gv->num == 0) return PLAYER_LORD_GODALMING; //game have not started
	switch (gv->Path[gv->num - 1][0]) {
		case 'D': return PLAYER_LORD_GODALMING; break;
		case 'G': return PLAYER_DR_SEWARD; break;
		case 'S': return PLAYER_VAN_HELSING; break;
		case 'H': return PLAYER_MINA_HARKER; break;
		case 'M': return PLAYER_DRACULA; break;
		default :
			//fprintf(stderr, "Couldn't get Player!\n");
			//exit(EXIT_FAILURE);
			return PLAYER_LORD_GODALMING;
	}
}

int GvGetScore(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	assert(gv != NULL);
	if (gv->num == 0) return GAME_START_SCORE;
	int score = GAME_START_SCORE - gv->num / NUM_PLAYERS; 
	//Caculate loss score in DRACULA turn	  
	int Hunter_health[4]; //store hunter's health
	for (int i = 0; i < 4; i++) // Initialize the hunter's health
	Hunter_health[i] = GAME_START_HUNTER_LIFE_POINTS;  
	for (int i = 0; i < gv->num; i++) {
		if (i % 5 == PLAYER_DRACULA) {
			// Caculate loss score when Vampire mature
			if (gv->Path[i][5] == 'V') 
			score -= SCORE_LOSS_VAMPIRE_MATURES;
		} else {
			Hunter_health[i % 5]
			= GvGetRoundHealth(gv, i % 5, Hunter_health[i % 5], i);
			// Cacualte loss score when hunter are in hospital
			if (Hunter_health[i % 5] <= 0) {
				score -= SCORE_LOSS_HUNTER_HOSPITAL;
			}
		}
	}
	return score;
}

int GvGetHealth(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	assert(gv != NULL);
	int health = 0;
	// Intialize the health value
	if (player == PLAYER_DRACULA) health = GAME_START_BLOOD_POINTS;
	else health = GAME_START_HUNTER_LIFE_POINTS;
	for (int i = player; i < gv->num; i += NUM_PLAYERS) {
		// recovre the hunter's health
		if (health < 0 && player != PLAYER_DRACULA) 
		health = GAME_START_HUNTER_LIFE_POINTS;
		health = GvGetRoundHealth(gv, player, health, i);
	}
	// return health when the hunter in hospital
	if (health <= 0 && player != PLAYER_DRACULA) return 0;

	return health;
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// check gv is not NULL
	if (gv == NULL || gv->num == 0) {
		return NOWHERE;
	}
	int health = GvGetHealth(gv, player);
	if (health == 0) {
		return HOSPITAL_PLACE;
	}
	int numReturnedLocs = 1; 
	bool canFree = 1;
	PlaceId *result; 
	result = GvGetLastLocations(gv, player, 1, &numReturnedLocs, &canFree); 
	if (numReturnedLocs == 0) { // if player has not had a turn yet
		return NOWHERE; 
	}
	PlaceId location = *result;
	if (canFree == 1) { // free array
		free(result);
	}
	return location;
}

PlaceId GvGetVampireLocation(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// check gv is not NULL
	assert(gv != NULL);
	if (gv->num == 0) {
		return NOWHERE;
	}
	bool canFree = 1;
	int numReturnedLocs;
	PlaceId *locations = GvGetLastLocations(gv, PLAYER_DRACULA, TRAIL_SIZE, 
								   &numReturnedLocs, &canFree);
	if (numReturnedLocs == 0) {
		return NOWHERE;
	}
	// get current round
	Round round = GvGetRound(gv);
	// scan through last 6 rounds, from earliest to most recent
	int curr = (round - TRAIL_SIZE) * NUM_PLAYERS;
	if (curr < 0) {
		curr = 0;
	}
	int vampLoc = -1;
	for (int counter = 1; curr < gv->num; curr++, counter++) {
		if (gv->Path[curr][0] == 'D' && 
			gv->Path[curr][4] == 'V') {
			vampLoc = counter / 5;
		} else if (gv->Path[curr][3] == 'V' ||
				   gv->Path[curr][4] == 'V' ||
				   gv->Path[curr][5] == 'V') { 
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
	int player = GvGetPlayer(gv);
	// if not Dracula's turn
	if (player != PLAYER_DRACULA) {
		printf("Cannot determine trap locations.\n");
		return locations;
	}
	// get last 6 moves from Dracula
	bool canFree = 1;
	static int numReturnedLocs;
	locations = GvGetLastLocations(gv, PLAYER_DRACULA, TRAIL_SIZE, 
								   &numReturnedLocs, &canFree);
	*numTraps = numReturnedLocs;
	Round round = GvGetRound(gv);
	// scan through last 6 rounds, from earliest to most recent
	int curr = (round - TRAIL_SIZE) * NUM_PLAYERS;
	if (curr < 0) {
		curr = 0;
	}
	// array of locations to remove 
	PlaceId *remLoc = malloc(sizeof(PlaceId)*TRAIL_SIZE); 
	char abbrevLoc[1][3];
	int i = 0;
	for (; curr < gv->num; curr++) {
		// if trap encountered by Hunter
		if (gv->Path[curr][3] == 'T' &&
			gv->Path[curr][0] != 'D') {
			abbrevLoc[1][0] = gv->Path[curr][1];
			abbrevLoc[1][1] = gv->Path[curr][2];	
			abbrevLoc[1][2] = '\0';
			remLoc[i] = placeAbbrevToId(abbrevLoc[1]);
			i++;
		}
		if (gv->Path[curr][4] == 'T' &&
			gv->Path[curr][0] != 'D') {
			abbrevLoc[1][0] = gv->Path[curr][1];
			abbrevLoc[1][1] = gv->Path[curr][2];	
			abbrevLoc[1][2] = '\0';
			remLoc[i] = placeAbbrevToId(abbrevLoc[1]);
			i++;
		}
		if (gv->Path[curr][5] == 'T' &&
			gv->Path[curr][0] != 'D') {
			abbrevLoc[1][0] = gv->Path[curr][1];
			abbrevLoc[1][1] = gv->Path[curr][2];	
			abbrevLoc[1][2] = '\0';
			remLoc[i] = placeAbbrevToId(abbrevLoc[1]);
			i++;
		}
		if (gv->Path[curr][4] == 'V' &&
			gv->Path[curr][0] == 'D') {
			// remove loc from array
			int roundCount = (curr / 5) - (round - numReturnedLocs);
			locations[roundCount] = locations[*numTraps-1];
			locations = realloc(locations, sizeof(PlaceId)*(*numTraps-1));
			*numTraps -= 1;
		}
		else if (gv->Path[curr][3] == '.' &&
				 gv->Path[curr][0] == 'D') {
			// remove loc from array
			int roundCount = curr / 5 - (round - numReturnedLocs);
			locations[roundCount] = locations[*numTraps-1];
			locations = realloc(locations, sizeof(PlaceId)*(*numTraps-1));
			*numTraps -= 1;
		} 
		else if (gv->Path[curr][0] == 'D') { // Dracula is at sea
			abbrevLoc[1][0] = gv->Path[curr][1];
			abbrevLoc[1][1] = gv->Path[curr][2];	
			abbrevLoc[1][2] = '\0';
			PlaceId atSea = placeAbbrevToId(abbrevLoc[1]);
			if (placeIsSea(atSea)) {
				// remove loc from array
				int roundCount = curr / 5 - (round - numReturnedLocs);
				locations[roundCount] = locations[*numTraps-1];
				locations = realloc(locations, sizeof(PlaceId)*(*numTraps-1));
				*numTraps -= 1;
				
			}
		}
	}
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
int GvGetRoundHealth(GameView gv, Player player, int health, int round) {
	assert(gv != NULL);
	// Convert the palce in path char
	char place1[3];
	place1[0] = gv->Path[round][1];
	place1[1] = gv->Path[round][2];
	place1[2] = '\0';
	// Caculate the health value of Dracula
	if (player != PLAYER_DRACULA) {
		// Encounter the trap or Vampire
		for (int j = 3; j < 7; j++) {
			if (gv->Path[round][j] == 'T') health -= LIFE_LOSS_TRAP_ENCOUNTER;
			if (gv->Path[round][j] == 'D') health -= LIFE_LOSS_DRACULA_ENCOUNTER;
		}
		if (round >= NUM_PLAYERS){
			char place2[8];
			place2[0] = gv->Path[round - NUM_PLAYERS][1];
			place2[1] = gv->Path[round - NUM_PLAYERS][2];
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
			GvGetLastLocations(gv, player, backmove, false);
			place1 = placeIdToAbbrev(move[backmove])*/
			backmove *= NUM_PLAYERS;
			backmove = round - backmove;
			place1[0] = gv->Path[backmove][1];
			place1[1] = gv->Path[backmove][2];
			place1[2] = '\0'; 
		}
		// Caculate the health when encounter the hunter
		for (int j = round + 1; j < gv->num 
		&& j < round + NUM_PLAYERS; j++) {
			char place2[3];
			place2[0] = gv->Path[j][1];
			place2[1] = gv->Path[j][2];
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
// Check double back move
bool DnumCheck (PlaceId PId) {
	return (PId >= DOUBLE_BACK_1 && PId <= DOUBLE_BACK_5);	
}

// Check places are same
bool PlaceCmp(char *place1, char *place2) {
	return (placeAbbrevToId(place1) == placeAbbrevToId(place2));	 
}

// TODO
