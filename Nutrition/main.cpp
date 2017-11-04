#include <iostream>

#include <map>

#include <Food.h>
#include <NutritionError.h>

struct FoodAvailable
{
  Food food = Food("");
  uint16_t maxWeightAvailable = 0;
  uint16_t portionPreferred = 200;
  uint16_t deltaPortion = 10;

  FoodAvailable(const Food& food, uint16_t maxWeight = 0, uint16_t portion = 0, uint16_t delta = 0)
    : food(food)
    , maxWeightAvailable(maxWeight)
    , portionPreferred(portion)
    , deltaPortion(delta)
  {}
};

using GIPair = std::pair<int16_t, FoodAvailable>;
static std::multimap<int16_t, FoodAvailable> giMap = {
  GIPair(10, FoodAvailable(Food("avocado", 2, 6, 20, 212), 300, 100, 10)),
  GIPair(30, FoodAvailable(Food("apple", 0.4, 9.8, 0.4, 47), 1000, 300, 150)),
  GIPair(60, FoodAvailable(Food("buckweed", 12.6, 62.1, 3.3, 313), 450, 100, 10)),
  GIPair(60, FoodAvailable(Food("banana", 1.5, 21.8, 0.2, 95), 120 * 5, 120, 120)),
  GIPair(90, FoodAvailable(Food("honey", 0.8, 81.5, 0, 329), 350, 15, 5)),
  GIPair(0, FoodAvailable(Food("tvorog", 16.5, 1.3, 0, 71), 400, 200, 100)),
  GIPair(0, FoodAvailable(Food("chicken", 23.6, 0.4, 1.9, 113), 820, 200, 10)),
  GIPair(100, FoodAvailable(Food("sweets", 2.5, 63.8, 14.2, 375), 200, 30, 10)),
  GIPair(15, FoodAvailable(Food("kashew paste", 10, 9, 51, 590), 320, 50, 5))
};

int main()
{
  const Nutrition idealNutrition(1300, 1300 * 0.5, 1300 * 0.3, 1300 * 0.2);
  Nutrition sum(0, 0, 0, 0);

  const float allowedError = 0.1;
  NutritionError error(idealNutrition, sum), prevError;

  std::map<Food, uint16_t> foodMap;

  int cycleSteps = 0;
  while (error.error() > allowedError && error != prevError && cycleSteps < 10)
  {
    cycleSteps++;

    if (error.error() != 1)
    {

    }

    for (auto iter = giMap.begin(); iter != giMap.end(); ++iter)
    {
      Food checkPortion(iter->second.food);
      checkPortion.setPortion(iter->second.deltaPortion);

      auto foodIter = foodMap.find(iter->second.food);
      if (foodIter == foodMap.end() && (checkPortion.getPortionNutrition() + sum <= idealNutrition))
      {
        foodMap.insert(std::pair<Food, uint16_t>(iter->second.food, checkPortion.getPortionMass()));
        foodIter = foodMap.find(iter->second.food);

        sum += checkPortion.getPortionNutrition();

        std::cout << "add portion of " << checkPortion.getName() << "\n";
      }

      //if portion is allowed
      while (checkPortion.getPortionNutrition() + sum <= idealNutrition
             && foodIter->second <= iter->second.maxWeightAvailable
             && foodIter->second <= iter->second.portionPreferred)
      {
        //add portion
        foodIter->second += checkPortion.getPortionMass();
        sum += checkPortion.getPortionNutrition();

        std::cout << "add portion of " << checkPortion.getName() << "\n";
      }
    }

    prevError = error;
    error = NutritionError(idealNutrition, sum);


    for (auto iter = foodMap.begin(); iter != foodMap.end(); ++iter)
    {
      const Food& food = iter->first;
      std::cout << food.getName() << " : " << iter->second << std::endl;
    }

    std::cout << "\nSummary: \n" << "kkal: " << sum.kkal << "(" << idealNutrition.kkal << ")";
    std::cout << "\np: " << sum.proteins << "(" << idealNutrition.proteins << ")";
    std::cout << "\nc: " << sum.carbohydrates << "(" << idealNutrition.carbohydrates << ")";
    std::cout << "\nf: " << sum.fats << "(" << idealNutrition.fats << ")" << std::endl;

    std::cout << "\nError: " << error.error() * 100 << std::endl;

    std::cout << "kkal error = " << error.kkalErr
              << "\np error = " << error.proteinsErr
              << "\nc error = " << error.carbohydratesErr
              << "\nf error = " << error.fatsErr << std::endl << std::endl;
  }

  for (auto iter = foodMap.begin(); iter != foodMap.end(); ++iter)
  {
    const Food& food = iter->first;
    std::cout << food.getName() << " : " << iter->second << std::endl;
  }

  std::cout << "\nSummary: \n" << "kkal: " << sum.kkal << "(" << idealNutrition.kkal << ")";
  std::cout << "\np: " << sum.proteins << "(" << idealNutrition.proteins << ")";
  std::cout << "\nc: " << sum.carbohydrates << "(" << idealNutrition.carbohydrates << ")";
  std::cout << "\nf: " << sum.fats << "(" << idealNutrition.fats << ")" << std::endl;

  std::cout << "\nError: " << error.error() * 100 << std::endl;

  return 0;
}
