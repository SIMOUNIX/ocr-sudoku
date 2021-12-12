#ifndef NETWORK_H
#define NETWORK_H

#include "neuron.h"
#include "layer.h"

void initialize_weights();
void create_architechture();
void forwardprop();
void backwardprop(int p);
void compute_cost(int i);
void update_weights();
void get_inputs();
void get_desired_outputs();
void feed_input(int i);
void train_neural_network();
void test_nn();

#endif
