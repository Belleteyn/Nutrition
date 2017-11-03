#ifndef NUTRIENT_H
#define NUTRIENT_H

#include <iostream>

enum class Nutrient { Proteins, Carbohydrates, Fats };
std::ostream& operator << (std::ostream& os, const Nutrient& obj)
{
  switch (obj)
  {
  case Nutrient::Proteins:
    os << "Proteins";
  break;

  case Nutrient::Carbohydrates:
    os << "Carbohydrates";
  break;

  case Nutrient::Fats:
    os << "Fats";
  break;

  default:
  break;
  }

  return os;
}

#endif // NUTRIENT_H
