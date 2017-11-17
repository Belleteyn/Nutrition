#include "Tree.h"

#include <iostream>
#include <ctime>


//void printSub(const FoodTree::FoodNode& node)
//{
//  std::cout << node.getBody().getName() << " (" << node.getBody().getPortionMass() << ") sub:\n";
//  auto sub = node.getSub();

//  for (auto subIter = sub.begin(); subIter != sub.end(); ++subIter)
//  {
//    std::cout << (*subIter)->getBody().getName() << " " << (*subIter)->getBody().getPortionMass() << std::endl;
//  }
//}


//void depthSearch(FoodTree::FoodNode* node
//                 , FoodTree::Ration& ration
//                 , Nutrition& rationNutrition
//                 , FoodTree::RationList& rationList
//                 , const FoodTree::NutritionErrorComparator& comparator
//                 , const FoodTree::NutritionErrorComparator& overheadingComparator)
//{
//  auto& body = node->getBody();
//  auto& sub = node->getSub();

//  if (body.getPortionMass() > 0)
//  {
//    rationNutrition += body.getPortionNutrition();
//    ration.emplace_back(&body);
//  }

//  if (overheadingComparator(rationNutrition))
//  {
//    if (!sub.empty())
//    {
//      for (auto iter = sub.begin(); iter != sub.end(); ++iter)
//      {
//        //depthSearch(*iter, ration, rationNutrition, rationList, comparator, overheadingComparator);
//      }
//    }
//    else
//    {
//      if (!ration.empty() && comparator(rationNutrition))
//      {
//        rationList.emplace_back(ration);
//      }
//    }
//  }

//  if (body.getPortionMass() > 0)
//  {
//    rationNutrition -= body.getPortionNutrition();
//    ration.pop_back();
//  }
//}

FoodTree::FoodTree()
  : root_()
  , leaves_()
{}

FoodTree::~FoodTree()
{}

void FoodTree::addSubTree(const SubTree& sub, const NutritionErrorComparator& overheadingComparator)
{
  if (leaves_.empty())
  {
    root_.setSub(sub);
  }
  else
  {
    for (auto iter = leaves_.begin(); iter != leaves_.end(); ++iter)
    {
      bool setFull = true;
      for (auto subIter = sub.begin(); subIter != sub.end(); ++subIter)
      {
        auto checkNutrition = (*iter).getBody()->getPortionNutrition() + (*subIter).getBody()->getPortionNutrition();
        if (!overheadingComparator(checkNutrition))
        {
          std::cout << "compare "<< (*iter).getBody()->getName() << " (" << (*iter).getBody()->getPortionMass() << ") "
                    << " + " << (*subIter).getBody()->getName() << " (" << (*subIter).getBody()->getPortionMass() << ")" << std::endl;

          std::cout << "overhead: " << checkNutrition.kkal << " | " << checkNutrition.proteins << "/"
                    << checkNutrition.carbohydrates << "/" << checkNutrition.fats << std::endl;

          SubTree leavesCopy(sub.begin(), subIter);
          (*iter).setSub(leavesCopy);

          setFull = false;
          break;
        }
      }

      if (setFull) {
        (*iter).setSub(sub);
      }
    }
  }

  leaves_.clear();
  leaves_ = sub;
}

FoodTree::RationList FoodTree::depthSearch(const NutritionErrorComparator& allowedErrorComparator
                                           , const NutritionErrorComparator& overheadingComparator)
{
  FoodTree::Ration list;
  Nutrition rationNutrition(0, 0, 0, 0);

  FoodTree::RationList rationList;

  clock_t begin = clock();
//  ::depthSearch(root_, list, rationNutrition, rationList, allowedErrorComparator, overheadingComparator);
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

  std::cout << "\n ~ depth search time " << elapsed_secs << std::endl << std::endl;

  return rationList;
}
