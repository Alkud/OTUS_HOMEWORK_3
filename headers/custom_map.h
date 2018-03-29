// custom_map.h in Otus homework #3 project

#pragma once

#include <iterator>
#include <stdexcept>
#include <memory>
#include <queue>

template<typename T>
class Node
{
public:

  Node() :
    value{},
    pLeft{nullptr}, pRight{nullptr}, pParent{nullptr}{}

  Node(const T* const& _pValue) :
    value{_pValue},
    pLeft{nullptr}, pRight{nullptr}, pParent{nullptr}{}

  Node(const T& _value) :
    value {_value},
    pLeft{nullptr}, pRight{nullptr}, pParent{nullptr}
  {}

  Node(T&& _value) :
    value {std::move(_value)},
    pLeft{nullptr}, pRight{nullptr}, pParent{nullptr}
  {}

  Node(T&& _value, Node* const & parent) :
    value {std::move(_value)},
    pLeft{nullptr}, pRight{nullptr}, pParent{parent}
  {}

  Node(const T& _value, Node* const & parent) :
    value {std::move(_value)},
    pLeft{nullptr}, pRight{nullptr}, pParent{parent}
  {}

  ~Node()
  {}

  T value;
  Node* pLeft;
  Node* pRight;
  Node* pParent;
};

template<
          typename Key, typename T,
          class Compare = std::less<Key>,
          class Allocator = std::allocator<Node<std::pair<const Key, T>>>
        >
class CustomMap
{

public:

  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using rvalue_reference = value_type&&;
  using pointer = value_type*;//typename std::allocator_traits<Allocator>::pointer;
  using const_pointer = value_type* const;//typename std::allocator_traits<Allocator>::const_pointer;

private:

  using NodeVT = Node<value_type>;

public:

  class Iterator:  public std::iterator<
                          std::forward_iterator_tag, // iterator_category
                          value_type,                // value_type
                          difference_type,           // difference_type
                          pointer,                   // pointer
                          reference                  // reference
                                        >
  {
  public:
    using self_type = Iterator ;
    using iterator_category = std::forward_iterator_tag ;

    Iterator(NodeVT* pointerToNode = nullptr) : pNode{pointerToNode}{}
    Iterator(const Iterator& other) : pNode{other.pNode} {}
    Iterator(Iterator&& other) : pNode{nullptr} {std::swap(pNode, other.pNode);}
    Iterator& operator=(const Iterator& other) {pNode = other.pNode; return *this;}
    Iterator& operator=(Iterator&& other){std::swap(pNode, other.pNode); return *this;}

    self_type operator++()                                 // prefix increment
    {
      if (pNode->pRight != nullptr)                        // the node has a right child
      {
        pNode = pNode->pRight;                             // go to the right child
        while (pNode->pLeft != nullptr)                    // then find the leftmost child
        {
          pNode = pNode->pLeft;
        }
      }
      else if (pNode->pParent != nullptr)                  // the node has no right child but has a parent
      {
        NodeVT* tmpNode{pNode->pParent};                    // find its parent
        while (tmpNode != nullptr &&
               tmpNode->pRight != nullptr &&
               tmpNode->pRight == pNode)                   // iterate through parents
        {                                                  // while the are right parents
          pNode = tmpNode;
          tmpNode = tmpNode->pParent;
        }
        if (pNode->pRight != tmpNode)                      //
          pNode = tmpNode;
      }
          else                                             // a node having no parent is root
      {
        pNode = nullptr;
      }
      return *this;
    }

    self_type operator++(int)                              // postfix increment
    {
      self_type tmpIterator{*this};
      ++(*this);
      return tmpIterator;
    }

    reference operator*() { return pNode->value; }
    pointer operator->() { return std::addressof(pNode->value); }
    bool operator==(const self_type& other) { return pNode == other.pNode; }
    bool operator!=(const self_type& other) { return pNode != other.pNode; }

    NodeVT* pNode;
  };

