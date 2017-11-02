#ifndef FOOD_H
#define FOOD_H

#include <stdint.h>
#include <Nutrition.h>

const float kkalPerFatGram = 9.29;
const float kkalPerProteinGram = 4.11;
const float kkalPerCarbohydrateGram = 4.11;

class Food
{
public:
  Food(const char* name, float p = 0, float c = 0, float f = 0, uint16_t k = 0);
  ~Food() = default;

  void setPortion(const int gram);

  bool operator < (const Food& rhs) const;


  const char* getName() const;
  uint16_t getPortionMass() const;

  const Nutrition& getNutrition() const;
  const Nutrition& getPortionNutrition() const;

private:
  const char* name = "";

  /* per 100g */
  const Nutrition nutrition_;

  /* portion */
  uint16_t portionMass = 0;
  Nutrition portionNutrition_;
};

#endif // FOOD_H
