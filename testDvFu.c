#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "GameView.h"
#include "DraculaView.h"
#include "Places.h"
#include "testUtils.h"
#include "Queue.h"
#include "Map.h"
struct draculaView {
	// TODO: ADD FIELDS HERE
	char **Path;
	int num;
	Map map;
};
int main (void)
{
    {///////////////////////////////////////////////////////////////////
	
	printf("Test for Dracula's valid moves 1\n");
		
	char *trail =
		"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
		"GGE.... SGE.... HGE.... MGE....";
		
	Message messages[9] = {};
	DraculaView dv = DvNew(trail, messages);
		
	int numMoves = -1;
	PlaceId *moves = DvGetValidMoves(dv, &numMoves);
    printf("%d\n", numMoves);
    for (int i = 0; i < numMoves; i++) {
        printf("%s(%d) ",placeIdToName(moves[i]), i + 1);
    }
	assert(numMoves == 4);
	sortPlaces(moves, numMoves);
	assert(moves[0] == GALATZ);
	assert(moves[1] == KLAUSENBURG);
	assert(moves[2] == HIDE);
	assert(moves[3] == DOUBLE_BACK_1);
	free(moves);
		
	printf("Test passed!\n");
	DvFree(dv);
	}
    
    {///////////////////////////////////////////////////////////////////
	
	printf("Test for DvWhereCanIGo 1\n");
		
	char *trail =
		"GGE.... SGE.... HGE.... MGE.... DKL.V.. "
		"GGE.... SGE.... HGE.... MGE.... DD1T... "
		"GGE.... SGE.... HGE.... MGE.... DBCT... "
		"GGE.... SGE.... HGE.... MGE.... DHIT... "
		"GGE.... SGE.... HGE.... MGE....";
		
	Message messages[24] = {};
	DraculaView dv = DvNew(trail, messages);
		
	int numLocs = -1;
	PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
	assert(numLocs == 4);
	sortPlaces(locs, numLocs);
	assert(locs[0] == BELGRADE);
	assert(locs[1] == CONSTANTA);
	assert(locs[2] == GALATZ);
	assert(locs[3] == SOFIA);
	free(locs);
		
	printf("Test passed!\n");
	DvFree(dv);
	}
}