  class ConstIterator:  public std::iterator<
                                std::forward_iterator_tag, // iterator_category
                                value_type,                // value_type
                                difference_type,           // difference_type
                                const_pointer,             // pointer
                                const_reference            // reference
                                              >
  {
  public:
    using self_type = ConstIterator ;
    using iterator_category = std::forward_iterator_tag ;

    ConstIterator(NodeVT* pointerToNode = nullptr) : pNode{pointerToNode} {}
    ConstIterator(const ConstIterator& other) : pNode{other.pNode} {}
    ConstIterator(ConstIterator&& other) : pNode{nullptr} {std::swap(pNode, other.pNode);}
    ConstIterator& operator=(const ConstIterator& other) {pNode = other.pNode; return *this;}
    ConstIterator& operator=(ConstIterator&& other){std::swap(pNode, other.pNode); return *this;}

    self_type operator++()                                 // prefix increment
    {
      if (pNode->pRight != nullptr)                        // the node has a right child
      {
        pNode = pNode->pRight;                             // go to the right child
        while (pNode->pLeft != nullptr)                    // then find the leftmost child
        {
          pNode = pNode->pLeft;
        }
      }
      else if (pNode->pParent != nullptr)                   // the node has no right child but has a parent
      {
        NodeVT* tmpNode{pNode->pParent};                     // find its parent
        while (tmpNode != nullptr &&
               tmpNode->pRight != nullptr &&
               tmpNode->pRight == pNode)                    // iterate through parents
        {                                                   // while the are right parents
          pNode = tmpNode;
          tmpNode = tmpNode->pParent;
        }
        if (pNode->pRight != tmpNode)
          pNode = tmpNode;
      }
      else                                                   // a node having no parent is root
      {
        pNode = nullptr;
      }
      return *this;
    }

    self_type operator++(int)                                // postfix increment
    {
      self_type tmpIterator{*this};
      ++pNode;
      return tmpIterator;
    }

    const_reference operator*() { return *(pNode->pValue); }
    const_pointer operator->() { return std::addressof(pNode->pValue); }
    bool operator==(const self_type& other) { return pNode == other.pNode; }
    bool operator!=(const self_type& other) { return pNode != other.pNode; }

    NodeVT* pNode;
  };


  Iterator begin() const
  {
    if (nullptr == pRoot)                                // map is empty
      return Iterator{nullptr};

    Iterator result{pRoot};
    while(result.pNode->pLeft != nullptr)                // find the leftmost node
      result = result.pNode->pLeft;
    return result;
  }

  Iterator end() const
  {
    return Iterator{nullptr};
  }

  CustomMap() :
    pRoot{nullptr}, nodesCount{} {}

  CustomMap(const CustomMap& other) :
    pRoot{nullptr}, nodesCount{}
  {
    for (const auto& item : other)
      insert(item);
  }

  CustomMap(CustomMap&& other) :
    pRoot{nullptr}, nodesCount{other.nodesCount}
  {pRoot.swap(other.pRoot);}


  void clear_node(NodeVT* pNode)
  {
    if (pNode->pLeft != nullptr)
    {
      if (pNode->pLeft->pLeft  != nullptr ||
          pNode->pLeft->pRight != nullptr   )
      {
        clear_node(pNode->pLeft);
      }
      allocator.destroy(pNode->pLeft);
      allocator.deallocate(pNode->pLeft, 1);
      pNode->pLeft = nullptr;
    }

    if (pNode->pRight != nullptr)
    {
      if (pNode->pRight->pLeft != nullptr ||
          pNode->pRight->pRight != nullptr)
      {
        clear_node(pNode->pRight);
      }
      allocator.destroy(pNode->pRight);
      allocator.deallocate(pNode->pRight, 1);
      pNode->pRight = nullptr;
    }
  }

  ~CustomMap()
  {
    if (pRoot != nullptr)
      clear_node(pRoot);
    allocator.destroy(pRoot);
    allocator.deallocate(pRoot, 1);
    pRoot = nullptr;
  }

  reference at(const Key& key) const
  {
    auto iter{begin()};
    if (find(key) == end())
      throw std::out_of_range{"Key not found"};
    return *iter;
  }

  Iterator find(const Key& key) const
  {
    Iterator result{pRoot};
    while(result != end())
    {
      if (key == result->first)
        break;
      result++;
    }
    return result;
  }

