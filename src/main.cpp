/***********************************************************************
 * (c) Copyright 2018 Microsemi SoC Products Group. All rights reserved.
 * version 1.0 - 17/06/2018 by anton.krug@microsemi.com
 ***********************************************************************/
#include <stdio.h>
#include <float.h>
#include <cmath>
#include <iterator>

#include "titanic-data.h"

// Weights + bias calculated with the following script:
// https://github.com/truhlikfredy/machine-learning-experiments/blob/master/src/titanic-mk2.ipynb

const float weight[] = { 0.08254299f,
		                -0.23828413f,
						 0.12933668f,
						 0.07738735f,
						 0.01722623f };

const float bias     =   0.4832f;

int main() {
  // Initialize the true/false positives/negatives for the confusion matrix
  uint32_t falseNegative = 0;
  uint32_t falsePositive = 0;
  uint32_t trueNegative  = 0;
  uint32_t truePositive  = 0;

  // Iterate over the whole set
  for (uint32_t index = 0; index < titanic_data_size; index++) {
	// Get a single entry from data-set
    const struct titanic_entry entry = titanic_data[index];

    // Calculate a dot product of a passenger and weights
    const float dot = weight[0] * entry.sex    +
                      weight[1] * entry.age    +
                      weight[2] * entry.class1 +
                      weight[3] * entry.class2 +
                      weight[4] * entry.class3;

    // Step 1: Normalize  0 <= (dot + bias) <= 1
    // Step 2: Round up the float into discrete 0/1
    // Step 3: Compare the predicted vs the actual information
    const float    normalized = fminf(1.0f, fmaxf(0.0f, dot + bias));
    const uint32_t prediction = (uint32_t)(round(normalized));
    const bool     correct    = (prediction == entry.survived);

    // Increment the confusion matrix buckets without branching:
    falseNegative += !correct && !prediction;
    falsePositive += !correct &&  prediction;
    trueNegative  +=  correct && !prediction;
    truePositive  +=  correct &&  prediction;
  }

  const float accuracy = (titanic_data_size - (falsePositive + falseNegative)) /
		                 (float)titanic_data_size;

  printf("Confusion matrix:\n");
  printf("\n");
  printf("                  | Predicted death | Predicted survival |\n");
  printf("------------------|-----------------|--------------------|\n");
  printf("Actually died     | %15d | %18d |\n", trueNegative, falsePositive);
  printf("Actually survived | %15d | %18d |\n", falseNegative, truePositive);
  printf("----------------------------------------------------------\n");
  printf("total_entries=%d, correctly_predicated=%d, accuracy=%f \n",
         titanic_data_size, truePositive + trueNegative, accuracy);

  return 0;
}
