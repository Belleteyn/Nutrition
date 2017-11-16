#include "Tree.h"

#include <iostream>
#include <ctime>

void depthSearch(FoodTree::FoodNode* node
                 , FoodTree::Ration& ration
                 , Nutrition& rationNutrition
                 , FoodTree::RationList& rationList
                 , const FoodTree::NutritionErrorComparator& comparator)
{
  auto& body = node->getBody();
  auto& sub = node->getSub();

  if (body.getPortionMass() > 0)
  {
    rationNutrition += body.getPortionNutrition();
    ration.emplace_back(&body);
  }

  if (!sub.empty())
  {
    for (auto iter = sub.begin(); iter != sub.end(); ++iter)
    {
      depthSearch(*iter, ration, rationNutrition, rationList, comparator);
    }
  }
  else
  {
    if (!ration.empty() && comparator(rationNutrition))
    {
      rationList.emplace_back(ration);
    }
  }

  if (body.getPortionMass() > 0)
  {
    rationNutrition -= body.getPortionNutrition();
    ration.pop_back();
  }
}

FoodTree::FoodTree()
  : root_(new FoodNode(Food("")))
  , leaves_()
{
  leaves_.push_back(root_);
}

FoodTree::~FoodTree()
{
  delete root_;
}

void FoodTree::addLeaves(const std::list<FoodNode*>& leaves)
{
  for (auto iter = leaves_.begin(); iter != leaves_.end(); ++iter)
  {
    (*iter)->setSub(leaves);
  }

  leaves_ = leaves;
}

void FoodTree::print() const
{
  std::cout << "\ntree leaves: \n";

  for (auto iter = leaves_.begin(); iter != leaves_.end(); ++iter)
  {
    std::cout << (*iter)->getBody().getName() << " " <<(*iter)->getBody().getPortionMass() << "/ \n";
  }
}

FoodTree::RationList FoodTree::depthSearch(const NutritionErrorComparator& allowedErrorComparator)
{
  FoodTree::Ration list;
  Nutrition rationNutrition(0, 0, 0, 0);

  FoodTree::RationList rationList;

  clock_t begin = clock();
  ::depthSearch(root_, list, rationNutrition, rationList, allowedErrorComparator);
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

  std::cout << "\n ~ depth search time " << elapsed_secs << std::endl << std::endl;

  return rationList;
}
