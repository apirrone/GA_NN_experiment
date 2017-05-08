
#include "creature.h"

int randomBetween(int min, int max){
  return (rand() % (max + 1 - min)) + min;
}

void printGenCode(genCode gc){

  for(int i = 0 ; i < gc.nbGenes ; i++)
    printf("%d : %f\n", i, gc.genes[i]);

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

  int nbNeuronsFirstLayer = 5;
  int nbNeuronsMiddleLayer = 15;
  int nbNeuronsLastLayer = 4;

  int nbGenes = nbNeuronsFirstLayer*nbNeuronsMiddleLayer + nbNeuronsMiddleLayer*nbNeuronsLastLayer;
  
  for(int i = 0 ; i < nbCreatures ; i++){
    
    creatures[i].id = i;
    creatures[i].life = INITIAL_LIFE;
    creatures[i].score = 1;
    creatures[i].iterationLastMoved = 0;
    
    creatures[i].originPos.x = 1;  
    creatures[i].originPos.y = i+1;
    
    creatures[i].pos.x = 1;  
    creatures[i].pos.y = i+1;
    
    creatures[i].prevPos.x = 1;
    creatures[i].prevPos.y = i+1;
    
    creatures[i].gen = initGenCode(nbGenes);
    
    creatures[i].brain = initNeuralNetwork(nbNeuronsFirstLayer, nbNeuronsMiddleLayer, nbNeuronsLastLayer, creatures[i].gen.genes, creatures[i].gen.nbGenes);
    
  }

  return creatures;
  
}

int getIdByCoords(int x, int y, creature* creatures, int nbCreatures){

  for(int i = 0 ; i < nbCreatures ; i++)
    if(creatures[i].pos.x == x && creatures[i].pos.y == y)
      return creatures[i].id; 

  return -1;
  
}

float* getVision(creature c, int tabSize, int** tab, creature* creatures, int nbCreatures){
  
  float* vision = malloc(VISION_SIZE*sizeof(float));
  position coords = c.pos;    
  if( coords.y-1 <=0 || collision(coords, 0, creatures, nbCreatures, tab, tabSize))
    vision[0] = 1-(randomBetween(0, 30)/100); 
  else
    vision[0] = 0; 

  if( coords.x+1 >= tabSize || collision(coords, 1, creatures, nbCreatures, tab, tabSize))
    vision[1] = 1-(randomBetween(0, 30)/100); 
  else
    vision[1] = 0; 

  if( coords.y+1 >= tabSize || collision(coords, 2, creatures, nbCreatures, tab, tabSize))
    vision[2] = 1-(randomBetween(0, 30)/100); 
  else
    vision[2] = 0;

  if( coords.x-1 <=0 || collision(coords, 3, creatures, nbCreatures, tab, tabSize))
    vision[3] = 1-(randomBetween(0, 30)/100); 
  else
    vision[3] = 0;

  return vision;
  
}
  
void updateCreatures(creature* creatures, int nbCreatures, int tabSize, int** tab, int iteration){
  
  for(int i = 0 ; i < nbCreatures ; i++){
    
    float* vision = getVision(creatures[i], tabSize, tab, creatures, nbCreatures);

    moveCreature(&creatures[i], tab, tabSize, iteration, creatures, nbCreatures, vision);
    free(vision);
    if((creatures[i].pos.x == creatures[i].prevPrevPos.x && creatures[i].pos.y == creatures[i].prevPrevPos.y) ||
       (creatures[i].pos.x == creatures[i].prevPos.x && creatures[i].pos.y == creatures[i].prevPos.y))
	   creatures[i].score/0.1;
    
  }
  
}

bool collision(position p, int direction, creature* creatures, int nbCreatures, int** tab, int hs){

  position newPos;
  
  switch(direction){
  case 0 :
    newPos.x = p.x;
    newPos.y = p.y-1;
    break;
      
  case 1 :
    newPos.x = p.x+1;
    newPos.y = p.y;
    break;
      
  case 2 :
    newPos.x = p.x;
    newPos.y = p.y+1;
    break;
      
  case 3 :
    newPos.x = p.x-1;
    newPos.y = p.y;
    break;
  default:
    printf("creature.c:159 : WEIRD\n");
    break;
  }
  
  for(int i = 0 ; i < nbCreatures ; i++)
    if(creatures[i].pos.x == newPos.x && creatures[i].pos.y == newPos.y)
      return true;

  if(tab[newPos.x][newPos.y] == 2)
    return true;

  return false;  
}

//0 : up
//1 : right
//2 : down
//3 : left
bool canMove(position p, int direction, int** tab, int hs, creature* creatures, int nbCreatures){

  if(direction == 0 || direction == 2)
    if(p.y > 0 && p.y < hs-1)
      if(!collision(p, direction, creatures, nbCreatures, tab, hs))
	return true; 
  
  if(direction == 1 || direction == 3)
    if(p.x > 0 && p.x < hs-1)
      if(!collision(p, direction, creatures, nbCreatures, tab, hs))
	return true; 

  return false;  
}

