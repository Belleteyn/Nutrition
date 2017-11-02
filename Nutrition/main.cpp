#include <iostream>

#include <map>

#include <Food.h>

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
  GIPair(0, FoodAvailable(Food("chicken", 23.6, 0.4, 1.9, 113), 820, 200, 10))
};

int main()
{
  const float kMax = 1300;
  const float cMax = kMax * 0.5, pMax = kMax * 0.3, fMax = kMax * 0.2;

  const float allowedError = 0.1;
  float error = 1, prevError = 0;

  float pSumKkal = 0, cSumKkal = 0, fSumKkal = 0, kSum = 0;
  std::map<Food, uint16_t> foodMap;

  while (error > allowedError && error != prevError)
  {
    if (error != 1)
    {

    }

    for (auto iter = giMap.begin(); iter != giMap.end(); ++iter)
    {
      Food checkPortion(iter->second.food);
      checkPortion.setPortion(iter->second.deltaPortion);

      bool pcf = checkPortion.getPortionProteins() + pSumKkal <= pMax
                && checkPortion.getPortionCarbohydrates() + cSumKkal <= cMax
                && checkPortion.getPortionFats() + fSumKkal <= fMax
                && checkPortion.getPortionKkal() + kSum <= kMax;

      auto foodIter = foodMap.find(iter->second.food);
      if (foodIter == foodMap.end() && pcf)
      {
        foodMap.insert(std::pair<Food, uint16_t>(iter->second.food, checkPortion.getPortionMass()));
        foodIter = foodMap.find(iter->second.food);

        kSum += checkPortion.getPortionKkal();
        pSumKkal += checkPortion.getPortionProteins();
        cSumKkal += checkPortion.getPortionCarbohydrates();
        fSumKkal += checkPortion.getPortionFats();
      }

      //if portion is allowed
      while (checkPortion.getPortionProteins() + pSumKkal <= pMax
             && checkPortion.getPortionCarbohydrates() + cSumKkal <= cMax
             && checkPortion.getPortionFats() + fSumKkal <= fMax
             && checkPortion.getPortionKkal() + kSum <= kMax

             && foodIter->second <= iter->second.maxWeightAvailable
             && foodIter->second <= iter->second.portionPreferred)
      {
        //add portion
        foodIter->second += checkPortion.getPortionMass();

        kSum += checkPortion.getPortionKkal();
        pSumKkal += checkPortion.getPortionProteins();
        cSumKkal += checkPortion.getPortionCarbohydrates();
        fSumKkal += checkPortion.getPortionFats();
      }
    }

    prevError = error;
    auto kkalError = 1 - kSum / kMax;
    auto pError = 1 - pSumKkal / pMax;
    auto cError = 1 - cSumKkal / cMax;
    auto fError = 1 - cSumKkal / fMax;

    std::cout << "kkal error = " << kkalError
              << "\np error = " << pError
              << "\nc error = " << cError
              << "\nf error = " << fError << std::endl;

    error = std::max(kkalError, std::max(pError, std::max(cError, fError)));
  }

  for (auto iter = foodMap.begin(); iter != foodMap.end(); ++iter)
  {
    const Food& food = iter->first;
    std::cout << food.getName() << " : " << iter->second << std::endl;
  }

  std::cout << "Summary: \n" << "kkal: " << kSum << "(" << kMax << ")";
  std::cout << "\np: " << pSumKkal << "(" << pMax << ")";
  std::cout << "\nc: " << cSumKkal << "(" << cMax << ")";
  std::cout << "\nf: " << fSumKkal << "(" << fMax << ")" << std::endl;

  std::cout << "\nError: " << error * 100 << std::endl;

  return 0;
}
