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
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BORDEAUX);
		assert(locs[1] == BRUSSELS);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == MARSEILLES);
		assert(locs[4] == PARIS);
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
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == ADRIATIC_SEA);
		assert(locs[1] == BARI);
		assert(locs[2] == IONIAN_SEA);
		assert(locs[3] == VENICE);
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
		printf("Test passed!\n");	
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Cheking HvWherecanTheyGo(Dracula) 1\n");
		
		char *trail =
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V..";
		
		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGo(hv, PLAYER_DRACULA, &numLocs);
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == BUDAPEST);
		assert(locs[2] == KLAUSENBURG);
		assert(locs[3] == SZEGED);
		assert(locs[4] == ZAGREB);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Cheking HvWherecanTheyGo(Dracula) 2\n");
		
		char *trail =
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V.. "
			"GSZ.... SGE.... HGE.... MGE.... DBCT...";
		
		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGo(hv, PLAYER_DRACULA, &numLocs);
		assert(numLocs == 6);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == BUCHAREST);
		assert(locs[2] == CONSTANTA);
		assert(locs[3] == GALATZ);
		assert(locs[4] == KLAUSENBURG);
		assert(locs[5] == SOFIA);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Cheking HvWherecanTheyGo(Dracula) 3\n");
		
		char *trail =
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V.. "
			"GSZ.... SGE.... HGE.... MGE.... DBCT... "
			"GSZ.... SGE.... HGE.... MGE.... DCNT...";
		
		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGo(hv, PLAYER_DRACULA, &numLocs);
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BLACK_SEA);
		assert(locs[1] == CONSTANTA);
		assert(locs[2] == GALATZ);
		assert(locs[3] == VARNA);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Cheking HvWherecanTheyGo(Dracula) 4\n");
		
		char *trail =
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V.. "
			"GSZ.... SGE.... HGE.... MGE.... DBCT... "
			"GSZ.... SGE.... HGE.... MGE.... DCNT... "
			"GSZ.... SGE.... HGE.... MGE.... DBST...";
		
		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGo(hv, PLAYER_DRACULA, &numLocs);
		assert(numLocs == 3);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BLACK_SEA);
		assert(locs[1] == IONIAN_SEA);
		assert(locs[2] == VARNA);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Cheking HvWherecanTheyGo(Dracula) 5\n");
		
		char *trail =
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V.. "
			"GSZ.... SGE.... HGE.... MGE.... DBCT... "
			"GSZ.... SGE.... HGE.... MGE.... DCNT... "
			"GSZ.... SGE.... HGE.... MGE.... DBST... "
			"GSZ.... SGE.... HGE.... MGE.... DHIT...";
		
		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGo(hv, PLAYER_DRACULA, &numLocs);
		assert(numLocs == 3);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BLACK_SEA);
		assert(locs[1] == IONIAN_SEA);
		assert(locs[2] == VARNA);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Cheking HvWherecanTheyGo(Dracula) 6\n");
		
		char *trail =
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V.. "
			"GSZ.... SGE.... HGE.... MGE.... DBCT... "
			"GSZ.... SGE.... HGE.... MGE.... DCNT... "
			"GSZ.... SGE.... HGE.... MGE.... DBST... "
			"GSZ.... SGE.... HGE.... MGE.... DD3T...";
		
		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGo(hv, PLAYER_DRACULA, &numLocs);
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == BUCHAREST);
		assert(locs[2] == GALATZ);
		assert(locs[3] == KLAUSENBURG);
		assert(locs[4] == SOFIA);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Cheking HvWherecanTheyGoByType(Hunter) 1\n");
		
		char *trail =
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V.. "
			"GBD.... SGE.... HGE.... MGE.... DBCT... "
			"GVI.... SGE.... HGE.... MGE.... DCNT... "
			"GPR.... SGE.... HGE.... MGE.... DBST...";
		
		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanTheyGoByType(hv, PLAYER_LORD_GODALMING, false, true, false, &numLocs);
		assert(numLocs == 1);
		sortPlaces(locs, numLocs);
		assert(locs[0] == PRAGUE);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Cheking HvWhereCanIGo(Hunter) 4\n");
		
		char *trail =
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V.. "
			"GBD.... SGE.... HGE.... MGE.... DBCT... "
			"GVI.... SGE.... HGE.... MGE.... DCNT... "
			"GPR.... SGE.... HGE.... MGE.... DBST...";
		
		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanIGo(hv, &numLocs);
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BERLIN);
		assert(locs[1] == NUREMBURG);
		assert(locs[2] == PRAGUE);
		assert(locs[3] == VIENNA);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Cheking HvWhereCanIGoByType(Hunter) 1\n");
		
		char *trail =
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V..";
		
		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = HvWhereCanIGoByType(hv, false, false, false, &numLocs);
		assert(numLocs == 1);
		sortPlaces(locs, numLocs);
		assert(locs[0] == SZEGED);
		printf("Test passed!\n");
	}
}