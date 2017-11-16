#include <iostream>

#include <list>
#include <math.h>

#include <FoodContainer.h>
#include <NutritionError.h>

#include <Tree.h>

//TODO: add weights to nodes (max weight to node with preferred daily portion)
//TODO: depth search with weights
//TODO: remove branches with overheading

//TODO: dynamically change allowed error (based on available food)

struct FoodAvailable
{
  struct Daily
  {
    uint16_t minDailyPortion = 0;
    uint16_t preferredDailyPortion = 0;
    uint16_t maxDailyPortion = 0;

    Daily(uint16_t max = 0, uint16_t preferred = 0, uint16_t min = 0)
      : minDailyPortion(min)
      , preferredDailyPortion(preferred)
      , maxDailyPortion(max)
    {}
  };

  Food food = Food("");

  uint16_t maxWeightAvailable = 0;
  uint16_t deltaPortion = 10;
  Daily daily;

  FoodAvailable(const Food& food, uint16_t maxWeight = 0, uint16_t delta = 0, const Daily& daily = Daily())
    : food(food)
    , maxWeightAvailable(maxWeight)
    , deltaPortion(delta)
    , daily(daily)
  {}
};

using SubTree = std::list<FoodTree::FoodNode*>;

bool addNode(Food& food, uint16_t& portion, const FoodAvailable& avFood, const FoodAvailable::Daily& daily
             , const Nutrition& ideal, const float allowedError)
{
  if (portion > daily.maxDailyPortion) {
    portion = daily.maxDailyPortion;
  }

  if (portion > avFood.maxWeightAvailable) {
    portion = avFood.maxWeightAvailable;
  }

  food.setPortion(portion);
  auto overheading = NutritionError::maxOverheading(ideal, food.getPortionNutrition());
  if (overheading > allowedError)
  {
    std::cout << "overheading: " << overheading << " for " << food.getName() << ", portion " << food.getPortionMass()
              << " / " << food.getNutrient(NutritionError(ideal, food.getPortionNutrition()).maxErrorNutrient())
              << std::endl;
    return false;
  }

  return true;
}

SubTree createSubTree(const FoodAvailable& avFood, const Nutrition& ideal, const float allowedError)
{
  SubTree subTree;
  const auto& daily = avFood.daily;

  if (avFood.maxWeightAvailable == 0 || daily.maxDailyPortion == 0)
    return subTree;

  uint16_t portion = daily.minDailyPortion;
  Food food(avFood.food);

  if (!addNode(food, portion, avFood, daily, ideal, allowedError))
  {
    return subTree;
  }

  FoodTree::FoodNode* node = new FoodTree::FoodNode(food);
  subTree.push_back(node);

  while (portion < daily.maxDailyPortion && portion < avFood.maxWeightAvailable) {
    portion += avFood.deltaPortion;

    if (!addNode(food, portion, avFood, daily, ideal, allowedError))
    {
      return subTree;
    }

    FoodTree::FoodNode* node = new FoodTree::FoodNode(food);
    subTree.push_back(node);
  }

  return subTree;
}

