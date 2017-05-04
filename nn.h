#ifndef _NN_H_
#define _NN_H_

#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>

struct neuron_link{
  int idNeuronSource;
  int idNeuronTarget;
  float weight;
};

typedef struct neuron_link neuron_link;

struct neuron{
  
  int id;
  int threshold;

  bool firstLayer;
  bool middleLayer;
  bool lastLayer;

  //Connected neurons before this neuron
  int* beforeNeighbors;//ids
  /* struct neuron* beforeNeighbors; */
  int nbBeforeNeighbors;
  
};

typedef struct neuron neuron;

//Fully connected
struct neural_network{

  neuron* neurons;
  
  /* neuron* firstLayer; */
  /* neuron* middleLayer; */
  /* neuron* lastLayer; */
  
  int nbNeuronsFirstLayer;
  int nbNeuronsMiddleLayer;
  int nbNeuronsLastLayer;

  int nbNeurons;

  neuron_link* links;
  int nbLinks;// as much genes as there are links

  float *globalInputs;
  
};

typedef struct neural_network nn;

float getWeightLink(int idN1, int idN2, nn neural_net);
float sigmoid(float x);
float getNeuronOutput(neuron n, nn neural_net);
int getDirectionFromNeuralNetwork(nn neural_net, float* inputs);
nn initNeuralNetwork(int nbNeuronsFirstLayer, int nbNeuronsMiddleLayer, int nbNeuronsLastLayer, float* genes, int nbGenes);
neuron getNeuronById(int id, nn neural_net);
float sigmoid(float x);


#endif
