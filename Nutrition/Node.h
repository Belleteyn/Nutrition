#ifndef NODE_H
#define NODE_H

#include <list>
#include <iostream>

template <class T>
class Node
{
public:
  Node()
    : body_(T())
    , sub_()
  {}

  Node(const T& body)
    : body_(body)
    , sub_()
  {}

  Node(const T& body, const std::list<Node*>& sub)
    : body_(body)
    , sub_(sub)
  {}

  Node(const Node& other) = delete;
  Node& operator =(const Node& other) = delete;

  virtual ~Node() = default;

  void setSub(const std::list<Node*>& sub) { sub_ = sub; }

  const T& getBody() const { return body_; }

private:
  T body_;
  std::list<Node*> sub_;
};

#endif // NODE_H
