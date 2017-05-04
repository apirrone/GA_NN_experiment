

#include "creature.h"

int randomBetween(int min, int max){
  return (rand() % (max + 1 - min)) + min;
}

void printGenCode(genCode gc){

  for(int i = 0 ; i < gc.nbGenes ; i++){
    printf("%d : %f\n", i, gc.genes[i]);
  }
  printf("\n");

}

genCode initGenCode(int nbGenes){
  
  genCode gc;

  gc.genes = malloc(nbGenes*sizeof(float));
  
  for(int i = 0 ; i < nbGenes ; i++){
    gc.genes[i] = randomBetween(0, 100)/100.;
    gc.nbGenes = nbGenes;
  }

  /* printGenCode(gc); */
  
  return gc;
}

creature* initCreatures(int nbCreatures){
  
  creature* creatures = malloc(nbCreatures*sizeof(creature));

  int nbNeuronsFirstLayer = 4;
  int nbNeuronsMiddleLayer = 5;
  int nbNeuronsLastLayer = 4;

  int nbGenes = nbNeuronsFirstLayer*nbNeuronsMiddleLayer + nbNeuronsMiddleLayer*nbNeuronsLastLayer;
  
  for(int i = 0 ; i < nbCreatures ; i++){
    creatures[i].id = i;
    creatures[i].life = INITIAL_LIFE;
    creatures[i].score = 0;
    creatures[i].iterationLastMoved = 0; 
    creatures[i].originPos.x = i+1;  
    creatures[i].originPos.y = 1;
    creatures[i].pos.x = i+1;  
    creatures[i].pos.y = 1;
    creatures[i].gen = initGenCode(nbGenes);
    creatures[i].brain = initNeuralNetwork(nbNeuronsFirstLayer, nbNeuronsMiddleLayer, nbNeuronsLastLayer, creatures[i].gen.genes, creatures[i].gen.nbGenes);
  }

  return creatures;
  
}

float* getVision(creature c, int tabSize){
  
  float* vision = malloc(VISION_SIZE*sizeof(float));
  position coords = c.pos;
  if( coords.y-1 <=0 )
    vision[0] = 1-(randomBetween(0, 30)/100); 
  else
    vision[0] = 0; 
  
  if( coords.x+1 >= tabSize )
    vision[1] = 1-(randomBetween(0, 30)/100); 
  else
    vision[1] = 0; 
  
  if( coords.y+1 >= tabSize )
    vision[2] = 1-(randomBetween(0, 30)/100); 
  else
    vision[2] = 0;
  
  if( coords.x-1 <=0 )
    vision[3] = 1-(randomBetween(0, 30)/100); 
  else
    vision[3] = 0;
  
  return vision;
  
}
  
void updateCreatures(creature* creatures, int nbCreatures, int tabSize, int** tab, int iteration){
  
  for(int i = 0 ; i < nbCreatures ; i++){
    float* vision = getVision(creatures[i], tabSize);
    /* position coords = creatures[i].pos; */
    /* if( coords.y-1 <=0 ) */
    /*   vision[0] = 1-(randomBetween(0, 30)/100);  */
    /* else */
    /*   vision[0] = 0;  */

    /* if( coords.x+1 >= tabSize ) */
    /*   vision[1] = 1-(randomBetween(0, 30)/100);  */
    /* else */
    /*   vision[1] = 0;  */

    /* if( coords.y+1 >= tabSize ) */
    /*   vision[2] = 1-(randomBetween(0, 30)/100);  */
    /* else */
    /*   vision[2] = 0; */

    /* if( coords.x-1 <=0 ) */
    /*   vision[3] = 1-(randomBetween(0, 30)/100);  */
    /* else */
    /*   vision[3] = 0;     */
    
    moveCreature(&creatures[i], tabSize, iteration, creatures, nbCreatures, vision);
  } 
}



/* int getIndiceMax(float* tab, int sizeTab){ */
  
/*   float max = FLT_MIN; */
/*   int indice = 0; */
/*   for(int i = 0 ; i < sizeTab ; i++){ */
/*     if(tab[i] > max){ */
/*       max = tab[i]; */
/*       indice = i; */
/*     } */
/*   } */
  
/*   return indice; */
/* } */

/* int getDirection(genCode gc){ */
/*   return getIndiceMax(gc.genes, gc.nbGenes); */
/* } */

bool collisionOtherCreature(position p, int direction, creature* creatures, int nbCreatures){

  position newPos;
  
  switch(direction){
  case 0 :
    newPos.y = p.y-1;
    break;
      
  case 1 :
    newPos.x = p.x+1;
    break;
      
  case 2 :
    newPos.y = p.y+1;
    break;
      
  case 3 :
    newPos.x = p.x-1;
    break;
  }

  for(int i = 0 ; i < nbCreatures ; i++){
    if(creatures[i].pos.x == newPos.x && creatures[i].pos.y == newPos.y){
      /* printf("COLLISION\n"); */
      return true;
    }
  }

  return false;
  
}

