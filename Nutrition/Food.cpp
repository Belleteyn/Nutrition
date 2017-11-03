#include "Food.h"

#include <iostream>
#include <assert.h>
#include <string>

Food::Food(const char* name, float p, float c, float f, uint16_t k)
  : name_(name)
  , id_(std::hash<std::string>()(std::string(name)))
  , nutrition_(k, p, c, f)
  , portionMass_(0)
  , portionNutrition_(0, 0, 0, 0)
{

}

void Food::setPortion(const int gram)
{
  portionMass_ = gram;

  portionNutrition_.kkal = nutrition_.kkal * 0.01 * portionMass_;
  portionNutrition_.proteins = nutrition_.proteins * 0.01 * kkalPerProteinGram * portionMass_;
  portionNutrition_.carbohydrates = nutrition_.carbohydrates * 0.01 * kkalPerCarbohydrateGram * portionMass_;
  portionNutrition_.fats = nutrition_.fats * 0.01 * kkalPerFatGram * portionMass_;

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
  return portionMass_;
}

const Nutrition& Food::getNutrition() const
{
  return nutrition_;
}

const Nutrition& Food::getPortionNutrition() const
{
  return portionNutrition_;
}

float Food::getNutrient(const Nutrient& nutrient) const
{
  switch (nutrient) {
  case Nutrient::Proteins:
    return nutrition_.proteins;

  case Nutrient::Carbohydrates:
    return nutrition_.carbohydrates;

  case Nutrient::Fats:
    return nutrition_.fats;

  default:
  break;
  }

  return 0;
}
