#ifndef FOODNODE_H
#define FOODNODE_H

#include <Node.h>
#include <Food.h>

class FoodNode : public Node<FoodPtr>
{
public:
  FoodNode()
    : Node<FoodPtr>(FoodPtr(new Food("")))
  {}

  FoodNode(FoodPtr food)
    : Node<FoodPtr>(food)
  {}
  ~FoodNode() = default;

  const Food& getFood() const { return *body_.get(); }
  using Node<FoodPtr>::getSub;

  void setSub(const std::list<FoodNode>& sub) { sub_.clear(); sub_.assign(sub.begin(), sub.end()); }
  using Node<FoodPtr>::eraseSub;
};


#endif // FOODNODE_H
