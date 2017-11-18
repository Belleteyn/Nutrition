#include <iostream>

#include <list>
#include <map>
#include <math.h>

#include <FoodAvailable.h>
#include <NutritionError.h>

#include <Tree.h>

//TODO: add weights to nodes (max weight to node with preferred daily portion)
//TODO: depth search with weights

using GIPair = std::pair<int16_t, FoodAvailable>;
static std::multimap<int16_t, FoodAvailable> giMap = {
  /* proteins */
  GIPair(0, FoodAvailable(Food("Very Serious Cow (fat-free curd)", 18, 3.3, 0, 85), 440, 110, FoodAvailable::Daily(220, 110, 110))),
  GIPair(0, FoodAvailable(Food("Danone (fat-free smooth curd)", 10.9, 4, 0.1, 61), 0, 85, FoodAvailable::Daily(170, 85, 85))),
  GIPair(0, FoodAvailable(Food("House in village (fat-free smooth curd)", 10, 3.5, 0.1, 55), 800, 100, FoodAvailable::Daily(200, 100, 100))),
  GIPair(35, FoodAvailable(Food("QuestBar (mint chocolate)", 33, 8, 15, 347), 180, 60, FoodAvailable::Daily(60))),
  GIPair(70, FoodAvailable(Food("cheese pancake", 15.54, 12.3, 3.04, 141), 0, 60, FoodAvailable::Daily(184, 92))),

  GIPair(0, FoodAvailable(Food("chicken", 23.6, 0.4, 1.9, 113), 800, 0, FoodAvailable::Daily(300))),
  GIPair(0, FoodAvailable(Food("red salmon", 19, 0, 10, 135), 160, 0, FoodAvailable::Daily(200))),

  /* carbs */
  GIPair(30, FoodAvailable(Food("apple", 0.4, 9.8, 0.4, 47), 120, 120, FoodAvailable::Daily(300, 150))),
  GIPair(60, FoodAvailable(Food("banana", 1.5, 21.8, 0.2, 95), 120 * 4, 120, FoodAvailable::Daily(120 * 8, 120))),
  GIPair(100, FoodAvailable(Food("sweets", 2.5, 63.8, 14.2, 375), 0, 10, FoodAvailable::Daily(30))),
  GIPair(55, FoodAvailable(Food("oatmeal mistral", 13.8, 69.8, 5.9, 355), 40, 40, FoodAvailable::Daily(80, 40, 40))),
  GIPair(22, FoodAvailable(Food("pearl barley", 10, 75, 1, 350), 240, 40, FoodAvailable::Daily(80, 40))),
  GIPair(35, FoodAvailable(Food("QuestBar Hero (blueberry cobbler)", 28.3, 50, 11.7, 283), 180, 60, FoodAvailable::Daily(60))),
  GIPair(35, FoodAvailable(Food("QuestBar (strawberry)", 33.3, 41.7, 8.3, 300), 180, 60, FoodAvailable::Daily(60))),

  GIPair(60, FoodAvailable(Food("buckweed", 12.6, 62.1, 3.3, 313), 160, 0, FoodAvailable::Daily(160))),
  GIPair(90, FoodAvailable(Food("honey", 0.8, 81.5, 0, 329), 0, 0, FoodAvailable::Daily(30))),
  GIPair(0, FoodAvailable(Food("ricotta caramel", 4.9, 17.5, 9, 170), 100, 0, FoodAvailable::Daily(50))),
  GIPair(25, FoodAvailable(Food("milk 2,5%", 2.9, 4.8, 2.5, 53), 500, 0, FoodAvailable::Daily(40, 40, 20))),
  GIPair(0, FoodAvailable(Food("kefir 1%", 2.8, 4, 1, 36), 0, 250, FoodAvailable::Daily(500))),

  /* fats */
  GIPair(10, FoodAvailable(Food("avocado", 2, 6, 20, 212), 35, 10, FoodAvailable::Daily(300, 150))),
  GIPair(0, FoodAvailable(Food("linseed oil", 0, 0, 99.8, 898), 200, 5, FoodAvailable::Daily(10, 10, 5))),

  GIPair(15, FoodAvailable(Food("kashew paste", 10, 9, 51, 590), 48, 0, FoodAvailable::Daily(50)))
};

