// Copyright PWr in Space 2023, Krzysztof Gliwiński
#pragma once
#include <stdbool.h>

#include "esp_log.h"

#define ABTAG "Alpha-Beta filter"

typedef struct {
  float alpha;
  float beta;
  float prevTime;
  float xk_1;
  float xk;
  float vk_1;
  float vk;
} alphaBetaValues;

/*!
    \brief Initiate the alpha-beta values
    \param alpha - desired balue for alpha
    \param beta - desired value for beta
    \returns true - if the alpha beta values are in bounds,
             false otherwise
*/
bool alphaBetaInit(alphaBetaValues* abv, float alpha, float beta);

/*!
    \brief Alpha-beta filter. Counts the derivative by itself.
    \param curVal - current value
    \param curTime - current times
    \returns Value after filtering
*/
float alphaBetaFilter(alphaBetaValues* abv, float curVal, float curTime);
