#ifndef NUTRITIONERROR_H
#define NUTRITIONERROR_H

#include <assert.h>

#include <Nutrition.h>
#include <Nutrient.h>

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

    assert(kkalErr >= -1);
    assert(proteinsErr >= -1);
    assert(carbohydratesErr >= -1);
    assert(fatsErr >= -1);

    assert(kkalErr <= 1);
    assert(proteinsErr <= 1);
    assert(carbohydratesErr <= 1);
    assert(fatsErr <= 1);
  }

  float error() const
  {
    return std::max(abs(kkalErr), std::max(abs(proteinsErr), std::max(abs(carbohydratesErr), abs(fatsErr))));
  }

  Nutrient maxErrorNutrient() const
  {
    auto max = std::max(proteinsErr, std::max(carbohydratesErr, fatsErr));
    Nutrient meNutrient = (fatsErr == max) ? Nutrient::Fats : ((carbohydratesErr == max) ? Nutrient::Carbohydrates : Nutrient::Proteins);
    return meNutrient;
  }

  Nutrient minErrorNutrient() const
  {
    auto min = std::min(proteinsErr, std::min(carbohydratesErr, fatsErr));
    Nutrient meNutrient = (proteinsErr == min) ? Nutrient::Proteins : ((carbohydratesErr == min) ? Nutrient::Carbohydrates : Nutrient::Fats);
    return meNutrient;
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
