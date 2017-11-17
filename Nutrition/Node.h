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

  Node(const T& body, const std::list<Node>& sub)
    : body_(body)
    , sub_(sub)
  {}

  Node(const Node& other)
    : body_(other.body_)
    , sub_(other.sub_)
  {}

  Node operator =(const Node& other)
  {
    return Node(other);
  }

  virtual ~Node() = default;

  void setSub(const std::list<Node>& sub) { sub_ = sub; }

  const T& getBody() const { return body_; }
  const std::list<Node>& getSub() const { return sub_; }


  typename std::list<Node>::iterator eraseSub(typename std::list<Node>::const_iterator iter)
  {
    return sub_.erase(iter, sub_.end());
  }

protected:
  T body_;
  std::list<Node> sub_;
};

#endif // NODE_H
