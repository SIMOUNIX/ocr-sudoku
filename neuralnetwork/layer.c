#include "layer.h"
#include "neuron.h"

layer create_layer(int number_of_neurons)
{
    layer lay;
    lay.nb_neurons = number_of_neurons;
    lay.neurons = (struct neuron_t *) malloc(number_of_neurons * sizeof(struct neuron_t));
    return lay;
}
