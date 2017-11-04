#ifndef NUTRITION_H
#define NUTRITION_H

#include <stdint.h>

struct Nutrition
{
  float kkal = 0;
  float proteins = 0;
  float carbohydrates = 0;
  float fats = 0;

  Nutrition(float kkal, float p, float c, float f)
    : kkal(kkal)
    , proteins(p)
    , carbohydrates(c)
    , fats(f)
  {}

  Nutrition& operator = (const Nutrition& other)
  {
    if (this == &other) {
      return *this;
    }

    kkal = other.kkal;
    proteins = other.proteins;
    carbohydrates = other.carbohydrates;
    fats = other.fats;

    return *this;
  }

  Nutrition operator + (const Nutrition& rhs) const
  {
    return Nutrition(kkal + rhs.kkal, proteins + rhs.proteins, carbohydrates + rhs.carbohydrates, fats + rhs.fats);
  }

  bool operator <= (const Nutrition& rhs) const
  {
    return (kkal <= rhs.kkal
        && proteins <= rhs.proteins
        && carbohydrates <= rhs.carbohydrates
        && fats <= rhs.fats);
  }

  Nutrition& operator += (const Nutrition& rhs)
  {
    kkal += rhs.kkal;
    proteins += rhs.proteins;
    carbohydrates += rhs.carbohydrates;
    fats += rhs.fats;
    return *this;
  }

  Nutrition& operator -= (const Nutrition& rhs)
  {
    kkal -= rhs.kkal;
    proteins -= rhs.proteins;
    carbohydrates -= rhs.carbohydrates;
    fats -= rhs.fats;
    return *this;
  }
};

#endif // NUTRITION_H
