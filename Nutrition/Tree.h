#ifndef TREE_H
#define TREE_H

#include <Node.h>
#include <Food.h>

class FoodTree
{
public:
  using FoodNode = Node<Food>;
  using Ration = std::list<const Food*>;
  using RationList = std::list<std::list<const Food*>>;

  using NutritionErrorComparator = std::function<bool(const Nutrition&)>;

  FoodTree();
  virtual ~FoodTree();

  void addLeaves(const std::list<FoodNode*>& leaves, const NutritionErrorComparator& overheadingComparator);

  void print() const;

  RationList depthSearch(const NutritionErrorComparator& allowedErrorComparator);

private:
  FoodNode* root_;
  std::list<FoodNode*> leaves_;
};

#endif // TREE_H
