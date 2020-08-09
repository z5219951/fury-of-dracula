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
char* checkmove(DraculaView dv, char *move);
int hunterhere(DraculaView dv, PlaceId place);
// set default type
int MYBOAT = 1;
int MYRAIL = 1;
int MYROAD = 1;

void decideDraculaMove(DraculaView dv)
{
	// // TODO: Replace this with something better!

	PlaceId move;
	char move_name[3];
	// if round 0
	if (DvGetRound(dv) == 0) {
		PlaceId rplace = placeAbbrevToId("CD");
		if (hunterhere(dv, CASTLE_DRACULA) != 0) {
			srand(time(0));
			PlaceId rplace = MIN_REAL_PLACE + rand() % MAX_REAL_PLACE;
			while (rplace == HOSPITAL_PLACE
			|| rplace == CASTLE_DRACULA)
			rplace = rand() % MAX_REAL_PLACE;
		}
		strcpy(move_name, placeIdToAbbrev(rplace));
		registerBestPlay(move_name, "Let darkness be parted!");
		return;
	}
	int numMove= 0;
	PlaceId *reachMove = DvGetValidMoves(dv, &numMove);
	// if no valid moves
	if (numMove == 0) {
		move = placeAbbrevToId("TP");
		strcpy(move_name, placeIdToAbbrev(move));
		registerBestPlay(move_name, "We shall rise from the darkness~");
		return;
	}
	// if there are valid moves
	srand(time(0));
	strcpy(move_name, placeIdToAbbrev(reachMove[rand() % numMove]));
	if (strcmp(checkmove(dv, move_name), "vaild") != 0)
	strcpy(move_name, placeIdToAbbrev(reachMove[0]));
	registerBestPlay(move_name, "It is density, I give up");
	free(reachMove);
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
int hunterhere(DraculaView dv, PlaceId place) {
	int num = 0;
	for (int i = 0; i < 4; i++){
		if (DvGetPlayerLocation(dv, i) == place)  {
			num++;
		}
	}
	return num;
}
