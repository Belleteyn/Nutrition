#include <iostream>

#include <list>
#include <math.h>

#include <FoodContainer.h>
#include <NutritionError.h>

#include <Tree.h>

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

using SubTree = std::list<FoodTree::FoodNode*>;
SubTree createSubTree(const FoodAvailable& avFood)
{
  SubTree subTree;
  uint16_t portion = 0;

  Food food(avFood.food);

  while (portion <= avFood.portionPreferred) {
    if (portion > avFood.portionPreferred) {
      portion = avFood.portionPreferred;
    }

    food.setPortion(portion);

    FoodTree::FoodNode* node = new FoodTree::FoodNode(food);
    subTree.push_back(node);

    portion += avFood.deltaPortion;
  }

  return subTree;
}

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

  const float allowedError = 0.685;

  uint64_t N = 1, n = 1;

  FoodTree tree;

  //TODO: if empty, N = 0
  for (auto iter = giMap.begin(); iter != giMap.end(); ++iter)
  {
    auto foodAvailable = iter->second;
    std::cout << "available " << foodAvailable.maxWeightAvailable << " of " << foodAvailable.food.getName();
    auto portions = ceil(static_cast<float>(foodAvailable.maxWeightAvailable) / foodAvailable.deltaPortion);
    std::cout << " / max portions " << portions;

    auto preferredPortions = ceil(static_cast<float>(foodAvailable.portionPreferred) / foodAvailable.deltaPortion);
    std::cout << " / preferredPortions " << preferredPortions << std::endl;

    N *= portions;
    n *= preferredPortions;

    tree.addLeaves(createSubTree(foodAvailable));
  }

  std::cout << "N = " << N << std::endl;
  std::cout << "n = " << n << std::endl;

  FoodTree::Ration* minErrorRation = nullptr;
  Nutrition minErrorNutrition(0, 0, 0, 0);
  NutritionError minError(idealNutrition, minErrorNutrition);

  auto rationList = tree.depthSearch(idealNutrition, 0.7);
  for (auto ration = rationList.begin(); ration != rationList.end(); ++ration)
  {
    Nutrition sum(0, 0, 0, 0);
    for (auto iter = ration->begin(); iter != ration->end(); ++iter)
    {
      sum += (*iter)->getPortionNutrition();
    }

    NutritionError error(idealNutrition, sum);
    if (error.error() < minError.error())
    {
      minError = error;
      minErrorNutrition = sum;
      minErrorRation = &(*ration);
    }

//    if (error.error() <= allowedError)
//    {
//      std::cout << "ration: " << ration->size() << std::endl;
//      for (auto iter = ration->begin(); iter != ration->end(); ++iter)
//      {
//        std::cout << (*iter)->getName() << " -> " << (*iter)->getPortionMass() << std::endl;
//      }

//      std::cout << "\nSummary: Error = " << error.error() * 100 << std::endl;

//      std::cout << "kkal: " << sum.kkal << "(" << idealNutrition.kkal << ")"
//                << ", kkal error = " << error.kkalErr;

//      std::cout << "\np: " << sum.proteins << "(" << idealNutrition.proteins << ")"
//                << ", error = " << error.proteinsErr;

//      std::cout << "\nc: " << sum.carbohydrates << "(" << idealNutrition.carbohydrates << ")"
//                << ", error = " << error.carbohydratesErr;

//      std::cout << "\nf: " << sum.fats << "(" << idealNutrition.fats << ")"
//                << ", error = " << error.fatsErr << std::endl << std::endl;
//    }
  }

  if (!minErrorRation)
    return 0;

  std::cout << "min error ration: " << minErrorRation->size() << std::endl;
  for (auto iter = minErrorRation->begin(); iter != minErrorRation->end(); ++iter)
  {
    std::cout << (*iter)->getName() << " -> " << (*iter)->getPortionMass() << std::endl;
  }

  std::cout << "\nSummary: Error = " << minError.error() * 100 << std::endl;

  std::cout << "kkal: " << minErrorNutrition.kkal << "(" << idealNutrition.kkal << ")"
            << ", kkal error = " << minError.kkalErr;

  std::cout << "\np: " << minErrorNutrition.proteins << "(" << idealNutrition.proteins << ")"
            << ", error = " << minError.proteinsErr;

  std::cout << "\nc: " << minErrorNutrition.carbohydrates << "(" << idealNutrition.carbohydrates << ")"
            << ", error = " << minError.carbohydratesErr;

  std::cout << "\nf: " << minErrorNutrition.fats << "(" << idealNutrition.fats << ")"
            << ", error = " << minError.fatsErr << std::endl;

  return 0;
}
