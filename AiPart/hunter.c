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
	if (currRound == 1 || vampireNow == CITY_UNKNOWN) {
		const char *result = placeIdToAbbrev(currPlace);
		char resultTran[3];
		strcpy(resultTran, result);
		registerBestPlay(resultTran, "searching");
		return;
	}

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

	const char *resultVamp;
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
		char resultVamTran[3];
		strcpy(resultVamTran, resultVamp);
		registerBestPlay(resultVamTran, "hunting vampire");
		return;
	}

	// check Dracula loaction
	// move to Dracula if it is revealed
	int pathDraNum = -1;
	if(lastKnow != -1) {
		PlaceId *pathDra = HvGetShortestPathTo(hv,currPlayer, lastPlace, &pathDraNum);
		const char *resultDra = placeIdToAbbrev(pathDra[0]);
		char resultDraTran[3];
		strcpy(resultDraTran, resultDra);
		registerBestPlay(resultDraTran, "hunting Dracula");
		return;
	}

	//stay if  healthPoint < 3
	if (healthPoint < 3) {
		const char *resultDanger = placeIdToAbbrev(currPlace);
		char resultDanTran[3];
		strcpy(resultDanTran, resultDanger);
		registerBestPlay(resultDanTran, "resting");
		return;
	}

	// default place 
	int defaultNum = -1;
	PlaceId *defaultPlace = HvWhereCanIGo(hv, &defaultNum);
	const char *resultdefault = placeIdToAbbrev(defaultPlace[0]);
	char resultDefTran[3];
	strcpy(resultDefTran, resultdefault);
	registerBestPlay(resultDefTran, "default move");
	return;

}
