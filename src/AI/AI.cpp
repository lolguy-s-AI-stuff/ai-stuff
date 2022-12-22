#include <stdio.h>
#include <AI/AI.h>
#include <random>
#include <chrono>
short activation_function(short inp){
	return (char) (inp * 0.6)* (inp / 255) +2;
}
short costfunc(unsigned char expected,unsigned char got){
	char error = abs(expected - got);
	return error * error;
}

unsigned char *AI::run(unsigned char *input)
{
	for (size_t i = 0; i < LAYER_DEPTH; i++)
	{
		curr[i] = 0;
	}

	for (size_t i = 0; i < LAYER_DEPTH; i++)
	{
		tmp[i] = 0;
	}

	printf("running AI..\n");

	for (size_t i = 0; i < LAYER_COUNT; i++)
	{
		for (size_t x = 0; x < LAYER_DEPTH; x++)
		{
			if (i > 0)
			{
				curr[x] = activation_function(tmp[x] + 100) - 100;
				tmp[x] = 0;
			}
		}
		for (size_t j = 0; j < LAYER_DEPTH; j++)
		{

			for (size_t k = 0; k < LAYER_DEPTH; k++)
			{
				if (j <= (topology[i] - 1))
				{
					if (i == 0)
					{
						tmp[k] += (unsigned char)(input[j] * (weights[i][j][k] / 255.0)) + biases[i][j];
					}
					else
					{
						tmp[k] += (unsigned char)(curr[j] * (weights[i][j][k] / 255.0)) + biases[i][j];
					}
				}
			}
		}
	}
	return curr;
}
unsigned char lastweights[3][4][4]{
	{{255, 0, 0, 0}, {255, 0, 0, 0}, {255, 0, 0, 0}, {0, 0, 0, 0}},
	{{255, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};
char lastbiases[3][4]{
    	{0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
};
unsigned char input[3]{0, 0, 0};
unsigned char *output;
int training_number = 1;
int lastcost = 0;
int it = 1;
void AI::train()
{
	for (size_t i = 0; i < 3; i++)
	{
		input[i] = traindata[(training_number % 3) - 1][i];
	}
	output = run(input);
	printf("Iteration %i:", it);

	short cost = costfunc( traindata[(training_number % 3) - 1][3],output[0]);
	if (lastcost == 0)
		lastcost = cost;
	if (cost > lastcost)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				for (size_t k = 0; k < 4; k++)
				{
					weights[i][j][k] = lastweights[i][j][k];
				}
			}
		}
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				biases[i][j] = lastbiases[i][j];
			}
		}
		it++;
		goto CONCLUSION;
	}
	else
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				for (size_t k = 0; k < 4; k++)
				{
					lastweights[i][j][k] = weights[i][j][k];
				}
			}
		}
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				for (size_t k = 0; k < 4; k++)
				{
					weights[i][j][k] += (rand() % 4) - 2;
				}
			}
		}
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				lastbiases[i][j] = biases[i][j];
			}
		}
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
					biases[i][j] += (rand() % 4) - 2;
			}
		}
		lastcost = cost;
		printf("training number: %i,\n cost: %i\n", training_number, cost);
		it++;
		goto CONCLUSION;
	}
	CONCLUSION:
	if(it >= 3){// protect from the devil xD
		if (cost == 0){
			training = false;
		}
		training_number++;
		it = 1;
	}

}
void AI::save(FILE* file){
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			fwrite(weights[i][j], LAYER_DEPTH, LAYER_COUNT*LAYER_DEPTH, file);
		}
	}
	for (size_t i = 0; i < 2; i++)
	{
			fwrite(biases[i], LAYER_DEPTH, LAYER_COUNT, file);
	}

}
void AI::load(FILE* file){
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			fread(weights[i][j], LAYER_DEPTH, LAYER_COUNT*LAYER_DEPTH, file);
		}
	}
	for (size_t i = 0; i < 2; i++)
	{
			fread(biases[i], LAYER_DEPTH, LAYER_COUNT, file);
	}
}

