////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testGameView.c: test the GameView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v1.2	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "GameView.h"
#include "Places.h"
#include "testUtils.h"
#include "Queue.h"

int main(void)
{
	{///////////////////////////////////////////////////////////////////
	
		printf("Basic initialisation\n");

		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(GvGetHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == NOWHERE);
		assert(GvGetVampireLocation(gv) == NOWHERE);

		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("After Lord Godalming's turn\n");

		char *trail =
			"GST....";
		
		Message messages[1] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_DR_SEWARD);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == NOWHERE);

		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("After Mina Harker's turn\n");

		char *trail =
			"GST.... SAO.... HZU.... MBB....";
		
		Message messages[4] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_DRACULA);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(GvGetPlayerLocation(gv, PLAYER_VAN_HELSING) == ZURICH);
		assert(GvGetPlayerLocation(gv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == NOWHERE);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("After Dracula's turn\n");

		char *trail =
			"GST.... SAO.... HZU.... MBB.... DC?.V..";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahahaha"
		};
		
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 1);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(GvGetHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(GvGetPlayerLocation(gv, PLAYER_VAN_HELSING) == ZURICH);
		assert(GvGetPlayerLocation(gv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == CITY_UNKNOWN);

		GvFree(gv);
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
		
		GameView gv = GvNew(trail, messages);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) ==
				GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == GENEVA);
		assert(GvGetVampireLocation(gv) == NOWHERE);

		GvFree(gv);
		printf("Test passed\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Encountering Dracula twice\n");

		char *trail =
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD.. SGE..D. HCD.... MAO....";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahahaha",
			"Aha!"
		};
		
		GameView gv = GvNew(trail, messages);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) ==
				GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - 2*LIFE_LOSS_HUNTER_ENCOUNTER);

		GvFree(gv);
		printf("Test passed\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula at sea twice, "
		       "and losing blood points \n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DS?....";
		
		Message messages[] = {
			"Party at Geneva", "Okay", "Sure", "Let's go", "Mwahahahaha",
			"", "", "", "", "Back I go"
		};
		
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 2);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));

		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula doubling back at sea, "
		       "and losing blood points (Hunter View)\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DD1....";
		
		Message messages[] = {
			"Party at Geneva", "Okay", "Sure", "Let's go", "Mwahahahaha",
			"", "", "", "", "Back I go"
		};
		
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 2);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == SEA_UNKNOWN);

		GvFree(gv);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula doubling back at sea, "
			   "and losing blood points (Dracula View)\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DEC.... "
			"GST.... SST.... HST.... MST.... DD1.... "
			"GFR.... SFR.... HFR.... MFR....";
		
		Message messages[14] = {
			"Hello", "Rubbish", "Stuff", "", "Mwahahah",
			"Aha!", "", "", "", "Back I go"};
		
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 2);
		assert(GvGetPlayer(gv) == PLAYER_DRACULA);
		assert(GvGetScore(gv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == ENGLISH_CHANNEL);

		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Checking that hunters' health points are capped\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE....";
	
		Message messages[6] = {};
		GameView gv = GvNew(trail, messages);
	
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		
		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Checking that hunters encounter trap tiwce and rest\n");
		
		char *trail =
			"GGETT.. SGE.... HGE.... MGE.... DC?.V.. "
			"GGE....";
	
		Message messages[6] = {};
		GameView gv = GvNew(trail, messages);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - 1);
		
		GvFree(gv);
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
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetScore(gv) == GAME_START_SCORE
		                         - 4 * SCORE_LOSS_DRACULA_TURN
		                         - SCORE_LOSS_HUNTER_HOSPITAL);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == 0);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == HOSPITAL_PLACE);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == STRASBOURG);
		
		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing Dracula doubling back to Castle Dracula\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T...";
		
		Message messages[10] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS + (2 * LIFE_GAIN_CASTLE_DRACULA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CASTLE_DRACULA);
		
		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing vampire/trap locations\n");
		
		char *trail =
			"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
			"GBD.... SGE.... HGE.... MGE.... DKLT... "
			"GSZ.... SGE.... HGE.... MGE.... DGAT... "
			"GSZ.... SGE.... HGE.... MGE....";
		
		Message messages[19] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == GALATZ);
		assert(GvGetVampireLocation(gv) == CASTLE_DRACULA);
		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 2);
		sortPlaces(traps, numTraps);
		assert(traps[0] == GALATZ && traps[1] == KLAUSENBURG);
		free(traps);
		
		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing trap locations after one is destroyed\n");
		
		char *trail =
			"GVI.... SGE.... HGE.... MGE.... DBC.V.. "
			"GBD.... SGE.... HGE.... MGE.... DKLT... "
			"GSZ.... SGE.... HGE.... MGE.... DGAT... "
			"GBE.... SGE.... HGE.... MGE.... DCNT... "
			"GKLT... SGE.... HGE.... MGE....";
		
		Message messages[24] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) ==
				GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == KLAUSENBURG);
		assert(GvGetVampireLocation(gv) == BUCHAREST);
		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 2);
		sortPlaces(traps, numTraps);
		assert(traps[0] == CONSTANTA && traps[1] == GALATZ);
		free(traps);
		
		GvFree(gv);
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
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetScore(gv) == GAME_START_SCORE
		                         - 7 * SCORE_LOSS_DRACULA_TURN
		                         - SCORE_LOSS_VAMPIRE_MATURES);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == NOWHERE);
		
		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing move/location history\n");
		
		char *trail =
			"GLS.... SGE.... HGE.... MGE.... DST.V.. "
			"GCA.... SGE.... HGE.... MGE.... DC?T... "
			"GGR.... SGE.... HGE.... MGE.... DC?T... "
			"GAL.... SGE.... HGE.... MGE.... DD3T... "
			"GSR.... SGE.... HGE.... MGE.... DHIT... "
			"GSN.... SGE.... HGE.... MGE.... DC?T... "
			"GMA.... SSTTTV.";
		
		Message messages[32] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_DR_SEWARD) ==
				GAME_START_HUNTER_LIFE_POINTS - 2 * LIFE_LOSS_TRAP_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == NOWHERE);
		
		// Lord Godalming's move/location history
		{
			int numMoves = 0; bool canFree = false;
			PlaceId *moves = GvGetMoveHistory(gv, PLAYER_LORD_GODALMING,
			                                  &numMoves, &canFree);
			assert(numMoves == 7);
			assert(moves[0] == LISBON);
			assert(moves[1] == CADIZ);
			assert(moves[2] == GRANADA);
			assert(moves[3] == ALICANTE);
			assert(moves[4] == SARAGOSSA);
			assert(moves[5] == SANTANDER);
			assert(moves[6] == MADRID);
			if (canFree) free(moves);
		}
		
		// Dracula's move/location history
		{
			int numMoves = 0; bool canFree = false;
			PlaceId *moves = GvGetMoveHistory(gv, PLAYER_DRACULA,
			                                  &numMoves, &canFree);
			assert(numMoves == 6);
			assert(moves[0] == STRASBOURG);
			assert(moves[1] == CITY_UNKNOWN);
			assert(moves[2] == CITY_UNKNOWN);
			assert(moves[3] == DOUBLE_BACK_3);
			assert(moves[4] == HIDE);
			assert(moves[5] == CITY_UNKNOWN);
			if (canFree) free(moves);
		}
		
		{
			int numLocs = 0; bool canFree = false;
			PlaceId *locs = GvGetLocationHistory(gv, PLAYER_DRACULA,
			                                     &numLocs, &canFree);
			assert(numLocs == 6);
			assert(locs[0] == STRASBOURG);
			assert(locs[1] == CITY_UNKNOWN);
			assert(locs[2] == CITY_UNKNOWN);
			assert(locs[3] == STRASBOURG);
			assert(locs[4] == STRASBOURG);
			assert(locs[5] == CITY_UNKNOWN);
			if (canFree) free(locs);
		}
		
		GvFree(gv);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing connections\n");
		
		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		{
			printf("\tChecking Galatz road connections "
			       "(Lord Godalming, Round 1)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     1, GALATZ, true, false,
			                                     false, &numLocs);

			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BUCHAREST);
			assert(locs[1] == CASTLE_DRACULA);
			assert(locs[2] == CONSTANTA);
			assert(locs[3] == GALATZ);
			assert(locs[4] == KLAUSENBURG);
			free(locs);
		}

		{
			printf("\tChecking Ionian Sea boat connections "
			       "(Lord Godalming, Round 1)\n");
			
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     1, IONIAN_SEA, false, false,
			                                     true, &numLocs);
			
			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == ADRIATIC_SEA);
			assert(locs[1] == ATHENS);
			assert(locs[2] == BLACK_SEA);
			assert(locs[3] == IONIAN_SEA);
			assert(locs[4] == SALONICA);
			assert(locs[5] == TYRRHENIAN_SEA);
			assert(locs[6] == VALONA);
			free(locs);
		}

		{
			printf("\tChecking Paris rail connections "
			       "(Lord Godalming, Round 2)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     2, PARIS, false, true,
			                                     false, &numLocs);
			
			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BORDEAUX);
			assert(locs[1] == BRUSSELS);
			assert(locs[2] == COLOGNE);
			assert(locs[3] == LE_HAVRE);
			assert(locs[4] == MARSEILLES);
			assert(locs[5] == PARIS);
			assert(locs[6] == SARAGOSSA);
			free(locs);
		}
		
		{
			printf("\tChecking Athens rail connections (none)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     1, ATHENS, false, true,
			                                     false, &numLocs);
			
			assert(numLocs == 1);
			assert(locs[0] == ATHENS);
			free(locs);
		}
		
		GvFree(gv);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("Additional test for Game History Part");
		
		{
			// for empty trail
			char *trail ="";
			
			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);
			
			//test GvGetMoveHistory
			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetMoveHistory(gv, PLAYER_LORD_GODALMING,&numMoves, &canFree); 
				assert(moves == NULL);
				assert(numMoves == 0);
				if (canFree) free(moves);
			}

			//test GvGetLastMoves
			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLastMoves(gv, PLAYER_LORD_GODALMING,10,&numMoves, &canFree); 
				assert(moves == NULL);
				assert(numMoves == 0);
				if (canFree) free(moves);
			}

			// test GvGetLocationHistory
			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLocationHistory(gv, PLAYER_LORD_GODALMING,&numMoves, &canFree); 
				assert(moves == NULL);
				assert(numMoves == 0);
				if (canFree) free(moves);
			}

			GvFree(gv);
			printf("Test passed!\n");
		}

		{
			//test GvGetLastMoves
			char *trail =
				"GLS.... SGE.... HGE.... MGE.... DST.V.. "
				"GCA.... SGE.... HGE.... MGE.... DC?T... "
				"GGR.... SGE.... HGE.... MGE.... DC?T... "
				"GAL.... SGE.... HGE.... MGE.... DD3T... "
				"GSR.... SGE.... HGE.... MGE.... DHIT... "
				"GSN.... SGE.... HGE.... MGE.... DC?T... "
				"GMA.... SSTTTV.";
			
			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);


			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLastMoves(gv, PLAYER_LORD_GODALMING,1,&numMoves, &canFree); 
				assert(numMoves == 1);
				assert(moves[0] == MADRID);
				if (canFree) free(moves);
			}

			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLastMoves(gv, PLAYER_LORD_GODALMING,0,&numMoves, &canFree); 
				assert(numMoves == 0);
				assert(moves == NULL);
				if (canFree) free(moves);
			}
			
			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLastMoves(gv, PLAYER_LORD_GODALMING,10,&numMoves, &canFree); 
				assert(numMoves == 7);
				assert(moves[0] == LISBON);
				assert(moves[1] == CADIZ);
				assert(moves[2] == GRANADA);
				assert(moves[3] == ALICANTE);
				assert(moves[4] == SARAGOSSA);
				assert(moves[5] == SANTANDER);
				assert(moves[6] == MADRID);
			}

			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLastMoves(gv, PLAYER_LORD_GODALMING,7,&numMoves, &canFree); 
				assert(numMoves == 7);
				assert(moves[0] == LISBON);
				assert(moves[1] == CADIZ);
				assert(moves[2] == GRANADA);
				assert(moves[3] == ALICANTE);
				assert(moves[4] == SARAGOSSA);
				assert(moves[5] == SANTANDER);
				assert(moves[6] == MADRID);
			}

			GvFree(gv);
			printf("Test passed!\n");
		}

		{
			// test GvGetLocationHistory
			char *trail =
				"GLS.... SGE.... HGE.... MGE.... DST.V.. "
				"GCA.... SGE.... HGE.... MGE.... DC?T... "
				"GGR.... SGE.... HGE.... MGE.... DC?T... "
				"GAL.... SGE.... HGE.... MGE.... DD2T... "
				"GSR.... SGE.... HGE.... MGE.... DTPT... "
				"GSN.... SGE.... HGE.... MGE.... DHIT... "
				"GMA.... SSTTTV.";
			
			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);
			int numLocs = 0; bool canFree = false;
			PlaceId *locs = GvGetLocationHistory(gv, PLAYER_DRACULA,
			                                     &numLocs, &canFree);
			assert(numLocs == 6);
			assert(locs[0] == STRASBOURG);
			assert(locs[1] == CITY_UNKNOWN);
			assert(locs[2] == CITY_UNKNOWN);
			assert(locs[3] == CITY_UNKNOWN);
			assert(locs[4] == CASTLE_DRACULA);
			assert(locs[5] == CASTLE_DRACULA);
			if (canFree) free(locs);

			GvFree(gv);
			printf("Test passed!\n");
		}

		{
			//test GvGetLastLocations
			char *trail =
				"GLS.... SGE.... HGE.... MGE.... DST.V.. "
				"GCA.... SGE.... HGE.... MGE.... DC?T... "
				"GGR.... SGE.... HGE.... MGE.... DC?T... "
				"GAL.... SGE.... HGE.... MGE.... DD3T... "
				"GSR.... SGE.... HGE.... MGE.... DHIT... "
				"GSN.... SGE.... HGE.... MGE.... DC?T... "
				"GMA.... SSTTTV.";
			
			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);

			// test Hunter
			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLastLocations(gv, PLAYER_LORD_GODALMING,1,&numMoves, &canFree); 
				assert(numMoves == 1);
				assert(moves[0] == MADRID);
				if (canFree) free(moves);
			}

			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLastLocations(gv, PLAYER_LORD_GODALMING,0,&numMoves, &canFree); 
				assert(numMoves == 0);
				assert(moves == NULL);
				if (canFree) free(moves);
			}
			
			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLastLocations(gv, PLAYER_LORD_GODALMING,10,&numMoves, &canFree); 
				assert(numMoves == 7);
				assert(moves[0] == LISBON);
				assert(moves[1] == CADIZ);
				assert(moves[2] == GRANADA);
				assert(moves[3] == ALICANTE);
				assert(moves[4] == SARAGOSSA);
				assert(moves[5] == SANTANDER);
				assert(moves[6] == MADRID);
			}

			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLastLocations(gv, PLAYER_LORD_GODALMING,7,&numMoves, &canFree); 
				assert(numMoves == 7);
				assert(moves[0] == LISBON);
				assert(moves[1] == CADIZ);
				assert(moves[2] == GRANADA);
				assert(moves[3] == ALICANTE);
				assert(moves[4] == SARAGOSSA);
				assert(moves[5] == SANTANDER);
				assert(moves[6] == MADRID);
			}

			// test Dracula
			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLastLocations(gv, PLAYER_DRACULA,1,&numMoves, &canFree); 
				assert(numMoves == 1);
				assert(moves[0] == CITY_UNKNOWN);
				if (canFree) free(moves);
			}

			{
				int numMoves = 0; bool canFree = false;
				PlaceId *moves = GvGetLastLocations(gv, PLAYER_DRACULA,2,&numMoves, &canFree); 
				assert(numMoves == 2);
				assert(moves[0] == STRASBOURG);
				assert(moves[1] == CITY_UNKNOWN);
				if (canFree) free(moves);
			}

			GvFree(gv);
			printf("Test passed!\n");
		}
	}

	{///////////////////////////////////////////////////////////////////

		printf("Additional tests for player, vampire and trap locations.\n");
		
		{

			char *trail ="";

			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);

			assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == NOWHERE);
			assert(GvGetPlayerLocation(gv, PLAYER_VAN_HELSING) == NOWHERE);
			assert(GvGetPlayerLocation(gv, PLAYER_MINA_HARKER) == NOWHERE);
			assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == NOWHERE);
			int numTraps = 0;
			PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
			assert(numTraps == 0);
			free(traps);
			GvFree(gv);	
		}

		{
			
			char *trail =
				"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GSZ.... SGE.... HGE.... MGE....";
			
			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);
			
			int numTraps = 0;
			PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
			assert(numTraps == 2);
			sortPlaces(traps, numTraps);
			assert(traps[0] == SOFIA && traps[1] == VALONA);
			free(traps);
			GvFree(gv);
		
		}

		{
			
			char *trail = 
				"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GSZ.... SGE.... HGE.... MGE....";

			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);
			
			assert(GvGetVampireLocation(gv) == CASTLE_DRACULA);
			int numTraps = 0;
			PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
			assert(numTraps == 5);
			sortPlaces(traps, numTraps);
			assert(traps[0] == CASTLE_DRACULA &&
				   traps[1] == SOFIA &&
				   traps[2] == SOFIA && 
				   traps[3] == VALONA && 
				   traps[4] == VALONA);
			free(traps); 
			GvFree(gv);

		}	

		{

			char *trail =
				"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
				"GGE.... SGE.... HGE.... MGE.... DSTT... "
				"GGE.... SGE.... HGE.... MGE.... DHIT... "
				"GGE.... SGE.... HGE.... MGE.... DD1T... "
				"GSTTTTD SGE.... HGE.... MGE....";

			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);	
				
			int numTraps = 0;
			PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
			assert(numTraps == 0);
			free(traps); 
			GvFree(gv);
			
		}

		{

			// immature vampire placed on a HIDE move
			// (locations chosen are merely for testing purposes, 
			//  disregards dracula's movement restrictions)

			char *trail = 
				"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DHI.V.. "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GSZ.... SGE.... HGE.... MGE....";

			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);
			
			assert(GvGetVampireLocation(gv) == VALONA);
		}

		{

			// immature vampire placed on a DOUBLE_BACK move

			char *trail = 
				"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DSO.V.. "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GSZ.... SGE.... HGE.... MGE....";

			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);
			
			assert(GvGetVampireLocation(gv) == SOFIA);

		}

		{

			// immature vampire and a trap vanquished in same location

			char *trail = 
				"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DVAT... "
				"GVI.... SGE.... HGE.... MGE.... DCDT... "
				"GBD.... SGE.... HGE.... MGE.... DSOT... "
				"GSZ.... SGE.... HGE.... MGE.... DZAT... "
				"GVI.... SGE.... HGE.... MGE.... DHI.V.. "
				"GBD.... SGE.... HGE.... MGE.... DFLT... "
				"GSZ.... SGE.... HZATV.. MGE.... DPRT... "
				"GSZ.... SGE.... HGE.... MGE....";

			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);
			
			assert(GvGetVampireLocation(gv) == NOWHERE);
			int numTraps = 0;
			PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
			assert(numTraps == 4);
			sortPlaces(traps, numTraps);
			assert(traps[0] == CASTLE_DRACULA);
			assert(traps[1] == FLORENCE);
			assert(traps[2] == PRAGUE);
			assert(traps[3] == SOFIA);
			free(traps); 
			GvFree(gv);
		}
		
		{

			// immature vampire and two traps vanquished in same location

			char *trail =
				"GLS.... SGE.... HGE.... MGE.... DST.V.. "
				"GCA.... SGE.... HGE.... MGE.... DSTT... "
				"GGR.... SGE.... HGE.... MGE.... DFRT... "
				"GAL.... SGE.... HGE.... MGE.... DD2T... "
				"GSR.... SGE.... HGE.... MGE.... DHI.... "
				"GSN.... SGE.... HGE.... MGE.... DFRT... "
				"GMA.... SSTTTV. HGE.... MGE....";
			
			Message messages[32] = {};
			GameView gv = GvNew(trail, messages);

			assert(GvGetVampireLocation(gv) == NOWHERE);
			int numTraps = 0;
			PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
			assert(numTraps == 2);
			sortPlaces(traps, numTraps);
			assert(traps[0] == FRANKFURT && traps[1] == FRANKFURT);
			free(traps); 
			GvFree(gv);

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
			GameView gv = GvNew(trail, messages);

			assert(GvGetVampireLocation(gv) == NOWHERE);
			int numTraps = -1;
			PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
			assert(numTraps == 1);
			assert(traps[0] == EDINBURGH);
			free(traps);
			GvFree(gv);			

		}
		printf("Test passed!\n");
	
	}

	return EXIT_SUCCESS;
	
}
