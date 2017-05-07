#ifndef _GA_H_
#define _GA_H_

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <limits.h>
#include <float.h>

#include <ncurses.h>

#include <string.h>

#include "creature.h"
#include "nn.h"



char intChar(int a);
void saveBestCreatureBrain(nn brain, char* path);
nn loadCreatureBrain(char* path);
void updateTab(int** tab, int hs, creature* creatures, int nbCreatures, position* obstacles, int nbObstacles, position* food, int nbFood);
void display(int** tab, int hsize, creature* creatures, int nbCreatures);
position* initFood(int nbFood, int hs, creature* creatures, int nbCreatures, position* obstacles, int nbObstacles);
position* initObstacles(int nbObstacles, int hs, creature* creatures, int nbCreatures);
int** initTab(int hs);
void usage();


#endif
