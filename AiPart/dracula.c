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

char* checkmove(DraculaView dv, char *ove);
void freepath(PlaceId *adress);
// set default type
int MYBOAT = 1;
int MYRAIL = 1;
int MYROAD = 1;

void decideDraculaMove(DraculaView dv)
{
	// TODO: Replace this with something better!
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
	// 			HGetShortestPathLength(dv, locHunter, i, &pathLength);
	// 			// printf("%d\n", pathLength);
	// 			if (pathLength > furthestDist) {
	// 				move = i;
	// 				furthestDist = pathLength;
	// 			}
	// 			else if (pathLength == furthestDist) {
	// 				// check loc's distance from Munich
	// 				int distFromMunich_1 = -1;
	// 				HGetShortestPathLength(dv, i, MUNICH, &distFromMunich_1);
	// 				int distFromMunich_2 = -1;
	// 				HGetShortestPathLength(dv, move, MUNICH, &distFromMunich_2);
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
	// int numMove = 0;
	// PlaceId *reachMove = DvGetValidMoves(dv, &numMove);
	// if (numMove == 0) {
	// 	move = placeAbbrevToId("TP");
	// 	strcpy(move_name, placeIdToAbbrev(move));
	// 	registerBestPlay(move_name, "Mwahahahaha");
	// 	return;
	// }

	// // Dracula returning to CD if his health is low
	// if (health < 15) {
	// 	int shortestDist = -1;
	// 	for (int i = 0; reachMove[i] < numMove; i++) {
	// 		int distfromCD = -1;
	// 		HGetShortestPathLength(dv, reachMove[i], CASTLE_DRACULA, &distfromCD);
	// 		if (distfromCD < shortestDist) {
	// 			move = reachMove[i];
	// 		}
	// 	}
	// 	strcpy(move_name, placeIdToAbbrev(move));
	// 	registerBestPlay(move_name, "Mwahahahaha");
	// 	return;
	// }

	// if (reachMove != NULL) {
	// 	int furthestDis = -1;
	// 	for (int i = 0; i < numMove; i++) {
	// 		int pathLength = -1;
	// 		for (Player hunter = PLAYER_LORD_GODALMING; hunter < PLAYER_DRACULA; hunter++){
	// 			HGetShortestPathLength(dv, hunter, i, &pathLength);
	// 			if (pathLength > furthestDis) {
	// 				move = reachMove[i];
	// 				pathLength = furthestDis;
	// 			}
	// 			else if (pathLength == furthestDis) {
	// 				// check loc's distance from Munich
	// 				int distFromMunich_1 = -1;
	// 				HGetShortestPathLength(dv, reachMove[i], MUNICH, &distFromMunich_1);
	// 				int distFromMunich_2 = -1;
	// 				HGetShortestPathLength(dv, move, MUNICH, &distFromMunich_2);
	// 				if (distFromMunich_1 < distFromMunich_2) move = reachMove[i];
	// 			}
	// 		}
	// 	}
	// }

	// free(reachMove);
	// strcpy(move_name, placeIdToAbbrev(move));
	// registerBestPlay(move_name, "Mwahahahaha");

	PlaceId move;
	char move_name[3];

	// if round 0
	if (DvGetRound(dv) == 0) {
		strcpy(move_name, "MU");
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
	int distfromCD = -1;
	freepath(GetShortestPathLength(dv, KLAUSENBURG, NUREMBURG, &distfromCD, false));
	move = placeAbbrevToId("TP");
		strcpy(move_name, placeIdToAbbrev(move));
		registerBestPlay(move_name, "We shall rise from the darkness~");
	return;
	// if Dracula is on low health
	if (DvGetHealth(dv, PLAYER_DRACULA) <= 18) {
		int shortestDist = 100;
		PlaceId location;
		for (int i = 0; i < numMove; i++) {
			int distfromCD = -1;
			if (placeIsReal(reachMove[i])) location = reachMove[i];
			else location = DraculaMove(dv, reachMove[i]);
			FILE *out = fopen("test.out","w");
			fprintf(out, "lo:%s\n", placeIdToAbbrev(location));
			fprintf(out, "lo:%s\n", placeIdToAbbrev(reachMove[i]));
			//freepath(GetShortestPathLength(dv, location, CASTLE_DRACULA, &distfromCD, false));
			if (distfromCD < shortestDist) {
				move = reachMove[i];
				shortestDist = distfromCD;
			}
		}
		strcpy(move_name, placeIdToAbbrev(move));
		registerBestPlay(move_name, "Valor shall rise again");
		return;
	}

	// if there are valid moves
	int furthestDis = -1;
	for (int i = 0; i < numMove; i++) {
		int pathsum = 0;
		PlaceId location;
		if (placeIsReal(reachMove[i])) location = reachMove[i];
		else location = DraculaMove(dv, reachMove[i]);
		for (Player hunter = PLAYER_LORD_GODALMING; hunter < PLAYER_DRACULA; hunter++){
			int pathLength = -1;
			freepath(HGetShortestPathLength(dv, hunter, location, &pathLength));
			pathsum += pathLength;
		}
		if (pathsum > furthestDis) {
			move = reachMove[i];
			pathsum = furthestDis;
		}
		else if (pathsum == furthestDis) {
				//check loc's distance from Munich
			int distFromMunich_1 = -1;
			freepath(GetShortestPathLength(dv, location, MUNICH, &distFromMunich_1, false));
			int distFromMunich_2 = -1;
			freepath(GetShortestPathLength(dv, move, MUNICH, &distFromMunich_2, false));
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
				return move;
			}
	}
	return "vaild";
}

int hunterhere(DraculaView dv, PlaceId place) {
	int num = 0;
	int num_move =0;
	for (int i = 0; i < 4; i++){
		PlaceId *history = DvGetLastLocations(dv, PLAYER_DRACULA,
                            &num_move, false);
		for (int j  = 0; j < num_move; j++) {
			if (DvGetPlayerLocation(dv, i) == history[j])  {
				num++;
				break;
			}
		}
	}
	return num;
}
void freepath(PlaceId *adress){
//	if (adress != NULL) free(adress);
	return;
}
