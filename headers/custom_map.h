// custom_map.h in Otus homework #3 project

#pragma once

#include <iterator>
#include <stdexcept>
#include <memory>

template< typename Key, typename T,
          class Compare = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
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
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

private:

  class node
  {
  public:
    node(rvalue_reference _value) :
      value{std::move(_value)},
      pLeft{std::shared_ptr<node>{nullptr}},
      pRight{std::shared_ptr<node>{nullptr}},
      pParent{std::shared_ptr<node>{nullptr}}{}

    node(const_reference _value) :
      value{_value},
      pLeft{std::shared_ptr<node>{nullptr}},
      pRight{std::shared_ptr<node>{nullptr}},
      pParent{std::shared_ptr<node>{nullptr}}{}

    ~node(){}

    value_type value;
    std::shared_ptr<node> pParent;
    std::shared_ptr<node> pLeft;
    std::shared_ptr<node> pRight;
  };


public:

  class iterator:  public std::iterator<
                          std::forward_iterator_tag, // iterator_category
                          node,                      // value_type
                          difference_type,           // difference_type
                          node*,                     // pointer
                          node&                      // reference
                                        >
  {
  public:
    using self_type = iterator ;
    using iterator_category = std::forward_iterator_tag ;

    iterator(node* pointerToNode) : pNode{pointerToNode} {}

    self_type operator++()                              // prefix increment
    {

      if (pNode->pRight != nullptr)                     // the node has a right child
      {
        pNode = pNode->pRight;                          // go to the right child
        while (pNode->pLeft != nullptr)                 // then find the leftmost child
        {
          pNode = pNode->pLeft;
        }
        pNode = pNode->pLeft;
      }
      else                                              // the node has no right child
      {
        std::shared_ptr<node> tmpNode{pNode->pParent};  // find its parent
        while (tmpNode->pRight == pNode)                // iterate through parents
        {                                               // while the are right parents
          pNode = tmpNode;
          tmpNode = tmpNode->pParent;
        }
        if (pNode->pRight != tmpNode)                   //
          pNode = tmpNode;
      }
      return *this;
    }

    self_type operator++(int)                           // postfix increment
    {
      self_type tmpIterator{*this};
      ++pNode;
      return tmpIterator;
    }

    reference operator*() { return pNode->value; }
    pointer operator->() { return pNode; }
    bool operator==(const self_type& other) { return pNode == other.pNode; }
    bool operator!=(const self_type& other) { return pNode != other.pNode; }

    std::shared_ptr<node> pNode;
  };

  class const_iterator:  public std::iterator<
                                std::forward_iterator_tag, // iterator_category
                                node,                      // value_type
                                difference_type,           // difference_type
                                node*,                     // pointer
                                node&                      // reference
                                              >
  {
  public:
    using self_type = const_iterator ;
    using iterator_category = std::forward_iterator_tag ;

    const_iterator(node* pointerToNode) : pNode(pointerToNode) {}

    self_type operator++()               // prefix increment
    {
      if (pNode->pRight != nullptr)      // the node has a right child
      {
        pNode = pNode->pRight;           // go to the right child
        while (pNode->pLeft != nullptr)  // then find the leftmost child
        {
          pNode = pNode->pLeft;
        }
        pNode = pNode->pLeft;
      }
      else                               // the node has no right child
      {
        node* tmpNode{pNode->pParent};   // find its parent
        while (tmpNode->pRight == pNode) // iterate through parents
        {                                // while the are right parents
          pNode = tmpNode;
          tmpNode = tmpNode->pParent;
        }
        if (pNode->pRight != tmpNode)    //
          pNode = tmpNode;
      }
      return *this;
    }

    self_type operator++(int)            // postfix increment
    {
      self_type tmpIterator{*this};
      ++pNode;
      return tmpIterator;
    }

    const_reference operator*() { return *pNode; }
    const_pointer operator->() { return pNode; }
    bool operator==(const self_type& other) { return pNode == other.pNode; }
    bool operator!=(const self_type& other) { return pNode != other.pNode; }

    node* pNode;
  };


  iterator begin()
  {
    if (nullptr == root)                        // map is empty
      return nullptr;

    iterator result{root.get()};
    while(result.pNode->pLeft.get() != nullptr) // find the leftmost node
      result = result.pNode->pLeft.get();
    return result;
  }

  iterator end()
  {
    return nullptr;
  }

  custom_map() :
    root{nullptr}
  {}

  ~custom_map()
  {}

  reference at(const Key& key)
  {
    auto iter{begin()};
    if (find(key) == end())
      throw std::out_of_range{"Key not found"};
    return iter->value;
  }

  iterator find(const Key& key)
  {
    iterator result{root};
    while(result != end())
    {
      if (key == result->value.first)
        break;
      result++;
    }
    return result;
  }

  std::pair<bool, iterator> insert(rvalue_reference newValue)
  {
    if (root == nullptr)
    {
      root.swap(std::make_shared<node>(std::move(newValue)));
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
      root = new node{newValue};
      return std::make_pair<bool, iterator>(true, iterator{root});
    }
    else
    {
      return(addChildNode(std::move(newValue), root));
    }
  }


  reference operator[](const Key& key)
  {

  }

