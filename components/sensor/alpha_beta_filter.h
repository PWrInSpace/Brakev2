#pragma once

#include <stdbool.h>

typedef struct {
  float alpha;
  float beta;
  float prevVal;
  float prevTime;
} alphaBetaValues;

/*!
    \brief Initiate the alpha-beta values
    \param alpha - desired balue for alpha
    \param beta - desired value for beta
*/
bool alphaBetaInit(alphaBetaValues* abv, float alpha, float beta);

/*!
    \brief Alpha-beta filter. Counts the derivative by itself.
    \param curVal - current value
    \returns Value after filtering
*/
float alphaBetaFilter(float curVal);