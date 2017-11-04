#include "Genetic.h"

#include <list>

NutritionError Algorithms::Genetic::apply(FoodContainer& foods)
{
  std::list<FoodContainer> genotypes;

  for (auto iter = foods.begin(); iter != foods.end(); ++iter)
  {
    FoodContainer genotype(foods);
    auto mutation = genotype.find(iter->first);

    // + mutation
    // - mutation

    //if error < error0, add to container
  }
}
