########################################################################
# COMP2521 20T2 // the Fury of Dracula // the View
# view/Makefile: build tests for GameView/HunterView/DraculaView
#
# You can modify this if you add additional files
#
# 2018-12-31	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
# 2020-07-10    v2.0    Team Dracula <cs2521@cse.unsw.edu.au>
#
########################################################################

CC = gcc
CFLAGS = -Wall -Werror -g
BINS = testGameView testHunterView testDraculaView testMap testFu testDvFu testHvFu

all: $(BINS)

testHvFu: testHvFu.o testUtils.o GameView.o HunterView.o Map.o Places.o Queue.o Map.o 
testHvFu.o: testHvFu.c GameView.h HunterView.h Map.h Places.h Game.h Queue.h Map.h 

testDvFu:testDvFu.o testUtils.o GameView.o DraculaView.o Map.o Places.o Queue.o Map.o
testDvFu.o: testDvFu.c GameView.h DraculaView.h Map.h Places.h Game.h Queue.h Map.h

testFu: testFu.o testUtils.o GameView.o Map.o Places.o Queue.o Map.o
testFu.o: testFu.c GameView.h Map.h Places.h Game.h Queue.h Map.h
testHunterView: testHunterView.o testUtils.o HunterView.o GameView.o Map.o Places.o 
testHunterView.o: testHunterView.c HunterView.h GameView.h Map.h Places.h Game.h 

testGameView: testGameView.o testUtils.o GameView.o Map.o Places.o Queue.o
testGameView.o: testGameView.c GameView.h Map.h Places.h Game.h Queue.h

testHunterView: testHunterView.o testUtils.o HunterView.o GameView.o Map.o Places.o Queue.o
testHunterView.o: testHunterView.c HunterView.h GameView.h Map.h Places.h Game.h Queue.h

testDraculaView: testDraculaView.o testUtils.o DraculaView.o GameView.o Map.o Places.o Queue.o
testDraculaView.o: testDraculaView.c DraculaView.h GameView.h Map.h Places.h Game.h Queue.h

testMap: testMap.o Map.o Places.o
testMap.o: testMap.c Map.h Places.h

Places.o: Places.c Places.h Game.h
Map.o: Map.c Map.h Places.h Game.h
GameView.o:	GameView.c GameView.h Game.h Queue.h Map.h
HunterView.o: HunterView.c HunterView.h Game.h
DraculaView.o: DraculaView.c DraculaView.h Game.h
testUtils.o: testUtils.c Places.h Game.h
Queue.o: Queue.c Queue.h Game.h

sallytest: sallytest.o testUtils.o GameView.o Map.o Places.o HunterView.o DraculaView.o

.PHONY: clean
clean:
	-rm -f ${BINS} *.o core
