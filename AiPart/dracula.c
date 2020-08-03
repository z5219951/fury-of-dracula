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

#include "dracula.h"
#include "DraculaView.h"
#include "Game.h"


// extra functions
PlaceId *GetShortestPathTo(DraculaView dv, PlaceId src, PlaceId dest,
                           int *pathLength);
static Round playerNextRound(HunterView hv, Player player);
PlaceId *playerBfs(Dracula dv, Player player, PlaceId src, Round r);

void decideDraculaMove(DraculaView dv)
{
	// TODO: Replace this with something better!
	Round round = DvGetRound(dv);
	PlaceId * locG = DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING);
	PlaceId * locS = DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING);
	PlaceId * locH = DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING);
	PlaceId * locM = DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING);
	// Round 0
	// loop through all valid locations
	// excluding sea locations and hopsital
	if (round == 0) {
		for (PlaceId i = MIN_REAL_PLACE; loc <= MAX_REAL_PLACE) {
			// skip if sea locations and hospital
			if (loc == HOSPITAL_PLACE || placeIdToType(loc) == SEA) {
				continue;
			}
			int furthestDist = -1;
			int distFromMU = -1;
			int pathLength = -1;
			PlaceId *locG = GetShortestPathTo(dv, locG, i, &pathLength);
			if (pathLength => furthestDist) {
				
			}
		} 

	}


	registerBestPlay("CD", "Mwahahahaha");
}

PlaceId *GetShortestPathTo(DraculaView dv, PlaceId src, PlaceId dest,
                           int *pathLength)
{
	
	Round r = playerNextRound(dv, player);
	PlaceId *pred = playerBfs(dv, player, src, r);
	
	// One pass to get the path length
	int dist = 0;
	PlaceId curr = dest;
	while (curr != src) {
		dist++;
		curr = pred[curr];
	}
	
	PlaceId *path = malloc(dist * sizeof(PlaceId));
	// Another pass to copy the path in
	int i = dist - 1;
	curr = dest;
	while (curr != src) {
		path[i] = curr;
		curr = pred[curr];
		i--;
	}
	
	free(pred);
	*pathLength = dist;
	return path;
}

/**
 * Gets the round number of the player's next move
 */
static Round playerNextRound(DraculaView dv, Player player) {
	return DvGetRound(hv) + (player < DvGetPlayer(hv) ? 1 : 0);
}

/**
 * Performs a BFS for the given player starting at `src`, assuming the
 * round is `r`. Returns a predecessor array.
 */
PlaceId *playerBfs(Dracula dv, Player player, PlaceId src, Round r) {
	PlaceId *pred = malloc(NUM_REAL_PLACES * sizeof(PlaceId));
	placesFill(pred, NUM_REAL_PLACES, -1);
	pred[src] = src;
	
	Queue q1 = QueueNew(); // current round locations
	Queue q2 = QueueNew(); // next round locations
	
	QueueEnqueue(q1, src);
	while (!(QueueIsEmpty(q1) && QueueIsEmpty(q2))) {
		PlaceId curr = QueueDequeue(q1);
		int numReachable = 0;
		PlaceId *reachable = GvGetReachable(dv->gv, player, r, curr,
		                                    &numReachable);
		
		for (int i = 0; i < numReachable; i++) {
			if (pred[reachable[i]] == -1) {
				pred[reachable[i]] = curr;
				QueueEnqueue(q2, reachable[i]);
			}
		}
		free(reachable);
		
		// When we've exhausted the current round's locations, advance
		// to the next round and swap the queues (so the next round's
		// locations becomes the current round's locations)
		if (QueueIsEmpty(q1)) {
			Queue tmp = q1; q1 = q2; q2 = tmp; // swap queues
			r++;
		}
	}
	
	QueueDrop(q1);
	QueueDrop(q2);
	return pred;
}