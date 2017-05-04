#ifndef _CREATURE_H_
#define _CREATURE_H_

#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>
#include <math.h>

#include "nn.h"

#define NB_GENES 4
#define THRESHOLD_STOPPED_MOVING 3
#define INITIAL_LIFE 100
#define VISION_SIZE 4

struct genCode{
  float* genes;
  int nbGenes;
};

typedef struct genCode genCode;


struct position{
  int x;
  int y;
};

typedef struct position position;


struct creature{
  int id;
  int life;
  int score;
  int iterationLastMoved;
  position originPos;
  position pos;
  genCode gen;
  nn brain;
};

typedef struct creature creature;

genCode initGenCode();
creature* initCreatures(int nbCreatures);
void updateCreatures(creature* creatures, int nbCreatures, int tabSize, int** tab, int iteration);
int getIndiceMax(float* tab, int sizeTab);
int getDirection(genCode gc);
bool canMove(position p, int direction, int tabSize, creature* creatures, int nbCreatures);
//confidenceObstacleAhead : confidence (with noise) that there is an obstacle ahead
void moveCreature(creature *c, int tabSize, int iteration, creature* creatures,int nbCreatures, float* vision);
int randomBetween(int min, int max);
bool oneCreatureIsMoving(creature* creatures, int nbCreatures, int iteration);
creature* createNewGeneration(creature* creatures, int nbCreatures);
creature mate(creature c1, creature c2, int id);
genCode mixGenCode(genCode g1, genCode g2);
genCode mutate(genCode g);
bool collisionOtherCreature(position p, int direction, creature* creatures, int nbCreatures);


#endif
