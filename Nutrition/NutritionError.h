#ifndef NUTRITIONERROR_H
#define NUTRITIONERROR_H

#include <Nutrition.h>

struct NutritionError
{
  float proteinsErr = 0;
  float carbohydratesErr = 0;
  float fatsErr = 0;
  float kkalErr = 0;

  NutritionError() {}

  NutritionError(const Nutrition& ideal, const Nutrition& actual)
  {
    kkalErr = 1 - actual.kkal / ideal.kkal;
    proteinsErr = 1 - actual.proteins / ideal.proteins;
    carbohydratesErr = 1 - actual.carbohydrates / ideal.carbohydrates;
    fatsErr = 1 - actual.fats / ideal.fats;
  }

  float error() const
  {
    return std::max(kkalErr, std::max(proteinsErr, std::max(carbohydratesErr, fatsErr)));
  }

  bool operator == (const NutritionError& rhs) const
  {
    return (kkalErr == rhs.kkalErr
            && proteinsErr == rhs.proteinsErr
            && carbohydratesErr == rhs.carbohydratesErr
            && fatsErr == rhs.fatsErr);
  }

  bool operator != (const NutritionError& rhs) const
  {
    return (kkalErr != rhs.kkalErr
           || proteinsErr != rhs.proteinsErr
           || carbohydratesErr != rhs.carbohydratesErr
           || fatsErr != rhs.fatsErr);
  }
};

#endif // NUTRITIONERROR_H
