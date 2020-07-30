////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// HunterView.c: the HunterView ADT implementation
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
#include "HunterView.h"
#include "Map.h"
#include "Places.h"
// add your own #includes here
#include<string.h>
#include "QueueYue.h"
// TODO: ADD YOUR OWN STRUCTS HERE

struct hunterView {
	// TODO: ADD FIELDS HERE
	char **Path;
	int num;
	Map map;
};

// store the hunter reachable places
typedef struct huntPlace {
	PlaceId *places;
	int totalNum;
	int railNum;
	PlaceId start;
	int levelFinish;
	int round;
	int player;
	int road;
	int boat;
	int rail;
} *HunterReach;
// change hunter adt into gameview adt
static GameView hunterToGame(HunterView hv);

// get hunter reachable places, start from road
static void reachPlacesRoad(HunterView hv, HunterReach placeList, PlaceId p, int *levelRecord);

// get hunter reachable places, start from rail
static void reachPlacesRail(HunterView hv, HunterReach placeList, PlaceId p, int *levelRecord);

// get shortest path
static void findshort(HunterView hv, HunterReach placeList, PlaceId dest);

//check connection
static bool connectCheck(HunterView hv, HunterReach placeList,PlaceId src, PlaceId dest, int *levelRecord);

// clean placeList
static void cleanplaceLis(HunterReach placeList);

// free placeList
static void freePlaceLis(HunterReach placeList);

// check repeat places
static int checkPlaceRe(PlaceId *list, int len, PlaceId target);

// set default type
int MYBOAT = 1;
int MYRAIL = 1;
int MYROAD = 1;
////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

HunterView HvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	HunterView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate hunterView!\n");
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