//0 : up
//1 : right
//2 : down
//3 : left
bool canMove(position p, int direction, int tabSize, creature* creatures, int nbCreatures){
  /* printf("direction : %d\n", direction); */
  if(direction == 0 || direction == 2)
    if(p.y > 0 && p.y < tabSize-1){
      /* if(!collisionOtherCreature(p, direction, creatures, nbCreatures)) */
      /* printf("CAN MOVE\n"); */
      return true;
    }
  
  
  if(direction == 1 || direction == 3)
    if(p.x > 0 && p.x < tabSize-1){
      /* if(!collisionOtherCreature(p, direction, creatures, nbCreatures)) */
      /* printf("CAN MOVE\n"); */
      return true;
    }

  /* printf("CANNOT MOVE\n"); */
  return false;  
}

//0 : up
//1 : right
//2 : down
//3 : left
void moveCreature(creature *c, int tabSize, int iteration, creature* creatures, int nbCreatures, float* vision){

  /* int direction = getDirection(c->gen); */
  
  int direction = getDirectionFromNeuralNetwork(c->brain, vision);
  
  if(canMove(c->pos, direction, tabSize, creatures, nbCreatures)){

    c->iterationLastMoved = iteration;
    
    switch(direction){
    case 0 :
      c->pos.y -= 1;
      break;
      
    case 1 :
      c->pos.x += 1;
      break;
      
    case 2 :
      c->pos.y +=1;
      /* c->score++; */
      break;
      
    case 3 :
      c->pos.x -=1;
      break;
    } 
  }

}

bool oneCreatureIsMoving(creature* creatures, int nbCreatures, int iteration){
  
  for(int i = 0 ; i < nbCreatures ; i++)
    if(iteration - creatures[i].iterationLastMoved < THRESHOLD_STOPPED_MOVING)
      return true;
  
  return false;
}

float euclidianDistance(position a, position b){
  return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

creature* createNewGeneration(creature* creatures, int nbCreatures){

  creature* newCreatures = malloc(nbCreatures*sizeof(creature));
  
  int scores[nbCreatures];

  int max1;
  int max2;
            
  int first; 
  int second;

  int creatureScore;
  
  for(int i = 0 ; i < nbCreatures ; i++){
    
    max1 = 0;
    max2 = 0;
  
    first = 0;
    second = 1;
    
    for(int j = 0 ; j < nbCreatures ; j++){

      creatures[j].score = euclidianDistance(creatures[j].originPos, creatures[j].pos);//Add notion of time needed to reach distance
      
      //for now, multiplying the score with the random number would give too much of an advantage to the
      //creatures going the good way 
      /* scores[j] = creatures[j].score+randomBetween(0, 100); */
      creatureScore = creatures[j].score+randomBetween(0, 100);

      if(creatureScore>max1){
	max2 = max1;
	second = first;
	max1 = creatureScore;
	first = j;
      }
      else if(creatureScore > max2){
	max2 = creatureScore;
	second = j;
      }
      
    }

    newCreatures[i] = mate(creatures[first], creatures[second], i);
    
  }

  return newCreatures;
  
}

creature mate(creature c1, creature c2, int id){

  creature baby;
  
  baby.id = id;
  baby.life = INITIAL_LIFE;
  baby.score = 0;
  baby.iterationLastMoved = 0;
  baby.originPos.x = id+1;  
  baby.originPos.y = 1;  
  baby.pos.x = id+1;  
  baby.pos.y = 1;
  baby.gen = mixGenCode(c1.gen, c2.gen);
  
  int nbNeuronsFirstLayer = c1.brain.nbNeuronsFirstLayer;
  int nbNeuronsMiddleLayer = c1.brain.nbNeuronsMiddleLayer;
  int nbNeuronsLastLayer = c1.brain.nbNeuronsLastLayer;
  
  baby.brain = initNeuralNetwork(nbNeuronsFirstLayer, nbNeuronsMiddleLayer, nbNeuronsLastLayer, baby.gen.genes, baby.gen.nbGenes);
  
  return baby;
}

//uniform crossover 
genCode mixGenCode(genCode dad, genCode mom){
  
  genCode gen;
  gen.genes = malloc(dad.nbGenes*sizeof(float));//omg a feminist would kill me
  gen.nbGenes = dad.nbGenes;
  
  for(int i = 0 ; i < dad.nbGenes ; i++){
    
    if(randomBetween(0, 1) == 0)//dad's gene
      gen.genes[i] = dad.genes[i]; 
    else//mom's gene
      gen.genes[i] = mom.genes[i];
    
  }
  
  gen = mutate(gen);
  
  return gen;
  
}

genCode mutate(genCode g){
  
  int selected = randomBetween(0, g.nbGenes);
  g.genes[selected] += randomBetween(-100, 100);
  /* g.genes[selected] = randomBetween(0, ); */

  return g;
  
}
