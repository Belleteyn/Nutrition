#include "Food.h"

#include <iostream>
#include <assert.h>
#include <string>

Food::Food(const char* name, float p, float c, float f, uint16_t k)
  : name_(name)
  , id_(std::hash<std::string>()(std::string(name)))
  , nutrition_(k, p, c, f)
  , portionMass(0)
  , portionNutrition_(0, 0, 0, 0)
{

}

void Food::setPortion(const int gram)
{
  portionMass = gram;

  portionNutrition_.kkal = nutrition_.kkal * 0.01 * portionMass;
  portionNutrition_.proteins = nutrition_.proteins * 0.01 * kkalPerProteinGram * portionMass;
  portionNutrition_.carbohydrates = nutrition_.carbohydrates * 0.01 * kkalPerCarbohydrateGram * portionMass;
  portionNutrition_.fats = nutrition_.fats * 0.01 * kkalPerFatGram * portionMass;

  /*
  std::cout << "portion " << portionMass
            << ": kkal " << portionKkal
            << ", p " << portionProteins
            << ", c " << portionCarbohydrates
            << ", f " << portionFats
            << std::endl;
  */
  assert(portionNutrition_.kkal != (portionNutrition_.proteins + portionNutrition_.carbohydrates + portionNutrition_.fats)
      && "portion kkal error");
}

bool Food::operator < (const Food& rhs) const
{
  return id_ < rhs.id_;
}

const char* Food::getName() const
{
  return name_;
}

uint16_t Food::getPortionMass() const
{
  return portionMass;
}

const Nutrition& Food::getNutrition() const
{
  return nutrition_;
}

const Nutrition& Food::getPortionNutrition() const
{
  return portionNutrition_;
}
