#include "Tree.h"

#include <iostream>
#include <ctime>

void depthSearch(const FoodTree::FoodNode* node
                 , FoodTree::Ration& ration, FoodTree::RationList& rationList)
{
  if (node->getBody().getPortionMass() > 0)
  {
    ration.emplace_back(&node->getBody());
  }

  if (!node->getSub().empty())
  {
    auto sub = node->getSub();
    for (auto iter = sub.begin(); iter != sub.end(); ++iter)
    {
      depthSearch(*iter, ration, rationList);
    }
  }
  else
  {
    if (!ration.empty())
    {
      rationList.emplace_back(ration);
    }
  }

  if (node->getBody().getPortionMass() > 0)
  {
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

FoodTree::RationList FoodTree::depthSearch()
{
  FoodTree::Ration list;
  FoodTree::RationList rationList;

  clock_t begin = clock();
  ::depthSearch(root_, list, rationList);
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

  std::cout << "\n ~ depth search time " << elapsed_secs << std::endl << std::endl;

  return rationList;
}
