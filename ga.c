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
    
  }
}

void updateTab(int** tab, int hsize, creature* creatures, int nbCreatures){
  
  for(int i = 0 ; i < hsize ; i++){
    for(int j = 0 ; j < hsize ; j++){
      
      tab[i][j] = 0;
      for(int k = 0 ; k < nbCreatures ; k++){
	
	if(creatures[k].pos.x == i && creatures[k].pos.y == j) {
	  tab[i][j] = 1;
	}
	
      }
      
    }
  }

}

void display(int** tab, int hsize, creature* creatures, int nbCreatures){
  
  for(int j = 0 ; j < hsize ; j++){
    for(int i = 0 ; i < hsize ; i++){

      if(tab[i][j] == 0)
	printw(" ");
      else if(tab[i][j] == 2)
	printw("o");
      else
	printw("x");
      
      addch(' ');
      
    }
    
    addch('\n'); 
  }  
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
  printf("USAGE : ./ga <size_of_grid> <nb_creatures> <speed>\n"); 
}

int main(int argc, char* argv[]){

  if(argc!=4){
    usage();
    return EXIT_SUCCESS;
  }

  srand(time(NULL));
  
  int hs = atoi(argv[1]);
  int** tab = initTab(hs);
  int nbCreatures = atoi(argv[2]);
  
  float speed = atof(argv[3]);
  speed*=100000;

  creature* creatures = initCreatures(nbCreatures);
  
  initscr();

  int iteration = 0;
  int epoch = 0;

  while(1){
  
     //The current generation runs until all the creatures stopped moving for a while
     /* while(oneCreatureIsMoving(creatures, nbCreatures, iteration)){//Current generation */
    while(iteration<100){

      updateTab(tab, hs, creatures, nbCreatures);
       updateCreatures(creatures, nbCreatures, hs, tab, iteration);
       display(tab, hs, creatures, nbCreatures);
       
       /* move(LINES - 3, COLS - 18); */
       /* printw("Speed : %d", speed); */
       
       move(LINES - 2, COLS - 18);
       printw("Epoch : %d", epoch);
      
       move(LINES - 1, COLS - 18);
       printw("Iteration : %d", iteration);
    
       usleep(speed);//Microseconds
       refresh();

       /* if(getch() == KEY_UP){ */
       /* 	 speed-=100; */
       /* } */
       /* if(getch() == KEY_DOWN) */
       /* 	 speed+=100; */
       
       clear();
       iteration++;
    
     }

     iteration = 0;
    
     epoch ++;

     creatures = createNewGeneration(creatures, nbCreatures);
  }

  printf("DONE\n");
  endwin();
    
  return 0;
}

