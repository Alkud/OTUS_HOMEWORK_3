// custom_map.h in Otus homework #3 project

#pragma once

#include <iterator>
#include <stdexcept>
#include <memory>

template<typename T>
class node
{
public:

  node() :
    value{},
    pLeft{nullptr}, pRight{nullptr}, pParent{nullptr}{}

  node(const T& _value) :
    value {_value},
    pLeft{nullptr}, pRight{nullptr}, pParent{nullptr}
  {}

  node(T&& _value, node*& parent) :
    value {std::move(_value)},
    pLeft{}, pRight{}, pParent{parent}
  {}

  node(const T& _value, node*& parent) :
    value {_value},
    pLeft{}, pRight{}, pParent{parent}
  {}

  ~node()
  {}

  T value;
  node* pLeft;
  node* pRight;
  node* pParent;
};

template< typename Key, typename T,
          class Compare = std::less<Key>,
          class Allocator = std::allocator<node<std::pair<const Key, T>>>>
class custom_map
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

  using nodeT = node<value_type>;

public:

  class iterator:  public std::iterator<
                          std::forward_iterator_tag, // iterator_category
                          value_type,                // value_type
                          difference_type,           // difference_type
                          pointer,                   // pointer
                          reference                  // reference
                                        >
  {
  public:
    using self_type = iterator ;
    using iterator_category = std::forward_iterator_tag ;

    iterator(nodeT* pointerToNode = nullptr) : pNode{pointerToNode}{}
    iterator(const iterator& other) : pNode{other.pNode} {}
    iterator(iterator&& other) : pNode{nullptr} {std::swap(pNode, other.pNode);}
    iterator& operator=(const iterator& other) {pNode = other.pNode; return *this;}
    iterator& operator=(iterator&& other){std::swap(pNode, other.pNode); return *this;}

    self_type operator++()                             // prefix increment
    {
      if (pNode->pRight != nullptr)                    // the node has a right child
      {
        pNode = pNode->pRight;                   // go to the right child
        while (pNode->pLeft != nullptr)                // then find the leftmost child
        {
          pNode = pNode->pLeft;
        }
      }
      else if (pNode->pParent != nullptr)        // the node has no right child but has a parent
      {
        nodeT* tmpNode{pNode->pParent};           // find its parent
        while (tmpNode != nullptr &&
               tmpNode->pRight != nullptr &&
               tmpNode->pRight == pNode)         // iterate through parents
        {                                              // while the are right parents
          pNode = tmpNode;
          tmpNode = tmpNode->pParent;
        }
        if (pNode->pRight != tmpNode)            //
          pNode = tmpNode;
      }
      else                                             // a node having no parent is root
      {
        pNode = nullptr;
      }
      return *this;
    }

    self_type operator++(int)                           // postfix increment
    {
      self_type tmpIterator{*this};
      ++(*this);
      return tmpIterator;
    }

    reference operator*() { return pNode->value; }
    pointer operator->() { return std::addressof(pNode->value); }
    bool operator==(const self_type& other) { return pNode == other.pNode; }
    bool operator!=(const self_type& other) { return pNode != other.pNode; }

    nodeT* pNode;
  };

  class const_iterator:  public std::iterator<
                                std::forward_iterator_tag, // iterator_category
                                value_type,                // value_type
                                difference_type,           // difference_type
                                const_pointer,             // pointer
                                const_reference            // reference
                                              >
  {
  public:
    using self_type = const_iterator ;
    using iterator_category = std::forward_iterator_tag ;

    const_iterator(nodeT* pointerToNode = nullptr) : pNode{pointerToNode} {}
    const_iterator(const const_iterator& other) : pNode{other.pNode} {}
    const_iterator(const_iterator&& other) : pNode{nullptr} {std::swap(pNode, other.pNode);}
    const_iterator& operator=(const const_iterator& other) {pNode = other.pNode; return *this;}
    const_iterator& operator=(const_iterator&& other){std::swap(pNode, other.pNode); return *this;}

    self_type operator++()                              // prefix increment
    {
      if (pNode->pRight != nullptr)                     // the node has a right child
      {
        pNode = pNode->pRight;                    // go to the right child
        while (pNode->pLeft != nullptr)                 // then find the leftmost child
        {
          pNode = pNode->pLeft;
        }
      }
      else if (pNode->pParent != nullptr)        // the node has no right child but has a parent
      {
        nodeT* tmpNode{pNode->pParent};           // find its parent
        while (tmpNode != nullptr &&
               tmpNode->pRight != nullptr &&
               tmpNode->pRight == pNode)         // iterate through parents
        {                                              // while the are right parents
          pNode = tmpNode;
          tmpNode = tmpNode->pParent;
        }
        if (pNode->pRight != tmpNode)
          pNode = tmpNode;
      }
      else                                             // a node having no parent is root
      {
        pNode = nullptr;
      }
      return *this;
    }

    self_type operator++(int)                          // postfix increment
    {
      self_type tmpIterator{*this};
      ++pNode;
      return tmpIterator;
    }

    const_reference operator*() { return pNode->value; }
    const_pointer operator->() { return std::addressof(pNode->value); }
    bool operator==(const self_type& other) { return pNode == other.pNode; }
    bool operator!=(const self_type& other) { return pNode != other.pNode; }

    nodeT* pNode;
  };


  iterator begin() const
  {
    if (nullptr == root)                                // map is empty
      return nullptr;

    iterator result{root};
    while(result.pNode->pLeft != nullptr)         // find the leftmost node
      result = result.pNode->pLeft;
    return result;
  }

  iterator end() const
  {
    return iterator{};
  }

  custom_map() :
    root{nullptr}, nodesCount{} {}

  custom_map(const custom_map& other) :
    root{nullptr}, nodesCount{}
  {
    for (const auto& item : other)
      insert(item);
  }

  custom_map(custom_map&& other) :
    root{nullptr}, nodesCount{other.nodesCount}
  {std::swap(root, other.root);}

  void clear_node(nodeT* pNode)
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

  ~custom_map()
  {
    if (root != nullptr)
      clear_node(root);
    allocator.destroy(root);
    allocator.deallocate(root, 1);
    root = nullptr;
  }

  reference at(const Key& key) const
  {
    auto iter{begin()};
    if (find(key) == end())
      throw std::out_of_range{"Key not found"};
    return iter->value;
  }

  iterator find(const Key& key) const
  {
    iterator result{root};
    while(result != end())
    {
      if (key == result->first)
        break;
      result++;
    }
    return result;
  }

  std::pair<bool, iterator> insert(rvalue_reference newValue)
  {
    if (root == nullptr)
    {
      root = allocator.allocate(1);
      allocator.construct(root, std::move(newValue));
      nodesCount++;
      return std::make_pair<bool, iterator>(true, iterator{root});
    }
    else
    {
      return(addChildNode(std::move(newValue), root));
    }
  }

  std::pair<bool, iterator> insert(const_reference newValue)
  {
    if (root == nullptr)
    {
      root = allocator.allocate(1);
      allocator.construct(root, newValue);
      nodesCount++;
      return std::make_pair<bool, iterator>(true, iterator{root});
    }
    else
    {
      return(addChildNode(std::move(newValue), root));
    }
  }

  std::pair<bool, iterator> insert(const Key& key, const T& value)
  {
    return insert(std::make_pair<Key, T>(key, value));
  }

  std::pair<bool, iterator> insert(Key&& key, T&& value)
  {
    return insert(std::make_pair<Key, T>(std::move(key), std::move(value)));
  }


  reference operator[](const Key& key)
  {

  }

  size_t count(){return nodesCount;}

