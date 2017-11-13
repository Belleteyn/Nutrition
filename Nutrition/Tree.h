#ifndef TREE_H
#define TREE_H

#include <Node.h>
#include <Food.h>

class FoodTree
{
public:
  using FoodNode = Node<Food>;

  FoodTree();
  virtual ~FoodTree();

  void addLeaves(const std::list<FoodNode*>& leaves);

  void print() const;

private:
  FoodNode* root_;
  std::list<FoodNode*> leaves_;
};

#endif // TREE_H