//0 : up
//1 : right
//2 : down
//3 : left
void moveCreature(creature *c, int** tab, int hs, int iteration, creature* creatures, int nbCreatures, float* vision){
  
  float inputs[c->brain.nbNeuronsFirstLayer];

  inputs[0] = vision[0];
  inputs[1] = vision[1];
  inputs[2] = vision[2];
  inputs[3] = vision[3];
  inputs[4] = iteration;
  
  int direction = getDirectionFromNeuralNetwork(c->brain, inputs);

  c->prevPrevPos.x = c->prevPos.x;
  c->prevPrevPos.y = c->prevPos.y;
  
  c->prevPos.x = c->pos.x;
  c->prevPos.y = c->pos.y;
  
  if(canMove(c->pos, direction, tab, hs, creatures, nbCreatures)){

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
  
  float scores[nbCreatures];

  int max1;
  int max2;
            
  int first; 
  int second;

  int creatureScore;
  for(int i = 0 ; i < nbCreatures ; i++)
    scores[i] = euclidianDistance(creatures[i].originPos, creatures[i].pos)*creatures[i].score;

  float maxScore = 0;
  
  for(int i = 0 ; i < nbCreatures ; i++)
    if(scores[i] > maxScore)
      maxScore = scores[i];

  for(int i = 0 ; i < nbCreatures ; i++)
    scores[i]/=maxScore;

  int intScores[nbCreatures];

  for(int i = 0 ; i < nbCreatures ; i++)
    intScores[i] = scores[i]*100;

  int sum = 0;

  for(int i = 0 ; i < nbCreatures ; i++)
    sum+=(intScores[i]*intScores[i]);

  int association[sum];
  int final[sum];
  
  int iterFinal = 0;
  for(int i = 0 ; i < nbCreatures ; i++)
    for(int j = 0 ; j < intScores[i]*intScores[i] ; j++){
      final[iterFinal] = intScores[i]*intScores[i];
      association[iterFinal++] = i;
    } 
  

  for(int i = 0 ; i < nbCreatures ; i++){
    
    int c1 = randomBetween(0, sum-1);
    int c2 = randomBetween(0, sum-1);
    
    while(creatures[association[c2]].id == creatures[association[c1]].id)
      c2 = randomBetween(0, sum-1);
    
    
    newCreatures[i] = mate(creatures[association[c1]], creatures[association[c2]], i);    
			   
  }
    
  /* for(int i = 0 ; i < nbCreatures ; i++){ */
    
  /*   max1 = 0; */
  /*   max2 = 0; */
  
  /*   first = 0; */
  /*   second = 1; */
    
  /*   for(int j = 0 ; j < nbCreatures ; j++){ */

  /*     creatures[j].score = (euclidianDistance(creatures[j].originPos, creatures[j].pos))*creatures[j].score;//Add notion of time needed to reach distance */
      
  /*     /\* creatureScore = creatures[j].score+randomBetween(0, 1); *\/ */
  /*     /\* creatureScore = creatures[j].score; *\/ */

  /*     if(creatureScore>max1){ */
  /* 	max2 = max1; */
  /* 	second = first; */
  /* 	max1 = creatureScore; */
  /* 	first = j; */
  /*     } */
  /*     else if(creatureScore > max2){ */
  /* 	max2 = creatureScore; */
  /* 	second = j; */
  /*     } */
      
  /*   } */

  /*   if(i==0){ */
  /*     newCreatures[i] = creatures[first]; */
  /*     newCreatures[i].id = 0; */
  /*     newCreatures[i].life = INITIAL_LIFE; */
  /*     newCreatures[i].score = 1; */
  /*     newCreatures[i].iterationLastMoved = 0; */
  /*     newCreatures[i].originPos.x = 1;   */
  /*     newCreatures[i].originPos.y = 1;   */
  /*     newCreatures[i].pos.x = 1;   */
  /*     newCreatures[i].pos.y = 1; */
  /*     newCreatures[i].prevPos.x = 1;   */
  /*     newCreatures[i].prevPos.y = 1; */
  /*     newCreatures[i].prevPrevPos.x = 1;   */
  /*     newCreatures[i].prevPrevPos.y = 1; */
      
      
  /*   } */
  /*   else */
  /*     newCreatures[i] = mate(creatures[first], creatures[second], i); */
    
  /* } */

  return newCreatures;
  
}

creature mate(creature c1, creature c2, int id){

  creature baby;
  
  baby.id = id;
  baby.life = INITIAL_LIFE;
  baby.score = 1;
  baby.iterationLastMoved = 0;
  baby.originPos.x = 1;  
  baby.originPos.y = id+1;
  
  baby.pos.x = 1;  
  baby.pos.y = id+1;
  
  baby.prevPos.x = 1;  
  baby.prevPos.y = id+1;
  
  baby.prevPrevPos.x = 1;  
  baby.prevPrevPos.y = id+1;
  
  baby.gen = mixGenCode(c1.gen, c2.gen); 
  /* baby.gen = mixGenCodeOneGen(c1.gen, c2.gen); */ 
  /* baby.gen = mixGenChoseOne(c1.gen, c2.gen); */
  
  int nbNeuronsFirstLayer = c1.brain.nbNeuronsFirstLayer;
  int nbNeuronsMiddleLayer = c1.brain.nbNeuronsMiddleLayer;
  int nbNeuronsLastLayer = c1.brain.nbNeuronsLastLayer;
  
  baby.brain = initNeuralNetwork(nbNeuronsFirstLayer, nbNeuronsMiddleLayer, nbNeuronsLastLayer, baby.gen.genes, baby.gen.nbGenes);
  
  return baby;
}

genCode mixGenChoseOne(genCode dad, genCode mom){
  genCode gen;
  int choice = randomBetween(0, 1);
  gen = (choice?dad:mom);
  gen = mutate(gen);
  
  return gen;
}

genCode mixGenCodeOneGen(genCode dad, genCode mom){
  genCode gen;

  int choice = randomBetween(0, dad.nbGenes);
  gen = dad;
  gen.genes[choice] = mom.genes[choice];

  gen = mutate(gen);
  
  return gen;
  
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
  
  int selected = randomBetween(0, g.nbGenes-1);
  g.genes[selected] += randomBetween(-100, 100);

  return g;
  
}
