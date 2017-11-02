#ifndef FOOD_H
#define FOOD_H

#include <stdint.h>

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
  float getPortionKkal() const;
  float getPortionProteins() const;
  float getPortionCarbohydrates() const;
  float getPortionFats() const;

private:
  const char* name = "";

  /* per 100g */
  const float proteins = 0;
  const float carbohydrates = 0;
  const float fats = 0;
  const uint16_t kkal = 0;

  /* portion */
  uint16_t portionMass = 0;
  float portionKkal = 0;
  float portionProteins = 0;
  float portionCarbohydrates = 0;
  float portionFats = 0;
};

#endif // FOOD_H
