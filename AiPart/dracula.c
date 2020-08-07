////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" Dracula AI
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "dracula.h"
#include "DraculaView.h"
#include "Game.h"
#include "Map.h"
#include "Queue.h"
#include <assert.h>
#include <string.h>
#include "Places.h"

// stores the shortest path
typedef struct shortestPath {
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
} *ShortestPath;

// extra functions
void GetShortestPathLength(DraculaView dv, PlaceId src, PlaceId dest,
                           int *pathLength);
static void findshort(DraculaView dv, ShortestPath placeList, PlaceId dest);
static void cleanplaceLis(ShortestPath placeList);
static bool connectCheck(DraculaView dv, ShortestPath placeList, PlaceId src, PlaceId dest, int *levelRecord);
// get reachable places, start from road
static void reachPlacesRoad(DraculaView dv, ShortestPath placeList, PlaceId p, int *levelRecord);
// // get hunter reachable places, start from rail
// static void reachPlacesRail(DraculaView dc, ShortestPath placeList, PlaceId p, int *levelRecord);

// set default type
int MYBOAT = 1;
int MYRAIL = 1;
int MYROAD = 1;

void decideDraculaMove(DraculaView dv)
{
	// // TODO: Replace this with something better!
	// Round round = DvGetRound(dv);
	// int health = DvGetHealth(dv, PLAYER_DRACULA);
	// // Dracula's next move
	// PlaceId move;
	// char move_name[3];
	// // If round 0,
	// // loop through all valid locations
	// // excluding sea locations and hopsital
	// if (round == 0) {
	// 	int furthestDist = -1;
	// 	for (PlaceId i = MIN_REAL_PLACE; i <= MAX_REAL_PLACE; i++) {
	// 		// skip if sea locations and hospital
	// 		if (i == HOSPITAL_PLACE || placeIdToType(i) == SEA) {
	// 			continue;
	// 		}
	// 		if (i % 5 == 0 || i % 5 == 1 || i % 5 == 3 || i % 5 == 4) {
	// 			continue;
	// 		}
	// 		for (Player hunter = PLAYER_LORD_GODALMING; hunter < PLAYER_DRACULA; hunter++){
	// 			// PlaceId hloc = DvGetPlayerLocation(dv, hunter);
	// 			// const char *name = placeIdToName(hloc);
	// 			// printf("%s\n", name);
	// 			int pathLength = -1;
	// 			PlaceId locHunter = DvGetPlayerLocation(dv, hunter);
	// 			GetShortestPathLength(dv, locHunter, i, &pathLength);
	// 			// printf("%d\n", pathLength);
	// 			if (pathLength > furthestDist) {
	// 				move = i;
	// 				furthestDist = pathLength;
	// 			}
	// 			else if (pathLength == furthestDist) {
	// 				// check loc's distance from Munich
	// 				int distFromMunich_1 = -1;
	// 				GetShortestPathLength(dv, i, MUNICH, &distFromMunich_1);
	// 				int distFromMunich_2 = -1;
	// 				GetShortestPathLength(dv, move, MUNICH, &distFromMunich_2);
	// 				if (distFromMunich_1 < distFromMunich_2) move = i;
	// 			}
	// 		}
	// 		// printf("move: %d\n", move);
	// 	}
	// 	strcpy(move_name, placeIdToAbbrev(move));
	// 	registerBestPlay(move_name, "Mwahahahaha");
	// 	return;
	// }

	// // if not round 0
	// int numReturnedLocs = 0;
	// PlaceId *reachableLocs = DvGetValidMoves(dv, &numReturnedLocs);
	// if (numReturnedLocs == 0) {
	// 	move = placeAbbrevToId("TP");
	// 	strcpy(move_name, placeIdToAbbrev(move));
	// 	registerBestPlay(move_name, "Mwahahahaha");
	// 	return;
	// }

	// // Dracula returning to CD if his health is low 
	// if (health < 15) {
	// 	int shortestDist = -1;
	// 	for (int i = 0; reachableLocs[i] < numReturnedLocs; i++) {
	// 		int distfromCD = -1;
	// 		GetShortestPathLength(dv, reachableLocs[i], CASTLE_DRACULA, &distfromCD);
	// 		if (distfromCD < shortestDist) {
	// 			move = reachableLocs[i];
	// 		}
	// 	}
	// 	strcpy(move_name, placeIdToAbbrev(move));
	// 	registerBestPlay(move_name, "Mwahahahaha");
	// 	return;
	// }

	// if (reachableLocs != NULL) {
	// 	int furthestDis = -1;
	// 	for (int i = 0; i < numReturnedLocs; i++) {
	// 		int pathLength = -1;
	// 		for (Player hunter = PLAYER_LORD_GODALMING; hunter < PLAYER_DRACULA; hunter++){
	// 			GetShortestPathLength(dv, hunter, i, &pathLength);
	// 			if (pathLength > furthestDis) {
	// 				move = reachableLocs[i];
	// 				pathLength = furthestDis;
	// 			}
	// 			else if (pathLength == furthestDis) {
	// 				// check loc's distance from Munich
	// 				int distFromMunich_1 = -1;
	// 				GetShortestPathLength(dv, reachableLocs[i], MUNICH, &distFromMunich_1);
	// 				int distFromMunich_2 = -1;
	// 				GetShortestPathLength(dv, move, MUNICH, &distFromMunich_2);
	// 				if (distFromMunich_1 < distFromMunich_2) move = reachableLocs[i];
	// 			}
	// 		}
	// 	}
	// } 

	// free(reachableLocs);
	// strcpy(move_name, placeIdToAbbrev(move));
	// registerBestPlay(move_name, "Mwahahahaha");
	
	PlaceId move;
	char move_name[3];

	// if round 0
	if (DvGetRound(dv) == 0) {
		strcpy(move_name, "CD");
		registerBestPlay(move_name, "Mwahahahaha");
		return;
	}

	int numReturnedLocs = 0;
	PlaceId *reachableLocs = DvGetValidMoves(dv, &numReturnedLocs);

	// if no valid moves
	if (numReturnedLocs == 0) {
		move = placeAbbrevToId("TP");
		strcpy(move_name, placeIdToAbbrev(move));
		registerBestPlay(move_name, "Mwahahahaha");
		return;
	}

	// if Dracula is on low leath
	if (DvGetHealth(dv, PLAYER_DRACULA) < 15) {
		int shortestDist = -1;
		for (int i = 0; reachableLocs[i] < numReturnedLocs; i++) {
			int distfromCD = -1;
			GetShortestPathLength(dv, reachableLocs[i], CASTLE_DRACULA, &distfromCD);
			if (distfromCD < shortestDist) {
				move = reachableLocs[i];
			}
		}
		strcpy(move_name, placeIdToAbbrev(move));
		registerBestPlay(move_name, "Mwahahahaha");
		return;
	}

	// if there are valid moves
	if (numReturnedLocs == 1) {
		strcpy(move_name, placeIdToAbbrev(reachableLocs[0]));
	}
	else {
		strcpy(move_name, placeIdToAbbrev(reachableLocs[1]));
	}
	free(reachableLocs);
	registerBestPlay(move_name, "Mwahahahaha");	
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void GetShortestPathLength(DraculaView dv, PlaceId src, PlaceId dest,
                             int *pathLength)
{
	// dest == current place
	if (src == dest) {
		*pathLength = 0;
		return;
	}
	PlaceId startId = src;
	int maxLen = MapNumPlaces(MapNew());
	ShortestPath placeList = malloc(sizeof(ShortestPath));

	// set default value in placeList
	placeList->places = calloc(maxLen, sizeof(PlaceId));
	placeList->round = DvGetRound(dv);
	placeList->totalNum = 0;
	placeList->player = PLAYER_DRACULA;
	placeList->railNum = 0;
	placeList->start = startId;
	MYBOAT = 1;
	MYRAIL = 1;
	MYROAD = 1;
	cleanplaceLis(placeList);
	// find shortest path
	findshort(dv, placeList, dest);
	*pathLength = placeList->totalNum;
	*pathLength = placeList->totalNum;
	*pathLength = placeList->totalNum;
	PlaceId *result = malloc(sizeof(PlaceId)*placeList->totalNum);
	for (int i = 0; i < placeList->totalNum; i++) {
		result[i] = placeList->places[i];
	}
	free(result);
	return;
}

// get shortest path in BFS
static void findshort(DraculaView dv, ShortestPath placeList, PlaceId dest) {
	assert (dv != NULL);
	int maxLen = MapNumPlaces(MapNew());
	PlaceId *visited = calloc(maxLen,sizeof(PlaceId));
	Queue q = QueueNew();
	PlaceId src = placeList->start;
	QueueEnqueue(q, src);
	int isFound = 0;
	int *new_path = calloc(maxLen,sizeof(PlaceId));

	// record round from
	int *levelRecord = calloc(maxLen,sizeof(int));
	for (int i = 0; i < maxLen; i++) {
		levelRecord[i] = -1;
		new_path[i] = -1;
	}
	levelRecord[src] = placeList->round;
	while (!QueueIsEmpty(q) && !isFound) {
		PlaceId y,x = QueueDequeue(q);
		if (visited[x]) {
			continue;
		}
		visited[x] = 1;
		for (y = 0; y < maxLen; y++) {
			if (visited[y] || connectCheck(dv,placeList,x,y, levelRecord) == false || new_path[y] != -1){
				continue;
			}
			new_path[y] = x;
			if (y == dest) { isFound = 1;break; }
			if (!visited[y]) {
				QueueEnqueue(q,y);
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

// clean placeList
static void cleanplaceLis(ShortestPath placeList) {
	Map ne = MapNew();
	int maxLen = MapNumPlaces(ne);
	//init to 0
	for (int i = 0; i < maxLen;i++) {
		placeList->places[i] = -1;
	}
	placeList->totalNum = 0;
}

// check two place conected
static bool connectCheck(DraculaView dv, ShortestPath placeList, PlaceId src, PlaceId dest, int *levelRecord) {
	placeList->railNum = 0;
	// check road and sea
	placeList->start = src;
	// int originRail = placeList->railNum;
	// PlaceId originStart = placeList->start;
	placeList->places[src] = src;
	reachPlacesRoad(dv,placeList,src,levelRecord);
	placeList->places[src] = -1;
	if (placeList->places[dest] != -1) {
		cleanplaceLis(placeList);
		return true;
	}
	cleanplaceLis(placeList);

	// // check rail
	// placeList->railNum = originRail;
	// placeList->start = originStart;
	// placeList->places[src] = src;
	// reachPlacesRail(dv,placeList,src,levelRecord);
	// if (placeList->places[dest] != -1) {
	// 	cleanplaceLis(placeList);
	// 	return true;
	// }
	// cleanplaceLis(placeList);
	return false;
}

// get hunter reachable palces from road or sea
static void reachPlacesRoad(DraculaView dv, ShortestPath placeList, PlaceId p, int *levelRecord) {
	ConnList current = MapGetConnections(MapNew(), p);
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

// // get hunter reachable places, start from rail
// static void reachPlacesRail(DraculaView dc, ShortestPath placeList, PlaceId p, int *levelRecord){
// 	if (placeList->railNum == 0) {
// 		return;
// 	}
// 	ConnList current = MapGetConnections(MapNew(), p);
// 	int originRail = placeList->railNum;
// 	// all location come from the same start place
// 	PlaceId originStart = placeList->start;
// 	for (ConnList i = current; i != NULL; i = i->next) {
// 		if (i->type == RAIL && MYRAIL) {
// 			if (placeList->places[i->p] == -1) {
// 				if (levelRecord[i->p] == -1) {
// 					levelRecord[i->p] = levelRecord[originStart] + 1;
// 				}
// 				placeList->places[i->p] = originStart;
// 				placeList->totalNum+=1;
				
// 				// move to next station
// 				placeList->railNum = placeList->railNum - 1;
// 				reachPlacesRail(dc, placeList, i->p, levelRecord);
// 				placeList->railNum = originRail;
// 			}
// 		}
// 	}
// }

