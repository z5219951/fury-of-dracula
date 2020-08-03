////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// hunter.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "hunter.h"
#include "HunterView.h"
#include <string.h>
#include <stdio.h>

// check valid move, if dest valid return dest, else return another place
static PlaceId checkMove(HunterView hv, PlaceId dest);

void decideHunterMove(HunterView hv)
{
	// set basic information
	Round currRound = HvGetRound(hv);
	Player currPlayer = HvGetPlayer(hv);
	int healthPoint = HvGetHealth(hv, currPlayer);
	PlaceId currPlace = HvGetPlayerLocation(hv, currPlayer);
	PlaceId vampireNow = HvGetVampireLocation(hv);

	// last know palce for Dracula
	Round lastKnow = -1;
	PlaceId lastPlace = HvGetLastKnownDraculaLocation(hv, &lastKnow); 
	//set four start corners
	// we choose PLAYER_LORD_GODALMING to stay in CD
	char **startPlace  = malloc(sizeof(char *)*4);
	startPlace[0] = "CD";
	startPlace[1] = "NP";
	startPlace[2] = "BR";
	startPlace[3] = "MA";


	//for round 0
	if (currRound == 0) {
		switch (currPlayer)
		{
		case PLAYER_LORD_GODALMING:
			registerBestPlay(startPlace[0], "hhh");
			return;
			break;
		case PLAYER_DR_SEWARD:
			registerBestPlay(startPlace[1], "hhh");
			return;
			break;
		case PLAYER_VAN_HELSING:
			registerBestPlay(startPlace[2], "hhh");
			return;
			break;
		default:
			registerBestPlay(startPlace[3], "hhh");
			return;
			break;
		}
	}

	// stay in the same place to do research for round 1 or vampireNow is unknow
	if (currRound == 1 || (vampireNow == CITY_UNKNOWN && currRound%13 == 6)) {
		char *result = placeIdToAbbrev(currPlace);
		registerBestPlay(result, "searching vampire");
		return;
	}
	
	// PLAYER_LORD_GODALMING should always stay in DC
	if (currPlayer == PLAYER_LORD_GODALMING) {
		// make sure he is in DC
		char *backDc;
		if (currPlace != CASTLE_DRACULA) {
			int numBackDc = -1;
			PlaceId *pathDc = HvGetShortestPathTo(hv,PLAYER_LORD_GODALMING, CASTLE_DRACULA, &numBackDc);
			backDc = placeIdToAbbrev(pathDc[0]);
			registerBestPlay(backDc, "Go back castle");
			return;
		}
		backDc = placeIdToAbbrev(CASTLE_DRACULA);
		registerBestPlay(backDc, "Stay alert");
		return;
	}

	// check whether vampireNow is in currPlace
	if (vampireNow == currPlace) {
		char *resultVamCurr = placeIdToAbbrev(currPlace);
		registerBestPlay(resultVamCurr, "find vampire");
		return;
	}
	if (vampireNow != CITY_UNKNOWN) {	
		// choose one to destory the vampire except PLAYER_LORD_GODALMING
		Player destroyVam = PLAYER_DR_SEWARD;
		int numPathSmall = -1;
		int numPathV = -1;
		int numPathM = -1;

		// get PLAYER_DR_SEWARD path length
		PlaceId *pathVamD = HvGetShortestPathTo(hv,PLAYER_DR_SEWARD, vampireNow, &numPathSmall);
		PlaceId *pathVamV = HvGetShortestPathTo(hv,PLAYER_VAN_HELSING, vampireNow, &numPathV);
		PlaceId *pathVamM = HvGetShortestPathTo(hv,PLAYER_MINA_HARKER, vampireNow, &numPathM);
		
		// choose the player
		if (numPathSmall > numPathV) {
			numPathSmall = numPathV;
			destroyVam = PLAYER_VAN_HELSING;
		}

		if (numPathSmall > numPathM) {
			numPathSmall = numPathM;
			destroyVam = PLAYER_MINA_HARKER;
		}

		char *resultVamp;
		switch (destroyVam)
		{
		case PLAYER_DR_SEWARD:
			resultVamp = placeIdToAbbrev(pathVamD[0]);
			break;
		case PLAYER_VAN_HELSING:
			resultVamp = placeIdToAbbrev(pathVamV[0]);
			break;
		case PLAYER_MINA_HARKER:
			resultVamp = placeIdToAbbrev(pathVamM[0]);
			break;
		
		default:
			break;
		}

		// moving if player is the one 

		if (currPlayer == destroyVam) {
			registerBestPlay(resultVamp, "hunting vampire");
			return;
		}
	}

	// check Dracula loaction
	// move to Dracula if it is revealed
	// the round should be no more than 7 round before
	int pathDraNum = -1;
	if(lastKnow != -1 && (currRound-lastKnow) < 7) {
		PlaceId *pathDra = HvGetShortestPathTo(hv,currPlayer, lastPlace, &pathDraNum);
		PlaceId startone = checkMove(hv, pathDra[0]);
		char *resultDra = placeIdToAbbrev(startone);
		registerBestPlay(resultDra, "hunting Dracula");
		return;
	}

	//stay if  healthPoint < 3
	if (healthPoint < 3) {
		char *resultDanger = placeIdToAbbrev(currPlace);
		registerBestPlay(resultDanger, "resting");
		return;
	}

	// if dracula use double back or hide, it probably nears one of the hunter
	Round lastMoveRound = -1;
	PlaceId lastMove = HvGetLastKnownDraculaMove(hv, &lastMoveRound);
	if (((lastMove >= DOUBLE_BACK_1) && (lastMove <= DOUBLE_BACK_5)) || lastMove == HIDE) {
		char *resultMove = placeIdToAbbrev(currPlace);
		registerBestPlay(resultMove, "searching Dracula");
		return;
	} 

	// default place 
	int defaultNum = -1;
	PlaceId *defaultPlace = HvWhereCanIGo(hv, &defaultNum);
	PlaceId final;
	for (int i = 0; i < defaultNum; i++) {
		if (defaultPlace[i] != currPlace) {
			final = defaultPlace[i];
			break;
		}
	}
	char *resultdefault = placeIdToAbbrev(final);
	registerBestPlay(resultdefault, "default move");
	return;

}


// check valid move, if dest valid return dest, else return another place
static PlaceId checkMove(HunterView hv, PlaceId dest) {
	int defaultNum = -1;
	PlaceId *defaultPlace = HvWhereCanIGo(hv, &defaultNum);
	Player currPlayer = HvGetPlayer(hv);
	PlaceId currPlace = HvGetPlayerLocation(hv, currPlayer);
	PlaceId final;
	for (int i = 0; i < defaultNum; i++) {
		// place reachable
		if (defaultPlace[i] == dest) {
			return dest;
		}
		if (defaultPlace[i] != currPlace) {
			final = defaultPlace[i];
		}
	}
	return final;
}