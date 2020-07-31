#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Places.h"
#include "testUtils.h"
#include "Queue.h"
#include "Map.h"
struct hunterView {
	// TODO: ADD FIELDS HERE
	char **Path;
	int num;
	Map map;
};
int main (void)
{
    {///////////////////////////////////////////////////////////////////
	
		printf("Checking Galatz road connections "
		       "(Lord Godalming, Round 1)\n");
		
		char *trail = "GGA....";
		Message messages[1] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGoByType(hv, PLAYER_LORD_GODALMING,
		                                       true, false, false, &numLocs);
		
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BUCHAREST);
		assert(locs[1] == CASTLE_DRACULA);
		assert(locs[2] == CONSTANTA);
		assert(locs[3] == GALATZ);
		assert(locs[4] == KLAUSENBURG);
		free(locs);
		
		HvFree(hv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Checking Paris rail connections "
		       "(Lord Godalming, Round 1)\n");
		
		char *trail = "GPA....";
		Message messages[1] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGoByType(hv, PLAYER_LORD_GODALMING,
		                                       false, true, false, &numLocs);
		printf("numLocs = %d\n", numLocs);
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BORDEAUX);
		assert(locs[1] == BRUSSELS);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == MARSEILLES);
		assert(locs[4] == PARIS);
		free(locs);
		
		HvFree(hv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Checking Adriatic Sea boat connections "
		       "(Lord Godalming, Round 1)\n");
		
		char *trail = "GAS....";
		Message messages[1] = {};
		HunterView hv = HvNew(trail, messages);                          
      
		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGoByType(hv, PLAYER_LORD_GODALMING,
		                                       false, false, true, &numLocs);
        printf("\nnumLocs = %d\n", numLocs);
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == ADRIATIC_SEA);
		assert(locs[1] == BARI);
		assert(locs[2] == IONIAN_SEA);
		assert(locs[3] == VENICE);

		free(locs);
		
		HvFree(hv);
		printf("Test passed!\n");
	} 
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Checking Szeged road connections "
		       "(Dracula, Round 1)\n");
		
		char *trail =
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V..";
		
		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGoByType(hv, PLAYER_DRACULA,
		                                       true, false, false, &numLocs);
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == BUDAPEST);
		assert(locs[2] == KLAUSENBURG);
		assert(locs[3] == SZEGED);
		assert(locs[4] == ZAGREB);
		free(locs);
		
		HvFree(hv);
		printf("Test passed!\n");	
	}

}