int main()
{
  const Nutrition idealNutrition(1300, 1300 * 0.5, 1300 * 0.3, 1300 * 0.2);

  const Nutrition allowedNutritionOverheading(0.5, 0.5, 0.3, 0.1);
  float allowedError = 1;

  uint64_t N = 1;

  FoodTree tree;

  auto overheadingComparator = [idealNutrition, &allowedNutritionOverheading](const Nutrition& nutrition) -> bool
  {
    auto overheading = NutritionError::overheading(idealNutrition, nutrition);

    return overheading.carbs < allowedNutritionOverheading.carbs
        && overheading.proteins < allowedNutritionOverheading.proteins
        && overheading.fats < allowedNutritionOverheading.fats
        && overheading.kkal < allowedNutritionOverheading.kkal;
  };

  //TODO: if empty, N = 0
  for (auto iter = giMap.begin(); iter != giMap.end(); ++iter)
  {
    auto foodAvailable = iter->second;
    std::cout << "available " << foodAvailable.maxWeightAvailable << " of " << foodAvailable.food.getName() << std::endl;

    auto sub = tree.createSubTree(foodAvailable, overheadingComparator);
    if (sub.size() > 0)
    {
      N *= sub.size();
      tree.addLeaves(sub, overheadingComparator);
      //tree.print();
    }
  }

  std::cout << "N = " << N << std::endl;

  FoodTree::Ration* minErrorRation = nullptr;
  Nutrition minErrorNutrition(0, 0, 0, 0);
  NutritionError minError(idealNutrition, minErrorNutrition);

  auto allowedErrorComparator = [idealNutrition, &allowedError](const Nutrition& nutrition) -> bool
  {
    NutritionError error(idealNutrition, nutrition);
    auto err = error.error();
    if (err < allowedError)
    {
      allowedError = err;
      return true;
    }

    return false;
  };

  auto rationList = tree.depthSearch(allowedErrorComparator, overheadingComparator);
  std::cout << "ration variants: " << rationList.size() << std::endl;

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

//    std::cout << "ration: " << ration->size() << std::endl;
//    for (auto iter = ration->begin(); iter != ration->end(); ++iter)
//    {
//      std::cout << (*iter)->getName() << " -> " << (*iter)->getPortionMass() << std::endl;
//    }

//    std::cout << "\nSummary: Error = " << error.error() * 100 << std::endl;

//    std::cout << "kkal: " << sum.kkal << "(" << idealNutrition.kkal << ")"
//              << ", kkal error = " << error.kkalErr;

//    std::cout << "\np: " << sum.proteins << "(" << idealNutrition.proteins << ")"
//              << ", error = " << error.proteinsErr;

//    std::cout << "\nc: " << sum.carbohydrates << "(" << idealNutrition.carbohydrates << ")"
//              << ", error = " << error.carbohydratesErr;

//    std::cout << "\nf: " << sum.fats << "(" << idealNutrition.fats << ")"
//              << ", error = " << error.fatsErr << std::endl << std::endl;
  }

  if (!minErrorRation)
    return 0;

  std::cout << "\nmin error ration: " << minErrorRation->size() << " foods \n\n";
  for (auto iter = minErrorRation->begin(); iter != minErrorRation->end(); ++iter)
  {
    std::cout << (*iter)->getName() << " -> " << (*iter)->getPortionMass() << std::endl;
  }

  std::cout << "\nSummary: Error = " << minError.error() * 100 << std::endl;

  std::cout << "kkal: " << minErrorNutrition.kkal << "(" << idealNutrition.kkal << ")"
            << ", kkal error = " << minError.kkalErr;

  std::cout << "\np: " << minErrorNutrition.proteins << "(" << idealNutrition.proteins << ")"
            << ", error = " << minError.proteinsErr;

  std::cout << "\nc: " << minErrorNutrition.carbs << "(" << idealNutrition.carbs << ")"
            << ", error = " << minError.carbsErr;

  std::cout << "\nf: " << minErrorNutrition.fats << "(" << idealNutrition.fats << ")"
            << ", error = " << minError.fatsErr << std::endl;

  return 0;
}
