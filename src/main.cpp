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

const float weight[] = { 0.08254299f, -0.23828413f, 0.12933668f,  0.07738735f, 0.01722623f };
const float bias     = 0.4832f;


int main() {
  // Initialize the true/false positives/negatives for the confusion matrix
  uint32_t TN = 0;
  uint32_t TP = 0;
  uint32_t FN = 0;
  uint32_t FP = 0;

  // Iterate over the whole set
  for (uint32_t index = 0; index < titanic_data_size; index++) {
	// Get a single entry from data-set
    const struct titanic_entry entry = titanic_data[index];

    // Calculate a dot product of an entry and weights
    const float dot = weight[0] * entry.sex    +
    		          weight[1] * entry.age    +
                      weight[2] * entry.class1 +
                      weight[3] * entry.class2 +
                      weight[4] * entry.class3;

    // Normalize  0 <= (dot + bias) <= 1
    const float    normalized = fminf(1.0f, fmaxf(0.0f, dot + bias));
    const uint32_t prediction = (uint32_t)(round(normalized));
    const bool     correct    = (prediction == entry.survived);

    // Increment the correct TN/TP/FN/FP bucket:
    TP += (correct  && prediction == 1)? 1 : 0;
    TN += (correct  && prediction == 0)? 1 : 0;
    FP += (!correct && prediction == 1)? 1 : 0;
    FN += (!correct && prediction == 0)? 1 : 0;
  }

  printf("Confusion matrix:\n");
  printf("\n");
  printf("                  | Predicted death | Predicted survival |\n");
  printf("------------------|-----------------|--------------------|\n");
  printf("Actually died     | %15d | %18d |\n", TN, FP);
  printf("Actually survived | %15d | %18d |\n", FN, TP);
  printf("----------------------------------------------------------\n");
  printf("total_entries=%d, correctly_predicated=%d, accuracy=%f \n",
		  titanic_data_size, TP + TN, (titanic_data_size - (FP + FN)) / (float)titanic_data_size);

  return 0;
}
