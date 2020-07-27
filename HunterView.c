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
////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

HunterView HvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	HunterView new = malloc(sizeof(*new));
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
	return 0;
}

Player HvGetPlayer(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return PLAYER_LORD_GODALMING;
}

int HvGetScore(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int HvGetHealth(HunterView hv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId HvGetVampireLocation(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
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
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
                             bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
                          int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
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
		if (i->type == RAIL) {
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
// TODO