  std::pair<bool, Iterator> insert(value_type&& newValue)
  {
    if (pRoot == nullptr)
    {
      pRoot = allocator.allocate(1);
      allocator.construct(pRoot, std::move(newValue));
      nodesCount++;
      return std::make_pair<bool, Iterator>(true, Iterator{pRoot});
    }
    else
    {
      return(addChildNode(std::move(newValue), pRoot));
    }
  }

  std::pair<bool, Iterator> insert(const value_type& newValue)
  {
    if (pRoot == nullptr)
    {
      pRoot = allocator.allocate(1);
      allocator.construct(pRoot, newValue);
      nodesCount++;
      return std::make_pair<bool, Iterator>(true, Iterator{pRoot});
    }
    else
    {
      return(addChildNode(std::move(newValue), pRoot));
    }
  }

  std::pair<bool, Iterator> insert(const Key& key, const T& value)
  {
    return insert(std::make_pair<Key, T>(key, value));
  }

  std::pair<bool, Iterator> insert(Key&& key, T&& value)
  {
    return insert(std::make_pair<Key, T>(std::move(key), std::move(value)));
  }


  reference operator[](const Key& key)
  {

  }

  size_t count(){return nodesCount;}

private:

  std::pair<bool, Iterator>
  addChildNode(rvalue_reference newValue,
               NodeVT* const & parent)
  {
    if (newValue.first == parent->value.first)
      return std::make_pair(false, Iterator{parent});                // item exists
    if(Compare()(newValue.first, parent->value.first))             // if template comparator gives TRUE
    {
      if(nullptr == parent->pLeft)                                   // create new child, if it doesn't exist
      {
        parent->pLeft = allocator.allocate(1);
        allocator.construct(parent->pLeft, std::move(newValue));
        nodesCount++;
        return std::make_pair(true, Iterator{parent->pLeft});
      }
      else
      {
        if (newValue.first == parent->pLeft->value.first)
          return std::make_pair(false, Iterator{parent->pLeft});     // item exists
        else
          return(addChildNode(std::move(newValue), parent->pLeft));  // or transfer it to the left child
      }
    }
    else                                                             // if template comparator gives FALSE
    {                                                                // add value to the RIGHT child node
      if(nullptr == parent->pRight)                                  // create new child, if it doesn't exist
      {
        parent->pRight = allocator.allocate(1);
        allocator.construct(parent->pRight, std::move(newValue));
        nodesCount++;
        return std::make_pair(true, Iterator{parent->pRight});
      }
      else
      {
        if (newValue.first == parent->pRight->value.first)
          return std::make_pair(false, Iterator{parent->pRight});       // item exists
        else
          return(addChildNode(std::move(newValue), parent->pRight));   // or transfer it to the right child
      }
    }
  }

  std::pair<bool, Iterator>
  addChildNode(const_reference newValue,
               NodeVT* const & parent)
  {
    if (newValue.first == parent->pValue->first)
      return std::make_pair(false, Iterator{parent});                // item exists
    if(Compare()(newValue.first, parent->pValue->first))             // if template comparator gives TRUE
    {
      if(nullptr == parent->pLeft)                                   // create new child, if it doesn't exist
      {
        parent->pLeft = allocator.allocate(1);
        allocator.construct(parent->pLeft, newValue);
        nodesCount++;
        return std::make_pair(true, Iterator{parent->pLeft});
      }
      else
      {
        if (newValue.first == parent->pLeft->pValue->first)
          return std::make_pair(false, Iterator{parent->pLeft});     // item exists
        else
          return(addChildNode(newValue, parent->pLeft));  // or transfer it to the left child
      }
    }
    else                                                             // if template comparator gives FALSE
    {                                                                // add value to the RIGHT child node
      if(nullptr == parent->pRight)                                  // create new child, if it doesn't exist
      {
        parent->pRight = allocator.allocate(1);
        allocator.construct(parent->pRight, newValue);
        nodesCount++;
        return std::make_pair(true, Iterator{parent->pRight});
      }
      else
      {
        if (newValue.first == parent->pRight->pValue->first)
          return std::make_pair(false, Iterator{parent->pRight});       // item exists
        else
          return(addChildNode(newValue, parent->pRight));   // or transfer it to the right child
      }
    }
  }

private:
  NodeVT* pRoot{nullptr};  // MAP ROOT
  size_t nodesCount; // Number of nodes
  Allocator allocator;
};
