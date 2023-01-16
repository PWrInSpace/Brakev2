// Copyright 2023 PWr in Space, Krzysztof Gliwiński
#include "alpha_beta_filter.h"

bool alphaBetaInit(alphaBetaValues* abv, float alpha, float beta) {
  if (alpha <= 0.f || alpha >= 1.f || beta <= 0 || beta > 2 ||
      (4.f - 2.f * alpha - beta) <= 0) {
    ESP_LOGE(ABTAG,
             "Filter values are not correct!\t 0 < alpha < 1 \t 0 < beta <= 2 "
             "\t 0 < 4 - 2*alpha - beta\n actual values: alpha = %f, beta = "
             "%f, 4 - 2*alpha - beta = %f",
             alpha, beta, (4.f - 2.f * alpha - beta));
    return false;
  }
  abv->alpha = alpha;
  abv->beta = beta;
  abv->prevTime = 0.0f;
  abv->xk_1 = 0.0f;
  abv->xk = 0.0f;
  abv->vk_1 = 0.0f;
  abv->vk = 0.0f;
  return true;
}

float alphaBetaFilter(alphaBetaValues* abv, float curVal, float curTime) {
  float dt = curTime - abv->prevTime;
  abv->xk = abv->xk_1 + (abv->vk_1 * dt);
  abv->vk = abv->vk_1;

  float rk = curVal - abv->xk;

  abv->xk += abv->alpha * rk;
  abv->vk += (abv->beta * rk) / dt;

  abv->xk_1 = abv->xk;
  abv->vk_1 = abv->vk;

  abv->prevTime = curTime;
  return abv->xk;
}
