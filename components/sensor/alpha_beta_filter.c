#include "alpha_beta_filter.h"

bool alphaBetaInit(alphaBetaValues* abv, float alpha, float beta)
{
    abv->alpha = alpha;
    abv->beta = beta;
    abv->prevTime = 0.0f;
    abv->prevVal = 0.0f;
}