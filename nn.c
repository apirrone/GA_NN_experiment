#include "nn.h"

nn initNeuralNetwork(int nbNeuronsFirstLayer, int nbNeuronsMiddleLayer, int nbNeuronsLastLayer, float* genes, int nbGenes){
  
  nn neural_net;
  neural_net.nbNeuronsFirstLayer = nbNeuronsFirstLayer;
  neural_net.nbNeuronsMiddleLayer = nbNeuronsMiddleLayer;
  neural_net.nbNeuronsLastLayer = nbNeuronsLastLayer;
  neural_net.nbLinks = nbGenes;
  neural_net.nbNeurons = nbNeuronsFirstLayer+nbNeuronsMiddleLayer+nbNeuronsLastLayer;
  neural_net.neurons = malloc(neural_net.nbNeurons*sizeof(neuron)); 

  int id = 0; 
  
  //init firstLayer
  for(int i = 0 ; i < nbNeuronsFirstLayer ; i++){
    
    neural_net.neurons[id].id = id;
    
    neural_net.neurons[id].firstLayer = true;
    neural_net.neurons[id].middleLayer = false;
    neural_net.neurons[id].lastLayer = false;

    neural_net.neurons[id].beforeNeighbors = NULL;
    neural_net.neurons[id].nbBeforeNeighbors = 0;
    id++;
    
  }

  //init middleLayer  
  for(int i = 0 ; i < nbNeuronsMiddleLayer ; i++){
    
    neural_net.neurons[id].id = id;
    
    neural_net.neurons[id].firstLayer = false;
    neural_net.neurons[id].middleLayer = true;
    neural_net.neurons[id].lastLayer = false;

    neural_net.neurons[id].nbBeforeNeighbors = neural_net.nbNeuronsFirstLayer;
    neural_net.neurons[id].beforeNeighbors = malloc(neural_net.nbNeuronsFirstLayer*sizeof(int));
    
    int iterBeforeNeighbors = 0;
    
    for(int j = 0 ; j < neural_net.nbNeurons ; j++)
	     if(neural_net.neurons[j].firstLayer)
	neural_net.neurons[id].beforeNeighbors[iterBeforeNeighbors++] = neural_net.neurons[j].id; 
    
    id++;
    
  }
  
  //init lastLayer
  for(int i = 0 ; i < nbNeuronsLastLayer ; i++){
    
    neural_net.neurons[id].id = id;
    
    neural_net.neurons[id].firstLayer = false;
    neural_net.neurons[id].middleLayer = false;
    neural_net.neurons[id].lastLayer = true;

    neural_net.neurons[id].nbBeforeNeighbors = neural_net.nbNeuronsMiddleLayer;
    neural_net.neurons[id].beforeNeighbors = malloc(neural_net.nbNeuronsMiddleLayer*sizeof(int)); 

    int iterBeforeNeighbors = 0;
    for(int j = 0 ; j < neural_net.nbNeurons ; j++)
      if(neural_net.neurons[j].middleLayer)
	neural_net.neurons[id].beforeNeighbors[iterBeforeNeighbors++] = neural_net.neurons[j].id;

    id++;
  }

  neural_net.links = malloc(neural_net.nbLinks*sizeof(neuron_link)); 
  int iterLinks = 0;

  //Fill firstLayer->middleLayer
  for(int i = 0 ; i < neural_net.nbNeurons ; i++)
    if(neural_net.neurons[i].firstLayer)
      for(int j = 0 ; j < neural_net.nbNeurons ; j++)
	if(neural_net.neurons[j].middleLayer){
	  
	  neural_net.links[iterLinks].idNeuronSource = neural_net.neurons[i].id;
	  neural_net.links[iterLinks].idNeuronTarget = neural_net.neurons[j].id;
	  neural_net.links[iterLinks].weight = genes[iterLinks];
	  iterLinks++;
	  
	}

  
  //Fill middleLayer->LastLayer
  for(int i = 0 ; i < neural_net.nbNeurons ; i++)
    if(neural_net.neurons[i].middleLayer)
      
      for(int j = 0 ; j < neural_net.nbNeurons ; j++) 
	if(neural_net.neurons[j].lastLayer){	  

	  neural_net.links[iterLinks].idNeuronSource = neural_net.neurons[i].id;
	  neural_net.links[iterLinks].idNeuronTarget = neural_net.neurons[j].id;
	  neural_net.links[iterLinks].weight = genes[iterLinks];
	  iterLinks++;
	  
	}
  
  neural_net.globalInputs = malloc(neural_net.nbNeuronsFirstLayer*sizeof(float));

  return neural_net;
}

int getIndiceMax(float* tab, int sizeTab){

  float max = FLT_MIN;
  int indice = 0;
  
  for(int i = 0 ; i < sizeTab ; i++)
    if(tab[i] > max){
      max = tab[i];
      indice = i;
    } 

  return indice;  
}

//Inputs :
//0 : obstacle up
//1 : obstacle right
//2 : obstacle down
//3 : obstacle left
int getDirectionFromNeuralNetwork(nn neural_net, float* inputs){

  neural_net.globalInputs = inputs;
  
  float outputs[neural_net.nbNeuronsLastLayer];

  int iterOutputs = 0;
  for(int i = 0 ; i < neural_net.nbNeurons ; i++)
    if(neural_net.neurons[i].lastLayer)
      outputs[iterOutputs++] = getNeuronOutput(neural_net.neurons[i], neural_net); 
  
  return getIndiceMax(outputs, neural_net.nbNeuronsLastLayer); 
}

float getNeuronOutput(neuron n, nn neural_net){
  
  float sum = 0;

  if(n.firstLayer)
    sum += neural_net.globalInputs[n.id];
  
  else
    for(int i = 0 ; i < n.nbBeforeNeighbors ; i++)
      sum += getNeuronOutput(getNeuronById(n.beforeNeighbors[i], neural_net), neural_net)*getWeightLink(n.id, n.beforeNeighbors[i], neural_net);
  
  sum = sigmoid(sum);
  return sigmoid(sum);
}

neuron getNeuronById(int id, nn neural_net){

  for(int i = 0 ; i < neural_net.nbNeurons ; i++)
    if(neural_net.neurons[i].id == id)
      return neural_net.neurons[i];

  neuron n;
  n.id = -1;
  
  return n ;//no neurons found
}

float sigmoid(float x){
  return x / (1. + abs(x));
}

float getWeightLink(int idN1, int idN2, nn neural_net){

  for(int i = 0 ; i < neural_net.nbLinks ; i++){
    if((neural_net.links[i].idNeuronSource == idN1 || neural_net.links[i].idNeuronSource == idN2) &&
       (neural_net.links[i].idNeuronTarget == idN1 || neural_net.links[i].idNeuronTarget == idN2)){
      return neural_net.links[i].weight;
    } 
  }

  return -1.;// Ok ?
  
}
