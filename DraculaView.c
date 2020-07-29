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
	return 0;
}

int DvGetScore(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int DvGetHealth(DraculaView dv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId DvGetVampireLocation(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numTraps = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	// If  Dracula  hasn't  made  a move yet
	if (dv->num < 5) {
		*numReturnedMoves = 0;
		return NULL;
	}
	int round;
	// Get current round
	if (dv->num < 6) {
		round = 1;
	}
	else {
		if (dv->num % 5 == 0) {
			round = dv->num / 5;
		} else {
			round = ((int)(dv->num / 5)) + 1;
		}
	}
	// Get current location of dracula from last round
	char LastMove[1][3];
	LastMove[0][0] = dv->Path[5 * (round - 1) - 1][1];
	LastMove[0][1] = dv->Path[5 * (round - 1) - 1][2];
	LastMove[0][2] = '\0';
	int from = placeAbbrevToId(LastMove[0]);
	// The following array can avoid the same city be added
	int n = MapNumPlaces(dv->map);
	int *repeated_city = calloc(n, sizeof(int));
	// Counter is the number of connection
	int counter = 0;
	ConnList curr = MapGetConnections(dv->map, from);
	while (curr != NULL) {
		counter++;
		curr = curr->next;
	}
	PlaceId *result = malloc(sizeof(PlaceId) * counter);
	// Get past 5 moves of dracula
	char Past5Move[5][3];
	if (round < 6) {
		for (int i = 0, j = 4; i < round - 1; i++, j+=5) {
			Past5Move[i][0] = dv->Path[j][1];
			Past5Move[i][1] = dv->Path[j][2];
			Past5Move[i][2] = '\0';
		}
	} else {
		for (int i = 0, j = round - 5 - 1; i < 5; i++, j++) {
			Past5Move[i][0] = dv->Path[4 + j * 5][1];
			Past5Move[i][1] = dv->Path[4 + j * 5][2];
			Past5Move[i][2] = '\0';
		}
	}

	curr = MapGetConnections(dv->map, from);
	// Check if dracula made hide move or double back in last round
	if (from == HIDE || from == DOUBLE_BACK_1) {
		if (round > 2) {
			char Last2Move[1][3];
			Last2Move[0][0] = dv->Path[5 * (round - 1) - 6][1];
			Last2Move[0][1] = dv->Path[5 * (round - 1) - 6][2];
			Last2Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last2Move[0]));
		}
	}
	else if (from == DOUBLE_BACK_2) {
		if (round > 3) {
			char Last3Move[1][3];
			Last3Move[0][0] = dv->Path[5 * (round - 1) - 11][1];
			Last3Move[0][1] = dv->Path[5 * (round - 1) - 11][2];
			Last3Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last3Move[0]));
		}
	}
	else if (from == DOUBLE_BACK_3) {
		if (round > 4) {
			char Last4Move[1][3];
			Last4Move[0][0] = dv->Path[5 * (round - 1) - 16][1];
			Last4Move[0][1] = dv->Path[5 * (round - 1) - 16][2];
			Last4Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last4Move[0]));
		}
	}
	else if (from == DOUBLE_BACK_4) {
		if (round > 5) {
			char Last5Move[1][3];
			Last5Move[0][0] = dv->Path[5 * (round - 1) - 21][1];
			Last5Move[0][1] = dv->Path[5 * (round - 1) - 21][2];
			Last5Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last5Move[0]));
		}
	}
	else if (from == DOUBLE_BACK_5) {
		if (round > 6) {
			char Last6Move[1][3];
			Last6Move[0][0] = dv->Path[5 * (round - 1) - 21][1];
			Last6Move[0][1] = dv->Path[5 * (round - 1) - 21][2];
			Last6Move[0][2] = '\0';
			curr = MapGetConnections(dv->map, placeAbbrevToId(Last6Move[0]));
		}
	}

	int index = 0;
	int round_temp;
	// Check whether a connected city satisfy condition
	while (curr != NULL) {
		// Dracula can not go to hospital
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
		bool hasRepeatedHide = false;
		bool hasRepeatedDB = false;
		
		if (round < 6) {
			round_temp = round - 1;
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
		// If dracula did not made hide move in the past 5 rounds
		if (!hasRepeatedHide) {
			if (!placeIsSea(from)) {
				result[index++] = HIDE;
				result[index] = '\0';
			}
		} 
		// If dracula did not made double back in the past 5 rounds
		if (!hasRepeatedDB) {
			if (round > 1) result[index++] = DOUBLE_BACK_1;
			if (round > 2) result[index++] = DOUBLE_BACK_2;
			if (round > 3) result[index++] = DOUBLE_BACK_3;
			if (round > 4) result[index++] = DOUBLE_BACK_4;
			if (round > 5) result[index++] = DOUBLE_BACK_5;
			result[index] = '\0';
		}
		// Skip to next adjacent city if dracula made the same move in the past 5 rounds
		if (hasRepeatedMove) {
			curr=curr->next;
			continue;
		}
		// If current place satisfy condition
		if ((curr->type == ROAD || curr->type == BOAT) && repeated_city[curr->p] == 0)  {
			result[index++] = curr->p;
			result[index] = '\0';
			repeated_city[curr->p] = 1;
		}
		curr = curr->next;
	}
	// If there is no legal move, dracula teleports to castel dracula
	if (index == 0) {
		*numReturnedMoves = 1;
		result[0] = TELEPORT;
		return result;
	}
	*numReturnedMoves = GetLenOfList(result);
	return result;
	
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	if (dv->num < 5) {
		*numReturnedLocs = 0;
		return NULL;
	}
	int round;
	// Get current round
	if (dv->num < 6) {
		round = 1;
	}
	else {
		if (dv->num % 5 == 0) {
			round = dv->num / 5;
		} else {
			round = ((int)(dv->num / 5)) + 1;
		}
	}
	// Get current location of dracula from last round
	char LastMove[1][3];
	LastMove[0][0] = dv->Path[5 * (round - 1) - 1][1];
	LastMove[0][1] = dv->Path[5 * (round - 1) - 1][2];
	LastMove[0][2] = '\0';
	int from = placeAbbrevToId(LastMove[0]);
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
	PlaceId *result = malloc(sizeof(PlaceId) * counter);
	// Get past 5 moves of dracula
	char Past5Move[5][3];
	if (round < 6) {
		for (int i = 0, j = 4; i < round - 1; i++, j+=5) {
			Past5Move[i][0] = dv->Path[j][1];
			Past5Move[i][1] = dv->Path[j][2];
			Past5Move[i][2] = '\0';
		}
	} else {
		for (int i = 0, j = round - 5 - 1; i < 5; i++, j++) {
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
		if (round < 6) {
			round_temp = round - 1;
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
				result[index] = '\0';
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
	if (dv->num < 5) {
		*numReturnedLocs = 0;
		return NULL;
	}
	int round;
	// Get current round
	if (dv->num < 6) {
		round = 1;
	}
	else {
		if (dv->num % 5 == 0) {
			round = dv->num / 5;
		} else {
			round = ((int)(dv->num / 5)) + 1;
		}
	}
	// Get current location of dracula from last round
	char LastMove[1][3];
	LastMove[0][0] = dv->Path[5 * (round - 1) - 1][1];
	LastMove[0][1] = dv->Path[5 * (round - 1) - 1][2];
	LastMove[0][2] = '\0';
	int from = placeAbbrevToId(LastMove[0]);
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
	PlaceId *result = malloc(sizeof(PlaceId) * counter);
	// Get past 5 moves of dracula
	char Past5Move[5][3];
	if (round < 6) {
		for (int i = 0, j = 4; i < round - 1; i++, j+=5) {
			Past5Move[i][0] = dv->Path[j][1];
			Past5Move[i][1] = dv->Path[j][2];
			Past5Move[i][2] = '\0';
		}
	} else {
		for (int i = 0, j = round - 5 - 1; i < 5; i++, j++) {
			Past5Move[i][0] = dv->Path[4 + j * 5][1];
			Past5Move[i][1] = dv->Path[4 + j * 5][2];
			Past5Move[i][2] = '\0';
		}
	}

	curr = MapGetConnections(dv->map, from);
	int index = 0;
	int round_temp;
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
		if (round < 6) {
			round_temp = round - 1;
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
		// If current place satisfy conditon
		if (curr->type == ROAD  && road == true && repeated_city[curr->p] == 0) {
			result[index++] = curr->p;
			result[index] = '\0';
			repeated_city[curr->p] = 1;

		} else if (curr->type == BOAT && boat == true && repeated_city[curr->p] == 0) {
			result[index++] = curr->p;
			result[index] = '\0';
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
	int round;
	// Get current round
	if (dv->num < 6) {
		round = 1;
	}
	else {
		if (dv->num % 5 == 0) {
			round = dv->num / 5;
		} else {
			round = ((int)(dv->num / 5)) + 1;
		}
	}
	int n = MapNumPlaces(dv->map);
	// Two cities may be connected through multiple ways
	// The following array is to avoid the same city be added
	int *repeated_city = calloc(n, sizeof(int));
	// If current player is dracula
	if (player == PLAYER_DRACULA) {
		if (dv->num < 5) {
			*numReturnedLocs = 0;
			return NULL;
		}
		// Get current location of dracula from last round
		char LastMove[1][3];
		LastMove[0][0] = dv->Path[5 * (round - 1) - 1][1];
		LastMove[0][1] = dv->Path[5 * (round - 1) - 1][2];
		LastMove[0][2] = '\0';
		int from = placeAbbrevToId(LastMove[0]);
		// Counter is the number of connection
		int counter = 0;
		ConnList curr = MapGetConnections(dv->map, from);
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * counter);
		// Get past 5 moves of dracula
		char Past5Move[5][3];
		if (round < 6) {
			for (int i = 0, j = 4; i < round - 1; i++, j+=5) {
				Past5Move[i][0] = dv->Path[j][1];
				Past5Move[i][1] = dv->Path[j][2];
				Past5Move[i][2] = '\0';
			}
		} else {
			for (int i = 0, j = round - 5 - 1; i < 5; i++, j++) {
				Past5Move[i][0] = dv->Path[4 + j * 5][1];
				Past5Move[i][1] = dv->Path[4 + j * 5][2];
				Past5Move[i][2] = '\0';
			}
		}

		curr = MapGetConnections(dv->map, from);
		int index = 0;
		int round_temp;
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
		if (round < 6) {
			round_temp = round - 1;
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
		// If current place satisfy condition
		if ((curr->type == ROAD || curr->type == BOAT) && repeated_city[curr->p] == 0)  {
			result[index++] = curr->p;
			result[index] = '\0';
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
		char LastMove[1][3];
		int from;
		if (player == PLAYER_LORD_GODALMING) {
			// If current player has not made a move yet
			if (dv->num < 1) {
				*numReturnedLocs = 0;
				return NULL;
			}
			LastMove[0][0] = dv->Path[5 * (round - 2)][1];
			LastMove[0][1] = dv->Path[5 * (round - 2)][2];
			LastMove[0][2] = '\0';
			from = placeAbbrevToId(LastMove[0]);
		} 
		else if (player == PLAYER_DR_SEWARD) {
			// If current player has not made a move yet
			if (dv->num < 2) {
				*numReturnedLocs = 0;
				return NULL;
			}
			LastMove[0][0] = dv->Path[5 * (round - 2) + 1][1];
			LastMove[0][1] = dv->Path[5 * (round - 2) + 1][2];
			LastMove[0][2] = '\0';
			from = placeAbbrevToId(LastMove[0]);
		} 
		else if (player == PLAYER_VAN_HELSING) {
			// If current player has not made a move yet
			if (dv->num < 3) {
				*numReturnedLocs = 0;
				return NULL;
			}
			LastMove[0][0] = dv->Path[5 * (round - 2) + 2][1];
			LastMove[0][1] = dv->Path[5 * (round - 2) + 2][2];
			LastMove[0][2] = '\0';
			from = placeAbbrevToId(LastMove[0]);
		} 
		else if (player == PLAYER_MINA_HARKER) {
			// If current player has not made a move yet
			if (dv->num < 4) {
				*numReturnedLocs = 0;
				return NULL;
			}
			LastMove[0][0] = dv->Path[5 * (round - 2) + 3][1];
			LastMove[0][1] = dv->Path[5 * (round - 2) + 3][2];
			LastMove[0][2] = '\0';
			from = placeAbbrevToId(LastMove[0]);
		}
		ConnList curr = MapGetConnections(dv->map, from);
		// Counter is the number of connection
		int counter = 0;
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * counter);
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
				result[index] = '\0';
				repeated_city[curr->p] = 1;
			}
			if (curr->type == BOAT) {
				// From sea to sea or port city
				if (placeIsSea(from)) {
					result[index++] = curr->p;
					result[index] = '\0';
					repeated_city[curr->p] = 1;
				}
				// From port city to adjacent sea
				if (placeIsLand(from) && placeIsSea(curr->p)) {
					result[index++] = curr->p;
					result[index] = '\0';
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

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	int round;
	// Get current round
	if (dv->num < 6) {
		round = 1;
	}
	else {
		if (dv->num % 5 == 0) {
			round = dv->num / 5;
		} else {
			round = ((int)(dv->num / 5)) + 1;
		}
	}
	int n = MapNumPlaces(dv->map);
	// Two cities may be connected through multiple ways
	// The following array is to avoid the same city be added
	int *repeated_city = calloc(n, sizeof(int));
	// If current player is dracula
	if (player == PLAYER_DRACULA) {
		if (dv->num < 5) {
			*numReturnedLocs = 0;
			return NULL;
		}
		// Get current location of dracula from last round
		char LastMove[1][3];
		LastMove[0][0] = dv->Path[5 * (round - 1) - 1][1];
		LastMove[0][1] = dv->Path[5 * (round - 1) - 1][2];
		LastMove[0][2] = '\0';
		int from = placeAbbrevToId(LastMove[0]);
		// Counter is the number of connection
		int counter = 0;
		ConnList curr = MapGetConnections(dv->map, from);
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * counter);
		// Get past 5 moves of dracula
		char Past5Move[5][3];
		if (round < 6) {
			for (int i = 0, j = 4; i < round - 1; i++, j+=5) {
				Past5Move[i][0] = dv->Path[j][1];
				Past5Move[i][1] = dv->Path[j][2];
				Past5Move[i][2] = '\0';
			}
		} else {
			for (int i = 0, j = round - 5 - 1; i < 5; i++, j++) {
				Past5Move[i][0] = dv->Path[4 + j * 5][1];
				Past5Move[i][1] = dv->Path[4 + j * 5][2];
				Past5Move[i][2] = '\0';
			}
		}

		curr = MapGetConnections(dv->map, from);
		int index = 0;
		int round_temp;
		// Check whether a adjacent place satisfy condition
		while(curr != NULL) {
		// Dracula can not move to hospital
		if (curr->p == ST_JOSEPH_AND_ST_MARY) {
			curr = curr->next;
			continue;
		}
		// Dracula can not move through rail
		if (curr->type == RAIL) {
			curr = curr->next;
			continue;
		}
		bool hasRepeatedMove = false;
		if (round < 6) {
			round_temp = round - 1;
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
		// If current place satisfy condition
		if (curr->type == ROAD && road == true && repeated_city[curr->p] == 0) {
			result[index++] = curr->p;
			result[index] = '\0';
			repeated_city[curr->p] = 1;
		}
		else if (curr->type == BOAT && boat == true && repeated_city[curr->p] == 0) {
			result[index++] = curr->p;
			result[index] = '\0';
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
		char LastMove[1][3];
		int from;
		if (player == PLAYER_LORD_GODALMING) {
			// If current player has not made a move yet
			if (dv->num < 1) {
				*numReturnedLocs = 0;
				return NULL;
			}
			LastMove[0][0] = dv->Path[5 * (round - 2)][1];
			LastMove[0][1] = dv->Path[5 * (round - 2)][2];
			LastMove[0][2] = '\0';
			from = placeAbbrevToId(LastMove[0]);
		} 
		else if (player == PLAYER_DR_SEWARD) {
			// If current player has not made a move yet
			if (dv->num < 2) {
				*numReturnedLocs = 0;
				return NULL;
			}
			LastMove[0][0] = dv->Path[5 * (round - 2) + 1][1];
			LastMove[0][1] = dv->Path[5 * (round - 2) + 1][2];
			LastMove[0][2] = '\0';
			from = placeAbbrevToId(LastMove[0]);
		} 
		else if (player == PLAYER_VAN_HELSING) {
			// If current player has not made a move yet
			if (dv->num < 3) {
				*numReturnedLocs = 0;
				return NULL;
			}
			LastMove[0][0] = dv->Path[5 * (round - 2) + 2][1];
			LastMove[0][1] = dv->Path[5 * (round - 2) + 2][2];
			LastMove[0][2] = '\0';
			from = placeAbbrevToId(LastMove[0]);
		} 
		else if (player == PLAYER_MINA_HARKER) {
			// If current player has not made a move yet
			if (dv->num < 4) {
				*numReturnedLocs = 0;
				return NULL;
			}
			LastMove[0][0] = dv->Path[5 * (round - 2) + 3][1];
			LastMove[0][1] = dv->Path[5 * (round - 2) + 3][2];
			LastMove[0][2] = '\0';
			from = placeAbbrevToId(LastMove[0]);
		}
		ConnList curr = MapGetConnections(dv->map, from);
		// Counter is the number of connection
		int counter = 0;
		while (curr != NULL) {
			counter++;
			curr = curr->next;
		}
		PlaceId *result = malloc(sizeof(PlaceId) * counter);
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
			if (curr->type == ROAD && road == true) {
				result[index++] = curr->p;
				result[index] = '\0';
				repeated_city[curr->p] = 1;
			}
			if (curr->type == BOAT && boat == true) {
				// From sea to sea or port city
				if (placeIsSea(from)) {
					result[index++] = curr->p;
					result[index] = '\0';
					repeated_city[curr->p] = 1;
				}
				// From port city to adjacent sea
				if (placeIsLand(from) && placeIsSea(curr->p)) {
					result[index++] = curr->p;
					result[index] = '\0';
					repeated_city[curr->p] = 1;
				}
			}
			curr = curr->next;
		}
		if (RailList != NULL && rail == true) {
			result = MergeList(result, RailList);
		}
		*numReturnedLocs = GetLenOfList(result);
		return result;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
// get the number of element in an array