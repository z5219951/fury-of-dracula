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
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "dracula.h"
#include "DraculaView.h"
#include "Game.h"
#include "Map.h"
#include "Queue.h"
#include "Places.h"
#include <time.h>

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
int hunterhere(DraculaView dv, PlaceId place);
int isexposed(DraculaView dv);
char* checkmove(DraculaView dv, char *ove);
void freepath(PlaceId *adress);
// set default type
int MYBOAT = 1;
int MYRAIL = 1;
int MYROAD = 1;

void decideDraculaMove(DraculaView dv)
{
	// TODO: Replace this with something better!
	PlaceId move;
	char move_name[3];

	// if round 0
	if (DvGetRound(dv) == 0) {
		srand(time(0));
		PlaceId rplace = rand() % MAX_REAL_PLACE;
		while (rplace == placeAbbrevToId("JM"))
		rplace = rand() % MAX_REAL_PLACE;
		strcpy(move_name, placeIdToAbbrev(rplace));
		registerBestPlay(move_name, "Let darkness be parted!");
		return;
	}

	int numMove = 0;
	PlaceId *reachMove = DvGetValidMoves(dv, &numMove);
	// if no valid moves
	if (numMove == 0) {
		move = placeAbbrevToId("TP");
		strcpy(move_name, placeIdToAbbrev(move));
		registerBestPlay(move_name, "We shall rise from the darkness~");
		return;
	}

	/*int distfromCD = -1;
	freepath(GetShortestPathLength(dv, VIENNA, CASTLE_DRACULA, &distfromCD, false));
	move = placeAbbrevToId("TP");
		strcpy(move_name, placeIdToAbbrev(move));
		registerBestPlay(move_name, "We shall rise from the darkness~");
	return;*/
	// if Dracula is on low health
	PlaceId now_location = DvGetPlayerLocation(dv, PLAYER_DRACULA);
	move = reachMove[0];
	if (strcmp(checkmove(dv, "HI"), "vaild") == 0 && now_location == CASTLE_DRACULA)
	move = HIDE;
	if (!hunterhere(dv, CASTLE_DRACULA))
	if (DvGetHealth(dv, PLAYER_DRACULA) <= 18) {
		int shortestDist = 140;
		for (int i = 0; i < numMove; i++) {
			PlaceId location = DraculaMove(dv, reachMove[i]);
			if (!hunterhere(dv, location)) continue;
			int distfromCD = -1;
			freepath(GetShortestPathLength(dv, location, CASTLE_DRACULA, &distfromCD, true));
			if (distfromCD < shortestDist) {
				move = reachMove[i];
				shortestDist = distfromCD;
			}
			if (distfromCD == 1)  break;
		}
		strcpy(move_name, placeIdToAbbrev(move));
		registerBestPlay(move_name, "Valor shall rise again");
		return;
	}

	// if there are valid moves
	// have been found out
	if (isexposed(dv))
		/*
		int closestDist = 500;
		for (PlaceId i = MIN_REAL_PLACE; i <= MAX_REAL_PLACE; i++) {
			for (int vmove = 0; vmove < numMove; vmove++) {
				//skip if sea locations and hospital
				if (i == HOSPITAL_PLACE || placeIdToType(i) == LAND) continue;
				int pathLength = -1;
				PlaceId location = DraculaMove(dv, reachMove[i]);
				GetShortestPathLength(dv, location, i, &pathLength, true);
				if (pathLength < closestDist) {
					move = vmove;
					closestDist = pathLength;
				}
				if (closestDist < 5) break;
			}
		}*/
		if (strcmp(checkmove(dv, "HI"), "vaild") == 0) {
			move = HIDE;
			strcpy(move_name, placeIdToAbbrev(move));
			registerBestPlay(move_name, "Mwahahahaha");
			return;
		}
	// normal move
	int furthestDis = -1;
	move = reachMove[0];
	for (int i = 0; i < numMove; i++) {
		PlaceId location = DraculaMove(dv, reachMove[i]);
		if (hunterhere(dv, location)) continue;
		int pathsum = -1;
		for (Player hunter = PLAYER_LORD_GODALMING; hunter < PLAYER_DRACULA; hunter++){
			int pathLength = -1;
			HGetShortestPathLength(dv, hunter, location, &pathLength);
			pathsum += pathLength;
		}
		if (pathsum > furthestDis) {
			move = reachMove[i];
			pathsum = furthestDis;
		}
		else if (pathsum == furthestDis && now_location != MUNICH) {
			//check loc's distance from Munich
			int distFromMunich_1 = -1;
			freepath(GetShortestPathLength(dv, location, MUNICH, &distFromMunich_1, true));
			int distFromMunich_2 = -1;
			freepath(GetShortestPathLength(dv, move, MUNICH, &distFromMunich_2, true));
			if (distFromMunich_1 < distFromMunich_2) move = reachMove[i];
		}
	}
	//srand(time(0));
	//strcpy(move_name, placeIdToAbbrev(reachMove[rand() % numMove]));
	strcpy(move_name, placeIdToAbbrev(move));
	freepath(reachMove);
	registerBestPlay(move_name, checkmove(dv, move_name));
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
char* checkmove(DraculaView dv, char *move){
	int num = 0;
	int* reachable = DvGetValidMoves(dv, &num);
	for (int i = 0; i < num; i++) {
			if (reachable [i] == placeAbbrevToId(move)) {
				free(reachable);
				return "vaild";
			}
	}
	free(reachable);
	return move;
}
int isexposed(DraculaView dv) {
	int num = 0;
	int num_move =0;
	for (int hunter = 0; hunter < 4; hunter++){
		bool canFree;
		PlaceId *history = DvGetLastLocations(dv, PLAYER_DRACULA,
                            &num_move, &canFree);
		for (int j  = 0; j < num_move; j++) {
			if (DvGetPlayerLocation(dv, hunter) == history[j])  {
				num++;
				break;
			}
		}
	}
	return num;
}
int hunterhere(DraculaView dv, PlaceId place) {
	int num = 0;
	for (int i = 0; i < 4; i++){
		if (DvGetPlayerLocation(dv, i) == place)  {
			num++;
		}
	}
	return num;
}
void freepath(PlaceId *adress){
	if (adress != NULL) free(adress);
	return;
}

/*PlaceId cloestsea(PlaceId place){
	switch (place){
		case MADRID: return SANTANDER; break;
		case GRANADA: return
	}
}*/
