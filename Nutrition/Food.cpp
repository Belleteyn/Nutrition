#include "Food.h"

#include <iostream>
#include <assert.h>

Food::Food(const char* name, float p, float c, float f, uint16_t k)
  : name(name)
  , proteins(p)
  , carbohydrates(c)
  , fats(f)
  , kkal(k)
  , portionMass(0)
  , portionProteins(0)
  , portionCarbohydrates(0)
  , portionFats(0)
{

}

void Food::setPortion(const int gram)
{
  portionMass = gram;

  portionKkal = kkal * 0.01 * portionMass;
  portionProteins = proteins * 0.01 * kkalPerProteinGram * portionMass;
  portionCarbohydrates = carbohydrates * 0.01 * kkalPerCarbohydrateGram * portionMass;
  portionFats = fats * 0.01 * kkalPerFatGram * portionMass;

  /*
  std::cout << "portion " << portionMass
            << ": kkal " << portionKkal
            << ", p " << portionProteins
            << ", c " << portionCarbohydrates
            << ", f " << portionFats
            << std::endl;
  */
  assert(portionKkal != (portionProteins + portionCarbohydrates + portionFats) && "portion kkal error");
}

bool Food::operator <(const Food& rhs) const
{
  return name < rhs.name;
}

const char* Food::getName() const
{
  return name;
}

float Food::getPortionKkal() const
{
  return portionKkal;
}

float Food::getPortionProteins() const
{
  return portionProteins;
}

float Food::getPortionCarbohydrates() const
{
  return portionCarbohydrates;
}

float Food::getPortionFats() const
{
  return portionFats;
}

uint16_t Food::getPortionMass() const
{
  return portionMass;
}