private:

  std::pair<bool, iterator> addChildNode(rvalue_reference newValue, nodeT*& parent)
  {
    if (newValue.first == parent->value.first)
      return std::make_pair(false, iterator{parent});                // item exists
    if(Compare()(newValue.first, parent->value.first))               // if template comparator gives TRUE
    {
      if(nullptr == parent->pLeft)
      {
        parent->pLeft = allocator.allocate(1);
        allocator.construct(parent->pLeft, std::move(newValue));     // create new child, if it doesn't exist
        nodesCount++;
        return std::make_pair(true, iterator{parent->pLeft});
      }
      else
      {
        if (newValue.first == parent->pLeft->value.first)
          return std::make_pair(false, iterator{parent->pLeft});     // item exists
        else
          return(addChildNode(std::move(newValue), parent->pLeft));  // or transfer it to the left child
      }
    }
    else                                                             // if template comparator gives FALSE
    {                                                                // add value to the RIGHT child node
      if(nullptr == parent->pRight)
      {
        parent->pRight = allocator.allocate(1);
        allocator.construct(parent->pRight, std::move(newValue));     // create new child, if it doesn't exist
        nodesCount++;
        return std::make_pair(true, iterator{parent->pRight});
      }
      else
      {
        if (newValue.first == parent->pRight->value.first)
          return std::make_pair(false, iterator{parent->pRight});       // item exists
        else
          return(addChildNode(std::move(newValue), parent->pRight));   // or transfer it to the right child
      }
    }
  }

  std::pair<bool, iterator> addChildNode(const_reference newValue, nodeT*& parent)
  {
    if (newValue.first == parent->value.first)
      return std::make_pair(false, iterator{parent});                  // item exists
    if(Compare()(newValue.first, parent->value.first))                 // if template comparator gives TRUE
    {                                                                  // add value to the LEFT child node
      if(nullptr == parent->pLeft)
      {
        parent->pLeft = allocator.allocate(1);
        allocator.construct(parent->pLeft, newValue);                  // create new child, if it doesn't exist
        nodesCount++;
        return std::make_pair(true, iterator{parent->pLeft});
      }
      else
      {
        if (newValue.first == parent->pLeft->pValue.first)
          return std::make_pair(false, iterator{parent->pLeft});        // item exists
        else
          return(addChildNode(newValue, parent->pLeft));              // ... or transfer it to the left child
      }
    }
    else                                                               // if template comparator gives FALSE
    {                                                                  // add value to the RIGHT child node
      if(nullptr == parent->pRight)
      {
        parent->pRight = allocator.allocate(1);
        allocator.construct(parent->pRight, newValue);                 // create new child, if it doesn't exist
        nodesCount++;
        return std::make_pair(true, iterator{parent->pRight});
      }
      else
      {
        if (newValue.first == parent->pRight->pValue.first)
          return std::make_pair(false, iterator{parent->pRight});       // item exists
        else
          return(addChildNode(newValue, parent->pRight));                // ...or transfer it to the right child
      }
    }
  }

private:
  nodeT* root{nullptr};  // MAP ROOT
  size_t nodesCount; // Number of nodes
  Allocator allocator;
};
