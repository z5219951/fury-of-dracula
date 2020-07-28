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
    while (new->Path[index] != NULL)
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
