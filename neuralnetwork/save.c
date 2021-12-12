#include "neuron.h"
#include "layer.h"
#include "save.h"

#include <stdio.h>
#include <stdlib.h>

void save(char* filename, layer* layers, int nb_layers, int *nb_neurons)
{
	FILE *file;
	file = fopen(filename, "w");
	for (int i = 0; i < nb_layers-1; i++)
	{
		for (int j = 0; j < nb_neurons[i]; j++)
		{
			if (i > 0)
			{
				fprintf(file, "%lf\n", layers[i].neurons[j].bias);
			}
			for (int k = 0; k < nb_neurons[i+1]; k++)
			{
				fprintf(file, "%lf\n", layers[i].neurons[j].out_weights[k]);
			}
		}
	}

	for (int j = 0; j < nb_neurons[nb_layers-1]; j++)
	{
		fprintf(file, "%lf\n", layers[nb_layers-1].neurons[j].bias);
	}
	fclose(file);
}

void load(char* filename, layer* layers, int nb_layers, int *nb_neurons)
{
	FILE *file;
	file = fopen(filename, "r");
	
	for (int i = 0; i < nb_layers-1; i++)
	{
		for (int j = 0; j < nb_neurons[i]; j++)
		{
			if (i > 0)
			{
				fscanf(file, "%f", &layers[i].neurons[j].bias);
			}
			for (int k = 0; k < nb_neurons[i+1]; k++)
			{
				fscanf(file, "%f", &layers[i].neurons[j].out_weights[k]);
			}
		}
	}

	for (int j = 0; j < nb_neurons[nb_layers-1]; j++)
	{
		fscanf(file, "%f", &layers[nb_layers-1].neurons[j].bias);
	}
	fclose(file);
}