void HvFree(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free(hv->Path);
	MapFree(hv->map);
	free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round HvGetRound(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	assert(hv != NULL);
	return hv->num / 5;
}

Player HvGetPlayer(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	assert(hv != NULL);
	if (hv->num == 0) return PLAYER_LORD_GODALMING; //game have not started
	switch (hv->Path[hv->num - 1][0]) {
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

int HvGetScore(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	assert(hv != NULL);
	if (hv->num == 0) return GAME_START_SCORE;
	int score = GAME_START_SCORE - hv->num / NUM_PLAYERS; 
	//Caculate loss score in DRACULA turn	  
	int Hunter_health[4]; //store hunter's health
	for (int i = 0; i < 4; i++) // Initialize the hunter's health
	Hunter_health[i] = GAME_START_HUNTER_LIFE_POINTS;  
	for (int i = 0; i < hv->num; i++) {
		if (i % 5 == PLAYER_DRACULA) {
			// Caculate loss score when Vampire mature
			if (hv->Path[i][5] == 'V') 
			score -= SCORE_LOSS_VAMPIRE_MATURES;
		} else {
			Hunter_health[i % 5]
			= HvGetRoundHealth(hv, i % 5, Hunter_health[i % 5], i);
			// Cacualte loss score when hunter are in hospital
			if (Hunter_health[i % 5] <= 0) {
				score -= SCORE_LOSS_HUNTER_HOSPITAL;
			}
		}
	}
	return score;
}

int HvGetHealth(HunterView hv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	assert(hv != NULL);
	int health = 0;
	// Intialize the health value
	if (player == PLAYER_DRACULA) health = GAME_START_BLOOD_POINTS;
	else health = GAME_START_HUNTER_LIFE_POINTS;
	for (int i = player; i < hv->num; i += NUM_PLAYERS) {
		// recovre the hunter's health
		if (health < 0 && player != PLAYER_DRACULA) 
		health = GAME_START_HUNTER_LIFE_POINTS;
		health = HvGetRoundHealth(hv, player, health, i);
	}
	// return health when the hunter in hospital
	if (health <= 0 && player != PLAYER_DRACULA) return 0;

	return health;
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// check hv is not NULL
	if (hv == NULL || hv->num == 0) {
		return NOWHERE;
	}
	int health = HvGetHealth(hv, player);
	if (health == 0) {
		return HOSPITAL_PLACE;
	}
	int numReturnedLocs = 1; 
	bool canFree = 1;
	PlaceId *result; 
	GameView trans = hunterToGame(hv);
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

PlaceId HvGetVampireLocation(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// check hv is not NULL
	if (hv == NULL || hv->num == 0) {
		return NOWHERE;
	}
	bool canFree = 1;
	int numReturnedLocs;
	GameView trans = hunterToGame(hv);
	PlaceId *locations = GvGetLastLocations(trans, PLAYER_DRACULA, TRAIL_SIZE, 
								   &numReturnedLocs, &canFree);
	if (numReturnedLocs == 0) {
		return NOWHERE;
	}
	Round round = HvGetRound(hv);
	// scan through last 6 rounds, from earliest to most recent
	int curr = (round - TRAIL_SIZE) * NUM_PLAYERS;
	if (curr < 0) {
		curr = 0;
	}
	int vampLoc = -1;
	for (int counter = 1; curr < hv->num; curr++, counter++) {
		if (hv->Path[curr][0] == 'D' && 
			hv->Path[curr][4] == 'V') {
			vampLoc = counter / 5;
		} else if (hv->Path[curr][3] == 'V' ||
				   hv->Path[curr][4] == 'V' ||
				   hv->Path[curr][5] == 'V') { 
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

////////////////////////////////////////////////////////////////////////
// Utility Functions

PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	int numReturnedLocs = 0;
	bool canFree = false;
	// check the hv is not null
	// return NOWHERE if Dracula has not moved yet
	if (hv == NULL || hv->num <= 4) {
		return NOWHERE;
	}
	// transform hv to gv
	GameView trans = hunterToGame(hv);
	// get result from GvGetLocationHistory
	PlaceId *result = GvGetLocationHistory(trans, PLAYER_DRACULA, &numReturnedLocs, &canFree);
	PlaceId lastLoc = NOWHERE;
	for (int i = 0; i < numReturnedLocs; i++) {
		if (result[i] != CITY_UNKNOWN && result[i] != SEA_UNKNOWN) {
			lastLoc = result[i];
			*round = i;
		}
	}
	return lastLoc;
}

PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// check whether the Player has moved
	// transform hv to gv
	GameView trans = hunterToGame(hv);
	// get result from GvGetLastLocations
	int numReturnedLocs = 0;
	bool canFree = false;
	*pathLength = 0;
	PlaceId *lastPlace = GvGetLastLocations(trans, hunter,1, &numReturnedLocs, &canFree);
	// if current player hasn't moved yet
	// or dest == current place
	if (numReturnedLocs == 0 || lastPlace[0] == dest) {
		lastPlace = malloc(sizeof(PlaceId));
		lastPlace[0] = dest;
		return lastPlace;
	}
	PlaceId startId = lastPlace[0];
	int maxLen = MapNumPlaces(hv->map);
	HunterReach placeList = malloc(sizeof(HunterReach));

	// set default value in placeList
	placeList->places = calloc(maxLen, sizeof(PlaceId));
	int round = (hv->num)/5;
	placeList->round = round;
	placeList->player = hunter;
	placeList->railNum = (hunter + round)%4;
	placeList->totalNum = 0;
	placeList->start = startId;
	MYBOAT = 1;
	MYRAIL = 1;
	MYROAD = 1;
	cleanplaceLis(placeList);
	// find shortest path
	findshort(hv, placeList,dest);
	*pathLength = placeList->totalNum;
	PlaceId *result = malloc(sizeof(PlaceId)*placeList->totalNum);
	for (int i = 0; i < placeList->totalNum; i++) {
		result[i] = placeList->places[i];
	}
	return result;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// check whether the Player has moved
	// transform hv to gv
	GameView trans = hunterToGame(hv);
	Player hunter = hv->num%5;
	if (hunter == 4) {
		hunter = PLAYER_LORD_GODALMING;
	}
	// get result from GvGetLastLocations
	int numReturn = 0;
	bool canFree = false;
	PlaceId *lastPlace = GvGetLastLocations(trans, hunter,1, &numReturn, &canFree);
	// if current player hasn't moved yet
	if (numReturn == 0) {
		*numReturnedLocs = 0;
		return NULL;
	}

	PlaceId startId = lastPlace[0];
	int maxLen = MapNumPlaces(hv->map);
	HunterReach placeList = malloc(sizeof(HunterReach));
	int lenNum = 0;

	// set default value in placeList
	placeList->places = calloc(maxLen, sizeof(PlaceId));
	int round = (hv->num)/5;
	placeList->round = round;
	placeList->player = hunter;
	placeList->railNum = (hunter + round)%4;
	placeList->totalNum = 0;
	placeList->start = startId;
	//set type
	MYBOAT = 1;
	MYRAIL = 1;
	MYROAD = 1;

	cleanplaceLis(placeList);

	int *levelRecord = calloc(maxLen,sizeof(int));
	for (int i = 0; i < maxLen; i++) {
		levelRecord[i] = -1;
	}
	levelRecord[startId] = placeList->round;

	PlaceId *result = malloc(sizeof(PlaceId)*maxLen);

	// check road and sea
	int originRail = placeList->railNum;
	PlaceId originStart = placeList->start;
	placeList->places[startId] = startId;
	reachPlacesRoad(hv,placeList,startId,levelRecord);
	for (int i = 0; i < maxLen; i++) {
		if (placeList->places[i] != -1) {
			result[lenNum] = i;
			lenNum++;
		}
	}
	cleanplaceLis(placeList);

	// check rail
	placeList->railNum = originRail;
	placeList->start = originStart;
	placeList->places[startId] = startId;
	reachPlacesRail(hv,placeList,startId,levelRecord);
	placeList->places[startId] = -1;
	for (int i = 0; i < maxLen; i++) {
		if (placeList->places[i] != -1 && checkPlaceRe(result, lenNum, i) == 1) {
			result[lenNum] = i;
			lenNum++;
		}
	}
	cleanplaceLis(placeList);
	*numReturnedLocs = lenNum;
	freePlaceLis(placeList);
	return result;
}

PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
                             bool boat, int *numReturnedLocs)
{
	GameView trans = hunterToGame(hv);
	Player hunter = hv->num%5;
	if (hunter == 4) {
		hunter = PLAYER_LORD_GODALMING;
	}
	// get result from GvGetLastLocations
	int numReturn = 0;
	bool canFree = false;
	PlaceId *lastPlace = GvGetLastLocations(trans, hunter,1, &numReturn, &canFree);
	// if current player hasn't moved yet
	if (numReturn == 0) {
		*numReturnedLocs = 0;
		return NULL;
	}

	if (!road&&!rail&&!boat) {
		*numReturnedLocs = 1;
		return lastPlace;
	}

	PlaceId startId = lastPlace[0];
	int maxLen = MapNumPlaces(hv->map);
	HunterReach placeList = malloc(sizeof(HunterReach));
	int lenNum = 0;

	// set default value in placeList
	placeList->places = calloc(maxLen, sizeof(PlaceId));
	int round = (hv->num)/5;
	placeList->round = round;
	placeList->player = hunter;
	placeList->railNum = (hunter + round)%4;
	placeList->totalNum = 0;
	placeList->start = startId;

	cleanplaceLis(placeList);

	int *levelRecord = calloc(maxLen,sizeof(int));
	for (int i = 0; i < maxLen; i++) {
		levelRecord[i] = -1;
	}
	levelRecord[startId] = placeList->round;

	PlaceId *result = malloc(sizeof(PlaceId)*maxLen);

	int originRail = placeList->railNum;
	PlaceId originStart = placeList->start;
	placeList->places[startId] = startId;
	// check road and sea
	//set type

	MYBOAT = boat;
	MYRAIL = rail;
	MYROAD = road;

	reachPlacesRoad(hv,placeList,startId,levelRecord);
	for (int i = 0; i < maxLen; i++) {
		if (placeList->places[i] != -1) {
			result[lenNum] = i;
			lenNum++;
		}
	}
	cleanplaceLis(placeList);

	// check rail
	placeList->railNum = originRail;
	placeList->start = originStart;
	placeList->places[startId] = startId;
	reachPlacesRail(hv,placeList,startId,levelRecord);
	placeList->places[startId] = -1;
	for (int i = 0; i < maxLen; i++) {
		if (placeList->places[i] != -1 && checkPlaceRe(result, lenNum, i) == 1) {
			result[lenNum] = i;
			lenNum++;
		}
	}
	cleanplaceLis(placeList);
	*numReturnedLocs = lenNum;
	MYBOAT = 1;
	MYRAIL = 1;
	MYROAD = 1;
	freePlaceLis(placeList);
	return result;
}

PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
                          int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Player currPlayer = (hv->num) % 5;
	int round = (hv->num) / 5;
	if (currPlayer > player) {
		round++;
	}
	int n = MapNumPlaces(hv->map);
	int *repeated_city = calloc(n, sizeof(int));
	PlaceId from;
	if (player == PLAYER_DRACULA) {
		from = HvGetLastKnownDraculaLocation(hv, &round);
		if (from == NOWHERE) {
			*numReturnedLocs = 0;
			return NULL;
		}
		int counter = 0;
		ConnList curr = MapGetConnections(hv->map, from);
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * (counter + 2));
		result[0] = from;
		result[1] = 999;
		char Past5Move[5][3];
		if (round < 6) {
			for (int i = 0, j = 4; i < round; i++, j+=5) {
				Past5Move[i][0] = hv->Path[j][1];
				Past5Move[i][1] = hv->Path[j][2];
				Past5Move[i][2] = '\0';
			}
		} else {
			for (int i = 0, j = round - 5; i < 5; i++, j++) {
				Past5Move[i][0] = hv->Path[4 + j * 5][1];
				Past5Move[i][1] = hv->Path[4 + j * 5][2];
				Past5Move[i][2] = '\0';
			}
		}
		int index = 1;
		int round_temp;
		if (round < 5) {
			round_temp = round;
		} else {
			round_temp = 5;
		}
		curr = MapGetConnections(hv->map, from);
		while (curr != NULL) {
			if (curr->type == RAIL
			|| repeated_city[curr->p] == 1
			|| curr->p ==ST_JOSEPH_AND_ST_MARY) {
				curr = curr->next;
				continue;
			}
			bool hasRepeatedMove = false;
			for (int i = 0; i < round_temp; i++) {
				if (strcmp(Past5Move[i], placeIdToAbbrev(curr->p)) == 0) {
					hasRepeatedMove = true;
				}
			}
			if (hasRepeatedMove) {
				curr = curr->next;
				continue;
			}
			if (curr->type == ROAD || curr->type == BOAT) {
				result[index++] = curr->p;
				result[index] =	999;
				repeated_city[curr->p] = 1;
			}
			curr = curr->next;
	}
		*numReturnedLocs = GetLenOfList(result);
		return result;
	} else {
		from = HvGetPlayerLocation(hv,player);
		int counter = 0;
		ConnList curr = MapGetConnections(hv->map, from);
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * (counter + 2));
		result[0] = from;
		result[1] = 999;
		int index = 1;
		int max_distance = (round + player) % 4;
		PlaceId *RailList = GetConnRail(hv->map, from, max_distance, repeated_city);
		curr = MapGetConnections(hv->map, from);
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
				if (placeIsSea(from)) {
					result[index++] = curr->p;
					result[index] = 999;
					repeated_city[curr->p] = 1;
				}
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
		return result;
	}
	return NULL;
}

PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Player currPlayer = (hv->num) % 5;
	int round = (hv->num) / 5;
	if (currPlayer > player) {
		round++;
	}
	int n = MapNumPlaces(hv->map);
	int *repeated_city = calloc(n, sizeof(int));
	int numReturn = 0;
    bool canFree = false;
    GameView trans = hunterToGame(hv);
    PlaceId *from = GvGetLastLocations(trans, player, 1, &numReturn, &canFree );
	if (player == PLAYER_DRACULA) {
		*from = HvGetLastKnownDraculaLocation(hv, &round);
		if (*from == NOWHERE) {
			*numReturnedLocs = 0;
			return NULL;
		}
		int counter = 0;
		ConnList curr = MapGetConnections(hv->map, *from);
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * (counter + 2));
		result[0] = *from;
		result[1] = 999;
		char Past5Move[5][3];
		if (round < 5) {
			for (int i = 0, j = 4; i < round; i++, j+=5) {
				Past5Move[i][0] = hv->Path[j][1];
				Past5Move[i][1] = hv->Path[j][2];
				Past5Move[i][2] = '\0';
			}
		} else {
			for (int i = 0, j = round - 5; i < 5; i++, j++) {
				Past5Move[i][0] = hv->Path[4 + j * 5][1];
				Past5Move[i][1] = hv->Path[4 + j * 5][2];
				Past5Move[i][2] = '\0';
			}
		}
		int index = 1;
		int round_temp;
		if (round < 5) {
			round_temp = round;
		} else {
			round_temp = 5;
		}
		curr = MapGetConnections(hv->map, *from);
		while (curr != NULL) {
			if (curr->type == RAIL
			|| repeated_city[curr->p] == 1
			|| curr->p == ST_JOSEPH_AND_ST_MARY) {
				curr = curr->next;
				continue;
			}
			bool hasRepeatedMove = false;
			for (int i = 0; i < round_temp; i++) {
				if (strcmp(Past5Move[i], placeIdToAbbrev(curr->p)) == 0) {
					hasRepeatedMove = true;
				}
			}
			if (hasRepeatedMove) {
				curr = curr->next;
				continue;
			}
			if (curr->type == ROAD && road == true) {
				result[index++] = curr->p;
				result[index] =	999;
				repeated_city[curr->p] = 1;
			}
			else if (curr->type == BOAT && boat == true)  {
				result[index++] = curr->p;
				result[index] = 999;
				repeated_city[curr->p] = 1;
			}
			curr = curr->next;
	}
		*numReturnedLocs = GetLenOfList(result);
		return result;
	} else {
		*from = HvGetPlayerLocation(hv,player);
		int counter = 0;
		ConnList curr = MapGetConnections(hv->map, *from);
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * (counter + 2));
		result[0] = *from;
		result[1] = 999;
		int index = 1;
		int max_distance = (round + player) % 4;
		PlaceId *RailList = NULL;
		if (rail == true) {
			RailList = GetConnRail(hv->map, *from, max_distance, repeated_city);
		}
		curr = MapGetConnections(hv->map, *from);
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
				if (placeIsSea(*from)) {
					result[index++] = curr->p;
					result[index] = 999;
					repeated_city[curr->p] = 1;
				}
				else if (placeIsLand(*from) && placeIsSea(curr->p)) {
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

		return result;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// help function
// change hunter adt into gameview adt
static GameView hunterToGame(HunterView hv) {
	Message messages[] = {};
	char *originPast = malloc(8*hv->num);
	for (int i = 0; i < hv->num; i++) {
		strcat(originPast, hv->Path[i]);
		if (i != hv->num-1) {
			strcat(originPast, " ");
		}
	}
	return GvNew(originPast, messages);
}

// get hunter reachable palces from road or sead
static void reachPlacesRoad(HunterView hv, HunterReach placeList, PlaceId p, int *levelRecord) {
	ConnList current = MapGetConnections(hv->map, p);
	PlaceId originStart = p;
	for (ConnList i = current; i != NULL; i = i->next) {
		// only accept no rail type
		if (i->type != RAIL) {
			if (i->type == ROAD) {
				if (!MYROAD) {
					continue;
				}
			}
			if ((i->type == BOAT)) {
				if (!MYBOAT) {
					continue;
				}
				
			}
			if (placeList->places[i->p] == -1) {
				if (levelRecord[i->p] == -1) {
					levelRecord[i->p] = levelRecord[originStart] + 1;
				}
				placeList->places[i->p] = originStart;
				placeList->totalNum+=1;
			}
		}
	}
}

// get hunter reachable places, start from rail
static void reachPlacesRail(HunterView hv, HunterReach placeList, PlaceId p, int *levelRecord){
	if (placeList->railNum == 0) {
		return;
	}
	ConnList current = MapGetConnections(hv->map, p);
	int originRail = placeList->railNum;
	// all location come from the same start place
	PlaceId originStart = placeList->start;
	for (ConnList i = current; i != NULL; i = i->next) {
		if (i->type == RAIL && MYRAIL) {
			if (placeList->places[i->p] == -1) {
				if (levelRecord[i->p] == -1) {
					levelRecord[i->p] = levelRecord[originStart] + 1;
				}
				placeList->places[i->p] = originStart;
				placeList->totalNum+=1;
				
				// move to next station
				placeList->railNum = placeList->railNum - 1;
				reachPlacesRail(hv, placeList, i->p, levelRecord);
				placeList->railNum = originRail;
			}
		}
	}
}

// get shortest path in BFS
static void findshort(HunterView hv, HunterReach placeList, PlaceId dest){
	assert (hv != NULL);
	int maxLen = MapNumPlaces(hv->map); 
	PlaceId *visited = calloc(maxLen,sizeof(PlaceId));
	QueueYue q = newQueueYue();
	PlaceId src = placeList->start;
	QueueYueJoin(q, src);
	int isFound = 0;
	int *new_path = calloc(maxLen,sizeof(PlaceId));

	// record round from
	int *levelRecord = calloc(maxLen,sizeof(int));
	for (int i = 0; i < maxLen; i++) {
		levelRecord[i] = -1;
		new_path[i] = -1;
	}
	levelRecord[src] = placeList->round;
	while (!QueueYueIsEmpty(q) && !isFound) {
		PlaceId y,x = QueueYueLeave(q);
		if (visited[x]) {
			continue;
		}
		visited[x] = 1;
		for (y = 0; y < maxLen; y++) {
			if (visited[y] || connectCheck(hv,placeList,x,y, levelRecord) == false || new_path[y] != -1){
				continue;
			}
			new_path[y] = x;
			if (y == dest) { isFound = 1;break; }
			if (!visited[y]) { 
				QueueYueJoin(q,y);
			}
		}
	}

	int index = 0;
	if (isFound) {
		int *or_path = calloc(maxLen,sizeof(PlaceId));
		int index2 = 0;
		for (int v = dest; v != src; v = new_path[v]) {
			or_path[index2] = v;
			index2++;
		}
		or_path[index2] = src;
		index2++;
		for (int i = (index2-2); i >= 0; i--) {
			placeList->places[index] = or_path[i];
			index++;
		}
	}
	placeList->totalNum = index;
}

// check two place conected
static bool connectCheck(HunterView hv, HunterReach placeList,PlaceId src, PlaceId dest, int *levelRecord) {
	placeList->railNum = (levelRecord[src] + placeList->player)%4;
	// check road and sea
	placeList->start = src;
	int originRail = placeList->railNum;
	PlaceId originStart = placeList->start;
	placeList->places[src] = src;
	reachPlacesRoad(hv,placeList,src,levelRecord);
	placeList->places[src] = -1;
	if (placeList->places[dest] != -1) {
		cleanplaceLis(placeList);
		return true;
	}
	cleanplaceLis(placeList);

	// check rail
	placeList->railNum = originRail;
	placeList->start = originStart;
	placeList->places[src] = src;
	reachPlacesRail(hv,placeList,src,levelRecord);
	if (placeList->places[dest] != -1) {
		cleanplaceLis(placeList);
		return true;
	}
	cleanplaceLis(placeList);
	return false;
}

// clean placeList
static void cleanplaceLis(HunterReach placeList) {
	Map ne = MapNew();
	int maxLen = MapNumPlaces(ne);
	//init to 0
	for (int i = 0; i < maxLen;i++) {
		placeList->places[i] = -1;
	}
	placeList->totalNum = 0;
}
// check repeat place
static int checkPlaceRe(PlaceId *list, int len, PlaceId target) {
	if (!len) {
		return 1;
	}
	for (int i = 0; i< len; i++) {
		if (list[i] == target) {
			return 0;
		}
	}
	return 1;
}

// free placeList
static void freePlaceLis(HunterReach placeList){
	free(placeList);
}

// TODO
int HvGetRoundHealth(HunterView hv, Player player, int health, int round) {
	assert(hv != NULL);
	// Convert the palce in path char
	char place1[3];
	place1[0] = hv->Path[round][1];
	place1[1] = hv->Path[round][2];
	place1[2] = '\0';
	// Caculate the health value of Dracula
	if (player != PLAYER_DRACULA) {
		// Encounter the trap or Vampire
		for (int j = 3; j < 7; j++) {
			if (hv->Path[round][j] == 'T') health -= LIFE_LOSS_TRAP_ENCOUNTER;
			if (hv->Path[round][j] == 'D') health -= LIFE_LOSS_DRACULA_ENCOUNTER;
		}
		if (round >= NUM_PLAYERS){
			char place2[8];
			place2[0] = hv->Path[round - NUM_PLAYERS][1];
			place2[1] = hv->Path[round - NUM_PLAYERS][2];
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
			hvGetLastLocations(hv, player, backmove, false);
			place1 = placeIdToAbbrev(move[backmove])*/
			backmove *= NUM_PLAYERS;
			backmove = round - backmove;
			place1[0] = hv->Path[backmove][1];
			place1[1] = hv->Path[backmove][2];
			place1[2] = '\0'; 
		}
		// Caculate the health when encounter the hunter
		for (int j = round + 1; j < hv->num 
		&& j < round + NUM_PLAYERS; j++) {
			char place2[3];
			place2[0] = hv->Path[j][1];
			place2[1] = hv->Path[j][2];
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
