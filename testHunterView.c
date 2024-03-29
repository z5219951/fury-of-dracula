////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testHunterView.c: test the HunterView ADT
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

#include "Game.h"
#include "HunterView.h"
#include "Places.h"
#include "testUtils.h"

int main(void)
{
	{///////////////////////////////////////////////////////////////////
	
		printf("Basic initialisation\n");
		
		char *trail = "";
		Message messages[] = {};
		HunterView hv = HvNew(trail, messages);

		assert(HvGetRound(hv) == 0);
		assert(HvGetPlayer(hv) == PLAYER_LORD_GODALMING);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(HvGetVampireLocation(hv) == NOWHERE);

		printf("Test passed\n");
		HvFree(hv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("After Lord Godalming's turn\n");

		char *trail =
			"GST....";
		
		Message messages[1] = {};
		HunterView hv = HvNew(trail, messages);

		assert(HvGetRound(hv) == 0);
		assert(HvGetPlayer(hv) == PLAYER_DR_SEWARD);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == NOWHERE);

		printf("Test passed!\n");
		HvFree(hv);
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("After Van Helsing's turn\n");

		char *trail =
			"GST.... SAO.... HZU....";
		
		Message messages[3] = {};
		HunterView hv = HvNew(trail, messages);

		assert(HvGetRound(hv) == 0);
		assert(HvGetPlayer(hv) == PLAYER_MINA_HARKER);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == ZURICH);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == NOWHERE);

		HvFree(hv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("After Dracula's turn\n");

		char *trail =
			"GST.... SAO.... HZU.... MBB.... DC?.V..";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahahaha"
		};
		
		HunterView hv = HvNew(trail, messages);

		assert(HvGetRound(hv) == 1);
		assert(HvGetPlayer(hv) == PLAYER_LORD_GODALMING);
		assert(HvGetScore(hv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == ZURICH);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(HvGetVampireLocation(hv) == CITY_UNKNOWN);
		Round round = -1;
		assert(HvGetLastKnownDraculaLocation(hv, &round) == NOWHERE);

		HvFree(hv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Encountering Dracula\n");

		char *trail =
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahahaha",
			"Aha!"
		};
		
		HunterView hv = HvNew(trail, messages);
		
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) ==
				GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
		assert(HvGetHealth(hv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == GENEVA);
		assert(HvGetVampireLocation(hv) == NOWHERE);
		Round round = -1;
		assert(HvGetLastKnownDraculaLocation(hv, &round) == GENEVA);
		assert(round == 0);

		HvFree(hv);
		printf("Test passed\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula doubling back at sea, "
			   "and losing blood points\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DD1....";
		
		Message messages[] = {
			"Party at Geneva", "Okay", "Sure", "Let's go", "Mwahahahaha",
			"", "", "", "", "Back I go"
		};
		
		HunterView hv = HvNew(trail, messages);

		assert(HvGetRound(hv) == 2);
		assert(HvGetPlayer(hv) == PLAYER_LORD_GODALMING);
		assert(HvGetScore(hv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		assert(HvGetHealth(hv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == SEA_UNKNOWN);

		HvFree(hv);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing a hunter 'dying'\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GSTTTTD";
		
		Message messages[21] = {};
		HunterView hv = HvNew(trail, messages);
		
		assert(HvGetScore(hv) == GAME_START_SCORE
		                         - 4 * SCORE_LOSS_DRACULA_TURN
		                         - SCORE_LOSS_HUNTER_HOSPITAL);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == 0);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == HOSPITAL_PLACE);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == STRASBOURG);
		
		HvFree(hv);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing Dracula doubling back to Castle Dracula\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T...";
		
		Message messages[10] = {};
		HunterView hv = HvNew(trail, messages);
		
		assert(HvGetHealth(hv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS + (2 * LIFE_GAIN_CASTLE_DRACULA));
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CASTLE_DRACULA);
		
		HvFree(hv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing vampire location\n");
		char *trail =
			"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
			"GBD.... SGE.... HGE.... MGE.... DC?T... "
			"GSZ.... SGE.... HGE.... MGE.... DC?T... "
			"GSZ.... SGE.... HGE....";
		
		Message messages[15] = {};
		HunterView hv = HvNew(trail, messages);
		
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(HvGetVampireLocation(hv) == CASTLE_DRACULA);
		
		HvFree(hv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing a vampire maturing\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T.V.";
		
		Message messages[35] = {};
		HunterView hv = HvNew(trail, messages);
		
		assert(HvGetScore(hv) == GAME_START_SCORE
		                         - 7 * SCORE_LOSS_DRACULA_TURN
		                         - SCORE_LOSS_VAMPIRE_MATURES);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(HvGetVampireLocation(hv) == NOWHERE);
		
		HvFree(hv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing Dracula's last known location 1\n");
		
		char *trail =
			"GGE.... SGE.... HVI.... MGE.... DCD.V.. "
			"GGE.... SGE.... HBD.... MGE.... DKLT... "
			"GGE.... SGE.... HSZ.... MGE.... DC?T... "
			"GGE.... SGE.... HKLT... MGE.... DC?T... "
			"GGE.... SGE.... HCDV... MGE.... DD1T...";
	
		Message messages[25] = {};
		HunterView hv = HvNew(trail, messages);
		
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		Round round = -1;
		assert(HvGetLastKnownDraculaLocation(hv, &round) == KLAUSENBURG);
		assert(round == 1);
		
		HvFree(hv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
		
		printf("Testing shortest path 1\n");
		
		char *trail =
			"GLS.... SLS.... HSW.... MMR.... DCD.V..";
		
		Message messages[5] = {};
		HunterView hv = HvNew(trail, messages);
		
		{
			printf("\tLisbon -> Barcelona (Lord Godalming, Round 1)\n");
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_LORD_GODALMING,
			                                    BARCELONA, &pathLength);
			assert(pathLength == 2);
			assert(path[0] == MADRID);
			assert(path[1] == BARCELONA);
			free(path);
		}
		
		{
			printf("\tLisbon -> Cologne (Lord Godalming, Round 1)\n");
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_LORD_GODALMING,
			                                    COLOGNE, &pathLength);
			assert(pathLength == 3);
			assert(path[0] == MADRID);
			assert(path[1] == BORDEAUX);
			assert(path[2] == COLOGNE);
			free(path);
		}
		
		{
			printf("\tSwansea -> Hamburg (Van Helsing, Round 1)\n");
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_VAN_HELSING,
			                                    HAMBURG, &pathLength);
			assert(pathLength == 3);
			assert(path[0] == EDINBURGH);
			assert(path[1] == NORTH_SEA);
			assert(path[2] == HAMBURG);
			free(path);
		}
		
		{
			printf("\tMarseilles -> Constanta (Mina Harker, Round 1)\n");
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_MINA_HARKER,
			                                    CONSTANTA, &pathLength);
			assert(pathLength == 4);
			assert(path[0] == GENOA);
			assert(path[1] == VENICE);
			assert(path[2] == BUDAPEST);
			assert(path[3] == CONSTANTA);
			free(path);
		}
		
		{
			printf("\tLisbon -> Castle Dracula (Dr. Seward, Round 1)\n");
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_DR_SEWARD,
			                                    CASTLE_DRACULA, &pathLength);
			assert(pathLength == 7);
			assert(path[0] == SARAGOSSA);
			assert(path[1] == MARSEILLES);
			assert(path[2] == GENOA);
			assert(path[3] == VENICE);
			assert(path[4] == BUDAPEST);
			assert(path[5] == GALATZ);
			assert(path[6] == CASTLE_DRACULA);
			free(path);
		}
		
		HvFree(hv);
		printf("Test passed!\n");
	}
	
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

	{
		printf("Additional tests for HvGetLastKnownDraculaLocation & HvGetShortestPathTo");
		{///////////////////////////////////////////////////////////////////
			printf("HvGetShortestPathTo Part\n");
			printf("Round 0\n");

			char *trail =
				"GGE....";
			
			Message messages[] = {
				"Hello"
			};
			
			HunterView hv = HvNew(trail, messages);

			
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_DR_SEWARD,
			                                    CASTLE_DRACULA, &pathLength);
			assert(path[0] == CASTLE_DRACULA);

			HvFree(hv);
			printf("Test passed!\n");
		}

		{///////////////////////////////////////////////////////////////////
	
			printf("Go to current place\n");

			char *trail =
				"GCD....";
			
			Message messages[] = {
				"Hello"
			};
			
			HunterView hv = HvNew(trail, messages);

			
			int pathLength = -1;
			PlaceId *path = HvGetShortestPathTo(hv, PLAYER_LORD_GODALMING,
			                                    CASTLE_DRACULA, &pathLength);
			assert(path[0] == CASTLE_DRACULA);

			HvFree(hv);
			printf("Test passed!\n");
		}

		{////////////////////////////////////////////////////////////

			printf("HvGetLastKnownDraculaLocation Part\n");
			printf("Round 0\n");

			char *trail =
				"";
			
			Message messages[] = {};
			
			HunterView hv = HvNew(trail, messages);

			
			Round round = -1;
			assert(HvGetLastKnownDraculaLocation(hv, &round) == NOWHERE);

			HvFree(hv);
			printf("Test passed!\n");
		}

		{////////////////////////////////////////////////////////////

			printf("HvGetLastKnownDraculaLocation Part\n");
			printf("Check Teleport\n");

			char *trail =
				"GGE.... SGE.... HVI.... MGE.... DCD.V.. "
				"GGE.... SGE.... HBD.... MGE.... DKLT... "
				"GGE.... SGE.... HSZ.... MGE.... DC?T... "
				"GGE.... SGE.... HKLT... MGE.... DTPT... "
				"GGE.... SGE.... HCDV... MGE.... DD1T...";
		
			Message messages[25] = {};
			HunterView hv = HvNew(trail, messages);
			
			Round round = -1;
			assert(HvGetLastKnownDraculaLocation(hv, &round) == CASTLE_DRACULA);
			assert(round == 4);

			HvFree(hv);
			printf("Test passed!\n");
		}
	}

	{////////////////////////////////////////////////////////////

		printf("Additional tests for player and vampire locations.\n");

		{

			// testing empty play history

			char *trail = "";

			Message messages[] = {};
			HunterView hv = HvNew(trail, messages);

			assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == NOWHERE);
			assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == NOWHERE);
			assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == NOWHERE);
			assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == NOWHERE);
			assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == NOWHERE);
			assert(HvGetVampireLocation(hv) == NOWHERE);
			HvFree(hv);
			
		}

		{

			// testing after first play

			char *trail =
				"GST....";
			
			Message messages[1] = {};
			HunterView hv = HvNew(trail, messages);

			assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
			assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == NOWHERE);
			assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == NOWHERE);
			assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == NOWHERE);
			assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == NOWHERE);
			assert(HvGetVampireLocation(hv) == NOWHERE);
			HvFree(hv);			

		}

		{

			// testing a hunter dying
			
			char *trail =
				"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
				"GGE.... SGE.... HGE.... MGE.... DSTT... "
				"GGE.... SGE.... HGE.... MGE.... DHIT... "
				"GGE.... SGE.... HZU.... MCF.... DD1T... "
				"GGE.... SSTTTTD";
			
			Message messages[21] = {};
			HunterView hv = HvNew(trail, messages);
			
			assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == GENEVA);
			assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == HOSPITAL_PLACE);
			assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == ZURICH);
			assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == CLERMONT_FERRAND);
			assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == STRASBOURG);
			assert(HvGetVampireLocation(hv) == CITY_UNKNOWN);
			HvFree(hv);

		}

		{

			// Testing an unrevealed vampire location

			char *trail =
				"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
				"GGE.... SGE.... HGE.... MGE.... DC?T... "
				"GGE.... SGE.... HGE.... MGE.... DC?T... "
				"GGE.... SGE.... HGE.... MGE.... DC?T... "
				"GGE.... SGE.... HGE.... MGE.... DC?T... "
				"GGE.... SGE.... HGE.... MGE.... DC?T... ";
			
			Message messages[35] = {};
			HunterView hv = HvNew(trail, messages);
			
			assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == GENEVA);
			assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == GENEVA);
			assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == GENEVA);
			assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == GENEVA);
			assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
			assert(HvGetVampireLocation(hv) == CITY_UNKNOWN);
			HvFree(hv);

		}

		{

			// Testing a matured vampire

			char *trail =
				"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
				"GGE.... SGE.... HGE.... MGE.... DC?T... "
				"GGE.... SGE.... HGE.... MGE.... DC?T... "
				"GGE.... SGE.... HGE.... MGE.... DC?T... "
				"GGE.... SGE.... HGE.... MGE.... DC?T... "
				"GGE.... SGE.... HGE.... MGE.... DC?T... "
				"GGE.... SGE.... HGE.... MGE.... DC?T.V. "
				"GGE.... SGE.... HGE.... MGE.... DC?T... ";
			
			Message messages[35] = {};
			HunterView hv = HvNew(trail, messages);

			assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
			assert(HvGetVampireLocation(hv) == NOWHERE);
			HvFree(hv);

		}

		{

			// Test for hunter encountering trap, vampire and Dracula

			char *trail =
				"GST.... SAO.... HCD.... MAO.... DGE.V.. "
				"GST.... SAO.... HCD.... MAO.... DHIT... "
				"GGETVD. SAO.... HCD....";
			
			Message messages[] = {};
			HunterView hv = HvNew(trail, messages);

			assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == GENEVA);
			assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == GENEVA);
			assert(HvGetVampireLocation(hv) == NOWHERE);
			HvFree(hv);
			
		}

	}
	
	{
		printf("Additional tests for Making a Move Part");
		{
			char *trail =
			"GLS.... SLS.... HSW.... MMR.... DCD.V..";
		
			Message messages[5] = {};
			HunterView hv = HvNew(trail, messages);
			printf("\tWhere can I go (Lord Godalming, Round 1)\n");
			int pathLength = -1;
			PlaceId *path = HvWhereCanIGo(hv, &pathLength);
			assert(pathLength == 5);
			assert(path[0] == ATLANTIC_OCEAN);
			assert(path[1] == CADIZ);
			assert(path[2] == LISBON);
			assert(path[3] == MADRID);
			assert(path[4] == SANTANDER);

			printf("Test passed!\n");
			free(path);
		}

		{
			char *trail =
			"";
		
			Message messages[5] = {};
			HunterView hv = HvNew(trail, messages);
			printf("\tWhere can I go (Lord Godalming, Round 0)\n");
			int pathLength = -1;
			PlaceId *path = HvWhereCanIGo(hv, &pathLength);
			assert(pathLength == 0);
			

			printf("Test passed!\n");
			free(path);
		}

		{
			char *trail =
			"GLS.... SLS.... HSW.... MMR.... DCD.V..";
		
			Message messages[5] = {};
			HunterView hv = HvNew(trail, messages);
			printf("\tWhere can I go by type(Lord Godalming, Round 1)\n");
			int pathLength = -1;
			PlaceId *path = HvWhereCanIGoByType(hv,true, true, true, &pathLength);
			assert(pathLength == 5);
			assert(path[0] == ATLANTIC_OCEAN);
			assert(path[1] == CADIZ);
			assert(path[2] == LISBON);
			assert(path[3] == MADRID);
			assert(path[4] == SANTANDER);

			printf("Test passed!\n");
			free(path);
		}

		{
			char *trail =
			"GLS.... SLS.... HSW.... MMR.... DCD.V..";
		
			Message messages[5] = {};
			HunterView hv = HvNew(trail, messages);
			printf("\tWhere can I go by type only by rail(Lord Godalming, Round 1)\n");
			int pathLength = -1;
			PlaceId *path = HvWhereCanIGoByType(hv,false, true, false, &pathLength);
			assert(pathLength == 2);
			assert(path[0] == LISBON);
			assert(path[1] == MADRID);
			printf("Test passed!\n");
			free(path);
		}

		{
			char *trail =
			"GLS.... SLS.... HSW.... MMR.... DCD.V..";
		
			Message messages[5] = {};
			HunterView hv = HvNew(trail, messages);
			printf("\tWhere can I go by type with road and rail(Lord Godalming, Round 1)\n");
			int pathLength = -1;
			PlaceId *path = HvWhereCanIGoByType(hv,true, true, false, &pathLength);
			assert(pathLength == 4);
			assert(path[0] == CADIZ);
			assert(path[1] == LISBON);
			assert(path[2] == MADRID);
			assert(path[3] == SANTANDER);
			printf("Test passed!\n");
			free(path);
		}

		{
			char *trail =
			"GLS.... SLS.... HSW.... MMR.... DCD.V..";
		
			Message messages[5] = {};
			HunterView hv = HvNew(trail, messages);
			printf("\tWhere can I go by type with boat(Lord Godalming, Round 1)\n");
			int pathLength = -1;
			PlaceId *path = HvWhereCanIGoByType(hv,false, false, true, &pathLength);
			assert(pathLength == 2);
			assert(path[0] == ATLANTIC_OCEAN);
			assert(path[1] == LISBON);

			printf("Test passed!\n");
			free(path);
		}

		{
			char *trail =
			"GLS.... SLS.... HSW.... MMR.... DCD.V..";
		
			Message messages[5] = {};
			HunterView hv = HvNew(trail, messages);
			printf("\tWhere can I go by type with nothing(Lord Godalming, Round 1)\n");
			int pathLength = -1;
			PlaceId *path = HvWhereCanIGoByType(hv,false, false, false, &pathLength);
			assert(pathLength == 1);
			assert(path[0] == LISBON);

			printf("Test passed!\n");
			free(path);
		}
	}
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
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
		free(locs);
		HvFree(hv);
	}
	return EXIT_SUCCESS;
	
}
