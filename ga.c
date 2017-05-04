#include "ga.h"

#include "creature.h"
#include "nn.h"

char intChar(int a){
  
  switch(a){
    
  case 0 :
    return ' ';
    break;
    
  case 1 :
    return 'x';
    break;
    
  case 2 :
    return 'o';
    break;
    
  case 3 :
    return 'f';
    break;
    
  }
}

void updateTab(int** tab, int hs, creature* creatures, int nbCreatures, position* obstacles, int nbObstacles){
  
  for(int i = 0 ; i < hs ; i++)
    for(int j = 0 ; j < hs ; j++)
      tab[i][j] = 0;
  
  for(int i = 0 ; i < nbObstacles ; i++)
    tab[obstacles[i].x][obstacles[i].y] = 2; 
  
  
  for(int k = 0 ; k < nbCreatures ; k++)	
    tab[creatures[k].pos.x][creatures[k].pos.y] = 1;
  

}

void display(int** tab, int hsize, creature* creatures, int nbCreatures){

  for(int j = 0 ; j < hsize ; j++){
    for(int i = 0 ; i < hsize ; i++){

      if(tab[i][j] == 0){
	attron(COLOR_PAIR(3));	
	printw(" ");
      }
      else if(tab[i][j] == 2){
	attron(COLOR_PAIR(2));
	printw(" ");//obstackes
      }
      else{
	if(getIdByCoords(i, j, creatures, nbCreatures) == 0)
	  attron(COLOR_PAIR(1));//red, best of last generation
	else
	  attron(COLOR_PAIR(4));

	printw(" ");//Creature
      }
      
      addch(' ');
      
    }
    
    addch('\n'); 
  }  
}


position* initObstacles(int s, int hs, creature* creatures, int nbCreatures){

  position* obstacles = malloc(s*sizeof(position));
  
  for(int i = 0 ; i < s ; ++i){
    obstacles[i].x = randomBetween(0, hs-1);
    obstacles[i].y = randomBetween(0, hs-1);
    for(int j = 0 ; j < nbCreatures ; j++)
      if(creatures[j].pos.x == obstacles[i].x && creatures[j].pos.y == obstacles[i].y)
	i--;
  }

  return obstacles; 
  
}

int** initTab(int hs){
  
  int** tab = malloc(hs*sizeof(int*));
  for(int i = 0 ; i < hs ; i++){
    tab[i] = malloc(hs*sizeof(int));
  }

  for(int i = 0 ; i < hs ; i++)
    for(int j = 0 ; j < hs ; j++)
      tab[i][j] = 0;
  
  return tab;

}

void usage(){
  printf("USAGE : ./ga <size_of_grid> <nb_creatures> <speed> <nb_obstacles>\n"); 
}

int main(int argc, char* argv[]){

  if(argc!=5){
    usage();
    return EXIT_SUCCESS;
  }

  srand(time(NULL));
  
  int hs = atoi(argv[1]);
  int** tab = initTab(hs);
  int nbCreatures = atoi(argv[2]);
  creature* creatures = initCreatures(nbCreatures);
  int nbObstacles = atoi(argv[4]);
  position* obstacles = initObstacles(nbObstacles, hs, creatures, nbCreatures);
  
  float s = atof(argv[3]);
  s*=100000;
  s+=1;
  
  WINDOW* w = initscr();
  cbreak();
  nodelay(w, TRUE);
  keypad(stdscr, TRUE);
  int iteration = 0;
  int epoch = 0;
  int c;

  start_color();

  init_pair(1, COLOR_BLACK, COLOR_RED);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  init_pair(3, COLOR_WHITE, COLOR_WHITE);
  init_pair(4, COLOR_BLACK, COLOR_BLUE);
  
  while(1){
  
    //The current generation runs until all the creatures stopped moving for a while
    /* while(oneCreatureIsMoving(creatures, nbCreatures, iteration)){//Current generation */
    while(iteration<100 && oneCreatureIsMoving(creatures, nbCreatures, iteration)){

      updateTab(tab, hs, creatures, nbCreatures, obstacles, nbObstacles);
      updateCreatures(creatures, nbCreatures, hs, tab, iteration);

      display(tab, hs, creatures, nbCreatures);
      attron(COLOR_PAIR(2));	      
      move(LINES - 3, COLS - 18);
      printw("Speed : %f", 1/s);
       
      move(LINES - 2, COLS - 18);
      printw("Epoch : %d", epoch);
      
      move(LINES - 1, COLS - 18);
      printw("Iteration : %d", iteration);
    
      usleep(s);//Microseconds
      refresh();

      c = wgetch(w);
      if(c == KEY_UP){
	if(s>1000)
	  s-=1000;
      }
      if(c == KEY_DOWN)
	s+=1000;
       
      clear();
      iteration++;
    }

    iteration = 0;
    
    epoch ++;
    obstacles = initObstacles(nbObstacles, hs, creatures, nbCreatures);
    creatures = createNewGeneration(creatures, nbCreatures);
  }

  printf("DONE\n");
  endwin();
    
  return 0;
}

