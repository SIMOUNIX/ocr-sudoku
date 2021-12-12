#include "neuron.h"
#include "layer.h"
#include "network.h"
#include "save.h"

#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../utils/image.h"
#include "bmptoarray.h"

layer *lay = NULL;
int number_of_layers;
int *number_of_neurons;
float alpha;
float *cost;
float full_cost;
float **input;
float **desired_outputs;
long num_training_ex;
int n=1;
int num_used_ex;



void initialize_weights()
{
    for (int i = 0; i < number_of_layers-1; i++)
    {
        for (int j = 0; j < lay[i].nb_neurons; j++)
        {
            for (int k = 0; k < number_of_neurons[i+1]; k++)
            {
                lay[i].neurons[j].out_weights[k] = (double)rand()/(double)RAND_MAX;
		//printf("%lf", lay[i].neurons[j].out_weights[k]);
                lay[i].neurons[j].dw[k] = 0.0;
            }

            if (i > 0)
            {
                lay[i].neurons[j].bias = (double)rand()/(double)RAND_MAX;
		//printf("%lf", lay[i].neurons[j].bias);
            }
        }
    }

    for(int j = 0; j<number_of_neurons[number_of_layers-1]; j++)
    {
        lay[number_of_layers-1].neurons[j].bias = (double)rand()/(double)RAND_MAX;
    }
}

void create_architechture()
{
    lay = (layer*) malloc(number_of_layers * sizeof(layer));

    for(int i = 0; i < number_of_layers; i++)
    {
        lay[i] = create_layer(number_of_neurons[i]);
        lay[i].nb_neurons = number_of_neurons[i];

        for(int j = 0; j < number_of_neurons[i]; j++)
        {
            if(i < number_of_layers - 1)
            {
                lay[i].neurons[j] = create_neuron(number_of_neurons[i+1]);
            }
        }
    }

    initialize_weights();
}

void forwardprop()
{
    for (int i = 1; i < number_of_layers; i++)
    {
        for (int j = 0; j < number_of_neurons[i]; j++)
        {
            lay[i].neurons[j].z = lay[i].neurons[j].bias;

            for (int k = 0; k < number_of_neurons[i-1]; k++)
            {
                lay[i].neurons[j].z = lay[i].neurons[j].z + (lay[i-1].neurons[k].out_weights[j] * lay[i-1].neurons[k].actv);
		//printf("%lf", lay[i].neurons[j].z);
            }

            if (i < number_of_layers-1)
            {
                /*if(lay[i].neurons[j].z < 0)
                {
                    lay[i].neurons[j].actv = 0.01 * lay[i].neurons[j].z;
                }
                else
                {
                    lay[i].neurons[j].actv = lay[i].neurons[j].z;
                }*/
		lay[i].neurons[j].actv = 1/(1 + exp(-lay[i].neurons[j].z));

            }

            else
            {
		float sum = 0;
		float max = 0;
		for (int k = 0; k < number_of_neurons[i]; k++)
		{
			if (lay[i].neurons[k].z > max)
				max = lay[i].neurons[k].z;
		}
		for (int k = 0; k < number_of_neurons[i]; k++)
		{
			sum+= exp(lay[i].neurons[k].z - max);
		}
		lay[i].neurons[j].actv = exp(lay[i].neurons[j].z - max)/sum;
                //lay[i].neurons[j].actv = 1/(1+exp(-lay[i].neurons[j].z));
		//printf("%lf", lay[i].neurons[j].actv);
            }
        }
    }
}

void backwardprop(int p)
{
    for(int j = 0; j<number_of_neurons[number_of_layers-1];j++)
    {
	/*lay[number_of_layers-1].neurons[j].z = desired_outputs[p][j] - lay[number_of_layers-1].neurons[j].actv;
	if (lay[number_of_layers-1].neurons[j].z >= 0)
        	lay[number_of_layers-1].neurons[j].dz = lay[number_of_layers-1].neurons[j].z * cost[j];
	else
		lay[number_of_layers-1].neurons[j].dz = lay[number_of_layers-1].neurons[j].z * 0.01 * cost[j];*/
	
	lay[number_of_layers-1].neurons[j].dz = (-desired_outputs[p][j]/lay[number_of_layers-1].neurons[j].actv) * (lay[number_of_layers-1].neurons[j].actv) * (1 - lay[number_of_layers-1].neurons[j].actv);

        for(int k=0;k<number_of_neurons[number_of_layers-2];k++)
        {
            lay[number_of_layers-2].neurons[k].dw[j] = (lay[number_of_layers-1].neurons[j].dz * lay[number_of_layers-2].neurons[k].actv);
            lay[number_of_layers-2].neurons[k].dactv = lay[number_of_layers-2].neurons[k].out_weights[j] * lay[number_of_layers-1].neurons[j].dz;
        }

        lay[number_of_layers-1].neurons[j].dbias = lay[number_of_layers-1].neurons[j].dz;
    }

    for(int i=number_of_layers-2;i>0;i--)
    {
        for(int j=0;j<number_of_neurons[i];j++)
        {
            /*if(lay[i].neurons[j].z >= 0)
            {
                lay[i].neurons[j].dz = 1;
            }
            else
            {
                lay[i].neurons[j].dz = 0.01;
            }*/
	    lay[i].neurons[j].dz = (lay[i].neurons[j].actv * (1 - lay[i].neurons[j].actv));

            for(int k=0;k<number_of_neurons[i-1];k++)
            {
                lay[i-1].neurons[k].dw[j] = lay[i].neurons[j].dz * lay[i-1].neurons[k].actv;

                if(i>1)
                {
                    lay[i-1].neurons[k].dactv = lay[i-1].neurons[k].out_weights[j] * lay[i].neurons[j].dz;
                }
            }

            lay[i].neurons[j].dbias = lay[i].neurons[j].dz;
        }
    }
}

