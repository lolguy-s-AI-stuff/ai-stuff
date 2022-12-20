#include <stdio.h>
#include <AI/AI.h>
#include <random>
#include <chrono>

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
				curr[x] = tmp[x];
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
						tmp[k] += (unsigned char)(input[j] * (weights[i][j][k] / 255.0));
					}
					else
					{
						tmp[k] += (unsigned char)(curr[j] * (weights[i][j][k] / 255.0));
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
// trash
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

	int cost = abs(output[0] - traindata[(training_number % 3) - 1][3]);
	if (lastcost == 0)
		lastcost = cost;
	if (cost >= lastcost)
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
		lastcost = cost;
		printf("training number: %i,\n cost: %i\n", training_number, cost);
		it++;
		goto CONCLUSION;
	}
	CONCLUSION:
	if(it >= 3){// protect from the devil xD
		training_number++;
		it = 1;
	}



}
