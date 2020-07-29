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
	
	printf("Testing function GvGetReachable:\n");
	printf("\tROUND 1\n");
	char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
	{
		printf("\nChecking Galatz connections\n");
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 1, GALATZ, &numLocs);
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}

	{
		printf("\nChecking Santander connections\n");
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 1, SANTANDER, &numLocs);
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}

	{
		printf("\nChecking Marseilles connections\n");
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 1, MARSEILLES, &numLocs);
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}
	
	{
		printf("\nChecking Venice connections\n");
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 1, VENICE, &numLocs);
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}

    printf("\nChecking location move of dracula\n");
	printf("\tROUND 2 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 2, ROME, &numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 2\n");
		printf("\nThe current location is Rome\n");
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
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 3, FLORENCE, &numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 3\n");
		printf("\nThe current location is Florence\n");
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
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 4, VENICE, &numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 4\n");
		printf("\nThe current location is Venice\n");
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
		PlaceId *locs = GvGetReachable(gv, PLAYER_DRACULA, 5, MUNICH, &numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Dracula in Round 5\n");
		printf("\nThe current location is Munich\n");
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
	
	
	// test function GetLenOfList
	printf("\tTesting function GetLenOfList\n");

	{
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs1 = GvGetReachable(gv, PLAYER_DRACULA, 2, ROME,&numLocs);
		
		int n = GetLenOfList(locs1);
		printf("\nThe length of this list is %d\n", n);
	}

	// test function MergeList
	printf("\tTesting function MergeList\n");
	{

		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs1 = GvGetReachable(gv, PLAYER_DRACULA, 2, ROME,&numLocs);

		trail = "GMN.... SPL.... HAM.... MPA.... DRO.V.. "
				"GMN.... SPL.... HAM.... MPA.... DFL.V..";
		gv = GvNew(trail, messages);
		numLocs = -1;
		PlaceId *locs2 = GvGetReachable(gv, PLAYER_DRACULA, 3, FLORENCE,&numLocs);
		PlaceId *newList = MergeList(locs1, locs2);
		
		int n = GetLenOfList(newList);
		printf("The length of new list is %d\n", n);
		printf("The list after merge is\n");
		for (int i = 0; i < n; i++) {
			printf("%s(%d) ", placeIdToName(newList[i]), i + 1);
		}
	}

	//test function GetConnRail
	 printf("\tTesting function GetConnRail\n");
	{
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int m = MapNumPlaces(gv->map);
		int *repeated_city = calloc(m, sizeof(int));
		PlaceId *locs = GetConnRail(gv->map, ROME, 1, repeated_city);
		int n = GetLenOfList(locs);
		printf("\nThe following list are cities connected with %s through rail\n", "ROME");
		printf("When maximun distance is 1\n");
		for (int i = 0; i < n; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		repeated_city = calloc(m, sizeof(int));
		locs = GetConnRail(gv->map, ROME, 2, repeated_city);
		n = GetLenOfList(locs);
		printf("\nWhen maximum distance is 2\n");
		for (int i = 0; i < n; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		repeated_city = calloc(m, sizeof(int));
		locs = GetConnRail(gv->map, ROME, 3, repeated_city);
		n = GetLenOfList(locs);		
		printf("\nWhen maximum distance is 2\n");
		for (int i = 0; i < n; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
	} 

	// Test move of huner
	printf("Checking move of hunters(Goadlming) by using function GvGetReachable\n");
	// ROUND2
	printf("\n\tROUND 2 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_LORD_GODALMING, 2, MANCHESTER, &numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Goadlming in Round 2\n");
		printf("\nThe current location is Manchester\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}
	// ROUND3
	printf("\n\tROUND 3 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GED.... SPL.... HAM.... MPA.... DRO.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_LORD_GODALMING, 3, EDINBURGH, &numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Goadlming in Round 3\n");
		printf("\nThe current location is Edinburgh\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}
	// ROUND4
	printf("\n\tROUND 4 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GED.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GNS.... SPL.... HAM.... MPA.... DRO.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_LORD_GODALMING, 4, NORTH_SEA, &numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Goadlming in Round 4\n");
		printf("\nThe current location is NorthSea\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}
	// ROUND5
	printf("\n\tROUND 5 \n");
	{	
		char *trail = "GMN.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GED.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GNS.... SPL.... HAM.... MPA.... DRO.V.. "
					  "GAM.... SPL.... HAM.... MPA.... DRO.V..";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachable(gv, PLAYER_LORD_GODALMING, 5, AMSTERDAM, &numLocs);
		printf("\nThe number of path in GameView is %d\n", gv->num);
		printf("\nChecking accessible cities of Goadlming in Round 5\n");
		printf("\nThe current location is Amsterdam\n");
		printf("\nThe number of accessible cities is %d\n", numLocs);
		printf("\nAccessiable cities: ");
		for (int i = 0; i < numLocs; i++) {
			printf("%s(%d) ", placeIdToName(locs[i]), i + 1);
		}
		printf("\n");
	}

	return 0;

}
			