using GIPair = std::pair<int16_t, FoodAvailable>;
static std::multimap<int16_t, FoodAvailable> giMap = {
  GIPair(10, FoodAvailable(Food("avocado", 2, 6, 20, 212), 35, 10, FoodAvailable::Daily(300, 150))),
  GIPair(30, FoodAvailable(Food("apple", 0.4, 9.8, 0.4, 47), 450, 80, FoodAvailable::Daily(300, 150))),
  GIPair(60, FoodAvailable(Food("buckweed", 12.6, 62.1, 3.3, 313), 450, 10, FoodAvailable::Daily(160))),
  GIPair(60, FoodAvailable(Food("banana", 1.5, 21.8, 0.2, 95), 120 * 8, 60, FoodAvailable::Daily(120 * 8, 120))),
  GIPair(90, FoodAvailable(Food("honey", 0.8, 81.5, 0, 329), 350, 5, FoodAvailable::Daily(30, 15))),
  GIPair(0, FoodAvailable(Food("Very Serious Cow (fat-free curd)", 18, 3.3, 0, 85), 220, 110, FoodAvailable::Daily(220, 110, 110))),
  GIPair(0, FoodAvailable(Food("Danone (fat-free smooth curd)", 10.9, 4, 0.1, 61), 340, 85, FoodAvailable::Daily(170, 85, 85))),
  GIPair(0, FoodAvailable(Food("chicken", 23.6, 0.4, 1.9, 113), 400, 10, FoodAvailable::Daily(300, 200))),
  GIPair(100, FoodAvailable(Food("sweets", 2.5, 63.8, 14.2, 375), 200, 10, FoodAvailable::Daily(30))),
  GIPair(15, FoodAvailable(Food("kashew paste", 10, 9, 51, 590), 320, 5, FoodAvailable::Daily(50, 20))),
  GIPair(0, FoodAvailable(Food("linseed oil", 0, 0, 99.8, 898), 200, 5, FoodAvailable::Daily(10, 10, 5))),
  GIPair(55, FoodAvailable(Food("oatmeal mistral", 13.8, 69.8, 5.9, 355), 400, 10, FoodAvailable::Daily(80, 40, 30))),
  GIPair(70, FoodAvailable(Food("cheese pancake", 15.54, 12.3, 3.04, 141), 92, 60, FoodAvailable::Daily(184, 92))),
  GIPair(0, FoodAvailable(Food("ricotta caramel", 4.9, 17.5, 9, 170), 150, 10, FoodAvailable::Daily(50, 30))),
  GIPair(25, FoodAvailable(Food("milk 2,5%", 2.9, 4.8, 2.5, 53), 950, 10, FoodAvailable::Daily(40, 40, 20))),
  GIPair(22, FoodAvailable(Food("pearl barley", 10, 75, 1, 350), 240, 10, FoodAvailable::Daily(80, 40))),
  GIPair(0, FoodAvailable(Food("red salmon", 19, 0, 10, 135), 200, 10, FoodAvailable::Daily(200, 100))),
  GIPair(0, FoodAvailable(Food("kefir 1%", 2.8, 4, 1, 36), 900, 10, FoodAvailable::Daily(440, 220))),
  GIPair(35, FoodAvailable(Food("QuestBar Hero (blueberry cobbler)", 28.3, 50, 11.7, 283), 180, 60, FoodAvailable::Daily(60))),
  GIPair(35, FoodAvailable(Food("QuestBar (strawberry)", 33.3, 41.7, 8.3, 300), 180, 60, FoodAvailable::Daily(60))),
  GIPair(35, FoodAvailable(Food("QuestBar (mint chocolate)", 33, 8, 15, 347), 180, 60, FoodAvailable::Daily(60)))
};

int main()
{
  const Nutrition idealNutrition(1300, 1300 * 0.5, 1300 * 0.3, 1300 * 0.2);

  const float allowedError = 0.5;

  uint64_t N = 1;

  FoodTree tree;

  auto overheadingComparator = [idealNutrition, allowedError](const Nutrition& nutrition) -> bool
  {
    return NutritionError::maxOverheading(idealNutrition, nutrition) < allowedError;
  };

  //TODO: if empty, N = 0
  for (auto iter = giMap.begin(); iter != giMap.end(); ++iter)
  {
    auto foodAvailable = iter->second;
    std::cout << "available " << foodAvailable.maxWeightAvailable << " of " << foodAvailable.food.getName() << std::endl;

    auto sub = createSubTree(foodAvailable, idealNutrition, allowedError);
    if (sub.size() > 0)
    {
      N *= sub.size();
      tree.addLeaves(sub, overheadingComparator);
      tree.print();
    }
  }

  std::cout << "N = " << N << std::endl;

  FoodTree::Ration* minErrorRation = nullptr;
  Nutrition minErrorNutrition(0, 0, 0, 0);
  NutritionError minError(idealNutrition, minErrorNutrition);

  auto allowedErrorComparator = [idealNutrition, allowedError](const Nutrition& nutrition) -> bool
  {
    NutritionError error(idealNutrition, nutrition);
    return error.error() < allowedError;
  };

  auto rationList = tree.depthSearch(allowedErrorComparator);
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
