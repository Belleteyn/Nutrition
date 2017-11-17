#ifndef TREE_H
#define TREE_H

#include <FoodNode.h>

class FoodTree
{
public:
  using Ration = std::list<const Food*>;
  using RationList = std::list<std::list<const Food*>>;

  using NutritionErrorComparator = std::function<bool(const Nutrition&)>;

  FoodTree();
  virtual ~FoodTree();

  void addLeaves(const std::list<FoodNode*>& leaves, const NutritionErrorComparator& overheadingComparator);

  void print() const;

  RationList depthSearch(const NutritionErrorComparator& allowedErrorComparator
                         , const NutritionErrorComparator& overheadingComparator);

private:
  FoodNode* root_;
  std::list<FoodNode*> leaves_;
};

#endif // TREE_H
