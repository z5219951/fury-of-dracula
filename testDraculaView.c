////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testDraculaView.c: test the DraculaView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-02	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DraculaView.h"
#include "Game.h"
#include "Places.h"
#include "testUtils.h"

int main(void)
{
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for basic functions, "
			   "just before Dracula's first move\n");

		char *trail =
			"GST.... SAO.... HZU.... MBB....";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "..."
		};
		
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 0);
		assert(DvGetScore(dv) == GAME_START_SCORE);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(DvGetPlayerLocation(dv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(DvGetPlayerLocation(dv, PLAYER_VAN_HELSING) == ZURICH);
		assert(DvGetPlayerLocation(dv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == NOWHERE);
		assert(DvGetVampireLocation(dv) == NOWHERE);
		int numTraps = -1;
		PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
		assert(numTraps == 0);
		free(traps);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for encountering Dracula\n");

		char *trail =
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD.. SAO.... HCD.... MAO....";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahah",
			"Aha!", "", "", ""
		};
		
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 1);
		assert(DvGetScore(dv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		assert(DvGetHealth(dv, PLAYER_LORD_GODALMING) == 5);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == 30);
		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == GENEVA);
		assert(DvGetVampireLocation(dv) == NOWHERE);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula leaving minions 1\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DED.V.. "
			"GST.... SST.... HST.... MST.... DMNT... "
			"GST.... SST.... HST.... MST.... DLOT... "
			"GST.... SST.... HST.... MST.... DHIT... "
			"GST.... SST.... HST.... MST....";
		
		Message messages[24] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 4);
		assert(DvGetVampireLocation(dv) == EDINBURGH);
		int numTraps = -1;
		PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
		assert(numTraps == 3);
		sortPlaces(traps, numTraps);
		assert(traps[0] == LONDON);
		assert(traps[1] == LONDON);
		assert(traps[2] == MANCHESTER);
		free(traps);
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula's valid moves 1\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE....";
		
		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
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

	{///////////////////////////////////////////////////////////////////

		printf("Additional tests for player, vampire and trap locations.\n");

		{

			// Test for just before Dracula's first move

			char *trail =
				"GBU.... SCO.... HDU.... MHA....";
			
			Message messages[] = {};
			DraculaView dv = DvNew(trail, messages);
			
			assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == BRUSSELS);
			assert(DvGetPlayerLocation(dv, PLAYER_DR_SEWARD) == COLOGNE);
			assert(DvGetPlayerLocation(dv, PLAYER_VAN_HELSING) == DUBLIN);
			assert(DvGetPlayerLocation(dv, PLAYER_MINA_HARKER) == HAMBURG);
			assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == NOWHERE);
			assert(DvGetVampireLocation(dv) == NOWHERE);
			int numTraps = -1;
			PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
			assert(numTraps == 0);
			free(traps);
			DvFree(dv);

		}

		{

			// Test for hunter encountering trap, vampire and Dracula

			char *trail =
				"GST.... SAO.... HCD.... MAO.... DGE.V.. "
				"GST.... SAO.... HCD.... MAO.... DGET... "
				"GGETVD. SAO.... HCD.... MAO....";
			
			Message messages[] = {};
			DraculaView dv = DvNew(trail, messages);

			assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == GENEVA);
			assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == GENEVA);
			assert(DvGetVampireLocation(dv) == NOWHERE);
			int numTraps = -1;
			PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
			assert(numTraps == 0);
			free(traps);
			DvFree(dv);
			
		}

		{

			// Test for vampire and trap locations 

			char *trail =
				"GST.... SAO.... HCD.... MAO.... DGE.V.. "
				"GST.... SAO.... HCD.... MAO.... DGET... "
				"GST.... SAO.... HCD.... MAO.... DHIT... "
				"GST.... SAO.... HCD.... MAO.... DPAT... "
				"GGEV... SAO.... HCD.... MAO.... ";
			
			Message messages[] = {};
			DraculaView dv = DvNew(trail, messages);

			assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == GENEVA);
			assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == PARIS);
			assert(DvGetVampireLocation(dv) == NOWHERE);
			int numTraps = -1;
			PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
			assert(numTraps == 3);
			sortPlaces(traps, numTraps);
			assert(traps[0] == GENEVA &&
				   traps[1] == GENEVA &&
				   traps[2] == PARIS);
			free(traps);
			DvFree(dv);

		}

		{

			// Test for traps encountered consectively

			char *trail =
				"GGE.... SGE.... HGE.... MGE.... DED.V.. "
				"GST.... SST.... HST.... MST.... DMNT... "
				"GST.... SST.... HST.... MST.... DLOT... "
				"GST.... SST.... HST.... MST.... DHIT... "
				"GMNT... SLOT... HLOT... MST....";
			
			Message messages[24] = {};
			DraculaView dv = DvNew(trail, messages);

			assert(DvGetVampireLocation(dv) == EDINBURGH);
			int numTraps = -1;
			PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
			assert(numTraps == 0);
			free(traps);
			DvFree(dv);			

		}

		{

			// Test for when Dracula starts at a sea location

			char *trail =
				"GGE.... SGE.... HGE.... MGE.... DNS.... "
				"GST.... SST.... HST.... MST.... DEDT... "
				"GST.... SST.... HST.... MST.... DLOT... "
				"GST.... SST.... HST.... MST.... DHIT... "
				"GST.... SLOT... HLOT... MST....";
			
			Message messages[24] = {};
			DraculaView dv = DvNew(trail, messages);

			assert(DvGetVampireLocation(dv) == NOWHERE);
			int numTraps = -1;
			PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
			assert(numTraps == 1);
			assert(traps[0] == EDINBURGH);
			free(traps);
			DvFree(dv);			

		}

		{

			// Test for when Dracula moves to a sea location

			char *trail =
				"GGE.... SGE.... HGE.... MGE.... DED.V.. "
				"GST.... SST.... HST.... MST.... DNS.... "
				"GST.... SST.... HST.... MST.... DLOT... "
				"GST.... SST.... HST.... MST.... DHIT... "
				"GST.... SLOT... HLOT... MST....";
			
			Message messages[24] = {};
			DraculaView dv = DvNew(trail, messages);

			assert(DvGetVampireLocation(dv) == EDINBURGH);
			int numTraps = -1;
			PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
			assert(numTraps == 0);
			free(traps);
			DvFree(dv);			

		}
		printf("Test passed!\n");

	}

	return EXIT_SUCCESS;

}