private:

  std::pair<bool, iterator> addChildNode(rvalue_reference newValue, node* parent)
  {
    iterator newValuePosition{find(newValue)};
    if (newValuePosition != end())                                      // new value is already inserted
    {
      return std::make_pair<false, iterator>(true, newValuePosition);
    }
    else
    {
      if(Compare(newValue.first, parent->value.first))                  // if template comparator gives TRUE
      {                                                                 // add value to the LEFT child node
        if(nullptr == parent->pLeft)
        {
          parent->pLeft = new node(std::move(newValue));                // create new child, if it doesn't exist
          return std::make_pair<true, iterator>(true, iterator{parent->pLeft});
        }
        else
        {
          return(addChildNode(std::move(newValue), parent->pRight));    // or transfer it to the left child
        }
      }
      else                                                              // if template comparator gives FALSE
      {                                                                 // add value to the RIGHT child node
        if(nullptr == parent->pRight)
        {
          parent->pRight = new node(std::move(newValue));             // create new child, if it doesn't exist
          return std::make_pair<true, iterator>(true, iterator{parent->pRight});
        }
        else
        {
          return(addChildNode(std::move(newValue), parent->pRight));  // or transfer it to the right child
        }
      }
    }
  }

  std::pair<bool, iterator> addChildNode(const_reference newValue, node* parent)
  {
    iterator newValuePosition{find(newValue)};
    if (newValuePosition != end())                                      // new value is already inserted
    {
      return std::make_pair<false, iterator>(true, newValuePosition);
    }
    else                                                                // insert new value
    {
      if(Compare(newValue.first, parent->value.first))                  // if template comparator gives TRUE
      {                                                                 // add value to the LEFT child node
        if(nullptr == parent->pLeft)
        {
          parent->pLeft = new node(newValue);                           // create new child, if it doesn't exist
          return std::make_pair<true, iterator>(true, iterator{parent->pLeft});
        }
        else
        {
          return(addChildNode(newValue, parent->pRight));               // or try to add it to the left child
        }
      }
      else                                                              // if template comparator gives FALSE
      {                                                                 // add value to the RIGHT child node
        if(nullptr == parent->pRight)
        {
          parent->pRight = new node(newValue);                          // create new child, if it doesn't exist
          return std::make_pair<true, iterator>(true, iterator{parent->pRight});
        }
        else
        {
          return(addChildNode(newValue, parent->pRight));              // or try to add it to the right child
        }
      }
    }
  }

  std::shared_ptr<node> root{nullptr};  // MAP ROOT
};
