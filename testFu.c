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
#include "Map.h"
struct gameView {
	// TODO: ADD FIELDS HERE
	char **Path;
	int num;
	Map map;
};

int main (void) 
{
	printf("\tTESTING GvGetReachable:\n");
    printf("\nChecking move of dracula by using function GvGetReachable\n");
	printf("\tROUND 2 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 2, ROME,&numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 2\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}
	printf("\tROUND 3 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DFL.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 3, FLORENCE,&numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 3\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}
	printf("\tROUND 4 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DFL.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DVE.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 4, VENICE,&numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 4\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}
	printf("\tROUND 5 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DFL.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DVE.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DMU.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 5, MUNICH,&numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 5\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}
	printf("\tROUND 6 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DFL.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DVE.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DMU.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DZA.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 6, ZAGREB,&numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 6\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}

	printf("\tROUND 7 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DFL.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DVE.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DMU.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DZA.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DVI.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 7, VIENNA,&numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 7\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}
	// transport to castel of dracula
	printf("\tROUND 8 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DFL.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DVE.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DMU.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DZA.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DVI.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DTP.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 8, CASTLE_DRACULA, &numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 8\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}
	// test double back
	printf("\tROUND 9 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DFL.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DVE.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DMU.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DZA.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DVI.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DTP.V.. "
					  "GMN.... SPL.... HAM.... MPA.... DD1.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 9, CASTLE_DRACULA, &numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 9\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}
	return 0;

}