void compute_cost(int i)
{
    float tcost=0;

    for(int j=0;j<number_of_neurons[number_of_layers-1];j++)
    {
        cost[j] = desired_outputs[i][j] - lay[number_of_layers-1].neurons[j].actv;
        tcost = tcost + cost[j];
    }

    full_cost = (full_cost + tcost)/n;

    n++;
}

void update_weights()
{
    int i,j,k;

    for(i=0;i<number_of_layers-1;i++)
    {
        for(j=0;j<number_of_neurons[i];j++)
        {
            for(k=0;k<number_of_neurons[i+1];k++)
            {
                lay[i].neurons[j].out_weights[k] = (lay[i].neurons[j].out_weights[k]) - (alpha * lay[i].neurons[j].dw[k]);
            }

            lay[i].neurons[j].bias = lay[i].neurons[j].bias - (alpha * lay[i].neurons[j].dbias);
        }
    }
}

void get_inputs()
{
    int i;

    char *path = malloc(21*sizeof(char));
    for(i = 1; i <= num_training_ex; i++)
    {
	sprintf(path, "test4/%05d.bmp", i);
	SDL_Surface *image = loadImage(path);
        input[i-1] = bmptoarray(image);
	/*for (int j = 0; j < image->h; j++)
	{
		for (int k = 0; k < image->w; k++)
		{
			printf("%d ", (int) input[i-1][image->w * j + k]);
		}
		printf("\n");
	}
	printf("\n");
	//printf("KMN");*/
    }
}

void get_desired_outputs()
{
    int i;

    FILE *file = fopen("test4.txt", "r");
    for(i = 0; i < num_training_ex; i++)
    {
	int x;
        fscanf(file, "%d", &x);
	desired_outputs[i][x] = 1;
	//for (int j = 0; j < 25; j++)
		//fscanf(file, "%d", &x);
	//printf("%d, %d\n", i, x);
    }
    fclose(file);
}

void feed_input(int i)
{
	for(int  j = 0; j < number_of_neurons[0];j++)
	{
		lay[0].neurons[j].actv = input[i][j];
	}
}

void train_neural_net()
{
    int i;
    int it=0;

    for(it=0;it<1000;it++)
    {
	long long good = 0;
        for(i=0;i<num_used_ex;i++)
        {
            feed_input(i);
            forwardprop();
	    float max = 0;
	    int max_index = 0;
	    int expected = 0;
	    //printf("HA");
	    for (int j = 0; j < number_of_neurons[number_of_layers-1]; j++)
	    {
		    printf("%f ", lay[number_of_layers-1].neurons[j].actv);
		    if (lay[number_of_layers-1].neurons[j].actv > max)
		    {
			    max = lay[number_of_layers-1].neurons[j].actv;
			    max_index = j;
		    }
	    }
	    for (int j = 0; j < number_of_neurons[number_of_layers-1]; j++)
		    if (desired_outputs[i][j] == 1)
			    expected = j;
	    printf("\n");
	    for (int j = 0; j < number_of_neurons[number_of_layers-1]; j++)
		    printf("%f ", desired_outputs[i][j]);
	    printf("\nExpected : %d\n", expected);
	    printf("Output : %d\n\n", max_index);
	    if (expected == max_index)
		    good++;

            compute_cost(i);
            backwardprop(i);
            update_weights();
        }
	printf("Precision : %lld good from %ld\n", good, num_training_ex);
    }
}

void test_nn()
{
	for(int i = 0; i < number_of_neurons[0]; i++)
	{
		if(scanf("%f",&lay[0].neurons[i].actv)){};
	}
	forwardprop();
}

int main()
{
    int i;

    number_of_layers = 3;

    num_used_ex = 5000;
    num_training_ex = 5000;

    alpha = 0.00001f;

    number_of_neurons = (int*) malloc(number_of_layers * sizeof(int));
    memset(number_of_neurons, 0, number_of_layers *sizeof(int));

    number_of_neurons[0] = 28*28;
    number_of_neurons[1] = 50;
    number_of_neurons[2] = 10;

    create_architechture();

    //load("weights.txt", lay, number_of_layers, number_of_neurons);

    input = (float**) malloc(num_training_ex * sizeof(float *));
    for(i = 0; i < num_training_ex; i++)
    {
        input[i] = (float*) malloc(number_of_neurons[0] * sizeof(float));
    }

    desired_outputs = (float**) malloc(num_training_ex * sizeof(float *));
    for(i = 0; i < num_training_ex; i++)
    {
        desired_outputs[i] = (float*) malloc(number_of_neurons[number_of_layers-1] * sizeof(float));
    }

    cost = (float *) malloc(number_of_neurons[number_of_layers-1] * sizeof(float));
    memset(cost,0,number_of_neurons[number_of_layers-1]*sizeof(float));

    get_inputs();
    printf("finito inputs");

    get_desired_outputs();
    printf("finito outputs");

    printf("\n");
    train_neural_net();
    

    /*for (int i = 0; i < number_of_layers - 1; i++)
    {
        for (int j = 0; j < number_of_neurons[i]; j++)
        {
            free(lay[i].neurons[j].dw);
        }
    }*/

    /*for (int i = 0; i < num_training_ex; i++)
    {
        free(input[i]);
    }

    for (int i = 0; i < num_training_ex; i++)
    {
        free(desired_outputs[i]);
    }
    
    free(cost);
    free(number_of_neurons);*/
    
    save("weights.txt", lay, number_of_layers, number_of_neurons);

    return 0;
}

