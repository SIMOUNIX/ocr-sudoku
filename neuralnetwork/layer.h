#ifndef LAYER_H
#define LAYER_H

#include "neuron.h"

typedef struct layer_t
{
	int nb_neurons;
	struct neuron_t *neurons;
} layer;

layer create_layer(int number_of_neurons);

#endif
