#include "Tree.h"

#include <iostream>

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
