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
    node() :
      value{}, pLeft{}, pRight{}, pParent{}{}

    node(rvalue_reference _value) :
      value{std::move(_value)},
      pLeft{}, pRight{}, pParent{}{}

    node(const_reference _value) :
      value{_value},
      pLeft{}, pRight{}, pParent{}{}

    node(const node& other) :
      value{other.value},
      pLeft{other.pLeft}, pRight{other.pRight}, pParent{other.pParent}{}

    node(node&& other) :
      value{}, pLeft{}, pRight{}, pParent{}
    {
      std::swap(value, other.value);
      std::swap(pLeft,other.pLeft);
      std::swap(pRight, other.pRight);
      std::swap(pParent, other.pParent);
    }

    node& operator=(const node& other)
    {
      value = other.value;
      pLeft = other.pLeft;
      pRight = other.pRight;
      pParent = other.pParent;
      return *this;
    }

    node& operator=(node&& other)
    {
      std::swap(value, other.value);
      std::swap(pLeft,other.pLeft);
      std::swap(pRight, other.pRight);
      std::swap(pParent, other.pParent);
      return *this;
    }


    ~node(){}

    value_type value;
    std::shared_ptr<node> pParent;
    std::shared_ptr<node> pLeft;
    std::shared_ptr<node> pRight;
  };


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

    iterator(node* pointerToNode = nullptr) : pNode{pointerToNode} {}
    iterator(const iterator& other) : pNode{other.pNode} {}
    iterator(iterator&& other) : pNode{nullptr} {std::swap(pNode, other.pNode);}
    iterator& operator=(const iterator& other) {pNode = other.pNode; return *this;}
    iterator& operator=(iterator&& other){std::swap(pNode, other.pNode); return *this;}

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
      ++(*this);
      return tmpIterator;
    }

    reference operator*() { return pNode->value; }
    pointer operator->() { return std::addressof(pNode->value); }
    bool operator==(const self_type& other) { return pNode == other.pNode; }
    bool operator!=(const self_type& other) { return pNode != other.pNode; }

    std::shared_ptr<node> pNode;
  };

  class const_iterator:  public std::iterator<
                                std::forward_iterator_tag, // iterator_category
                                node,                      // value_type
                                difference_type,           // difference_type
                                const_pointer,             // pointer
                                const_reference            // reference
                                              >
  {
  public:
    using self_type = const_iterator ;
    using iterator_category = std::forward_iterator_tag ;

    const_iterator(node* pointerToNode = nullptr) : pNode{pointerToNode} {}
    const_iterator(const const_iterator& other) : pNode{other.pNode} {}
    const_iterator(const_iterator&& other) : pNode{nullptr} {std::swap(pNode, other.pNode);}
    const_iterator& operator=(const const_iterator& other) {pNode = other.pNode; return *this;}
    const_iterator& operator=(const_iterator&& other){std::swap(pNode, other.pNode); return *this;}

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

    const_reference operator*() { return pNode->value; }
    const_pointer operator->() { return std::addressof(pNode->value); }
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
    iterator result{root.get()};
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
      root = std::make_shared<node>(std::move(newValue));
      return std::make_pair<bool, iterator>(true, iterator{root.get()});
    }
    else
    {
      return(addChildNode(std::move(newValue), root.get()));
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
    iterator newValuePosition{find(newValue.first)};
    if (newValuePosition != end())                                      // new value is already inserted
    {
      return std::make_pair(false, newValuePosition);
    }
    else
    {
      if(Compare()(newValue.first, parent->value.first))                // if template comparator gives TRUE
      {                                                                 // add value to the LEFT child node
        if(nullptr == parent->pLeft)
        {
          parent->pLeft = std::make_shared<node>                        // create new child, if it doesn't exist
                          (std::move(newValue));
          return std::make_pair(true, iterator{parent->pLeft.get()});
        }
        else
        {
          return(addChildNode(std::move(newValue), parent->pRight.get()));    // or transfer it to the left child
        }
      }
      else                                                              // if template comparator gives FALSE
      {                                                                 // add value to the RIGHT child node
        if(nullptr == parent->pRight)
        {
          parent->pRight = std::make_shared<node>                       // create new child, if it doesn't exist
                           (std::move(newValue));
          return std::make_pair(true, iterator{parent->pRight.get()});
        }
        else
        {
          return(addChildNode(std::move(newValue), parent->pRight.get()));  // or transfer it to the right child
        }
      }
    }
  }

  std::pair<bool, iterator> addChildNode(const_reference newValue, node* parent)
  {
    iterator newValuePosition{find(newValue.first)};
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
