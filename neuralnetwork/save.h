#ifndef SAVE_H
#define SAVE_H

#include "neuron.h"
#include "layer.h"

void save(char* filename, layer* layers, int nb_layers, int *nb_neurons);
void load(char* filename, layer* layers, int nb_layers, int *nb_neurons);

#endif
