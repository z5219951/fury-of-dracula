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
	PlaceId *locations = malloc(sizeof(PlaceId)*TRAIL_SIZE); 
	int player = GvGetPlayer(gv);
	// if not Dracula's turn
	if (player != PLAYER_DRACULA) {
		printf("Cannot determine trap locations.\n");
		return locations;
	}
	// get last 6 Dracula locations
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
		if (gv->Path[curr][3] == 'T' &&
			gv->Path[curr][0] != 'D') { // trap encountered by Hunter
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
			gv->Path[curr][0] == 'D') { // loc of immature vampire
			int roundCount = (curr / 5) - (round - numReturnedLocs);
			locations[roundCount] = locations[*numTraps-1];
			locations = realloc(locations, sizeof(PlaceId)*(*numTraps-1));
			*numTraps -= 1;
		}
		else if (gv->Path[curr][3] == '.' &&
				 gv->Path[curr][0] == 'D') { // no trap placed
			int roundCount = curr / 5 - (round - numReturnedLocs);
			locations[roundCount] = locations[*numTraps-1];
			locations = realloc(locations, sizeof(PlaceId)*(*numTraps-1));
			*numTraps -= 1;
		} 
		else if (gv->Path[curr][0] == 'D') {
			abbrevLoc[1][0] = gv->Path[curr][1];
			abbrevLoc[1][1] = gv->Path[curr][2];	
			abbrevLoc[1][2] = '\0';
			PlaceId atSea = placeAbbrevToId(abbrevLoc[1]);
			if (placeIsSea(atSea)) { // Dracula is at sea
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
		// Get past 5 move of dracula
		char Past5Move[5][3];
		if (round < 5) {
			for (int i = 0, j = 4; i < round ; i++, j+=5) {
				Past5Move[i][0] = gv->Path[j][1];
				Past5Move[i][1] = gv->Path[j][2];
				Past5Move[i][2] = '\0';
			}
		} else {
			for (int i = 0, j = round - 5; i < 5; i++, j++) {
				Past5Move[i][0] = gv->Path[4 + j * 5][1];
				Past5Move[i][1] = gv->Path[4 + j * 5][2];
				Past5Move[i][2] = '\0';
			}
		}
		// Check whether a adjacent city satisfy condition
		int index = 1;
		int round_temp;
		if (round < 5) {
			round_temp = round;
		} else {
			round_temp = 5;
		}
		curr = MapGetConnections(gv->map, from);
		while (curr != NULL) {
			if (curr->type == RAIL 
			|| repeated_city[curr->p] == 1 
			|| curr->p == ST_JOSEPH_AND_ST_MARY) {
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
			if (curr->type == ROAD || curr->type == BOAT) {
				result[index++] = curr->p;
				result[index] =	999;
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
	if (road == false && rail == false && boat == false) {
		PlaceId *result = malloc(sizeof(PlaceId) * 2);
		result[0] = from;
		result[1] = 999;
		*numReturnedLocs = 1;
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
		char Past5Move[5][3];
		// Get past 5 move of dracula
		if (round < 5) {
			for (int i = 0, j = 4; i < round; i++, j+=5) {
				Past5Move[i][0] = gv->Path[j][1];
				Past5Move[i][1] = gv->Path[j][2];
				Past5Move[i][2] = '\0';
			}
		} else {
			for (int i = 0, j = round - 5; i < 5; i++, j++) {
				Past5Move[i][0] = gv->Path[4 + j * 5][1];
				Past5Move[i][1] = gv->Path[4 + j * 5][2];
				Past5Move[i][2] = '\0';
			}
		}
		// Check whether a adjacent city satisfy condition
		int index = 1;
		int round_temp;
		if (round < 5) {
			round_temp = round;
		} else {
			round_temp = 5;
		}
		curr = MapGetConnections(gv->map, from);
		while (curr != NULL) {
			if (curr->type == RAIL 
			|| repeated_city[curr->p] == 1 
			|| curr->p == ST_JOSEPH_AND_ST_MARY) {
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

// Get the cities conncected through rail
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
// Merge two list
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
// The list should end with 999
int GetLenOfList(PlaceId *list) {
	int i = 0;
	while (list[i] < 999) {
		i++;
	}
	return i;
}
