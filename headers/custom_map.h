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
    node(custom_map& _map) :
      pValue{nullptr}, pLeft{nullptr},
      pRight{nullptr}, pParent{nullptr}{}

    node(const_reference _value) :
      pLeft{nullptr}, pRight{nullptr}, pParent{nullptr}
    {
      pointer valueAddress = allocator.allocate(1);
      allocator.construct(valueAddress, _value);
      pValue = std::shared_ptr<value_type>(valueAddress);
    }

    node(rvalue_reference _value, std::shared_ptr<node>& parent) :
      pLeft{}, pRight{}, pParent{parent}
    {
      pointer valueAddress = allocator.allocate(1);
      allocator.construct(valueAddress, std::move(_value));
      pValue = std::shared_ptr<value_type>(valueAddress);
    }

    node(const_reference _value, std::shared_ptr<node>& parent) :
      pLeft{}, pRight{}, pParent{parent}
    {
      pointer valueAddress = allocator.allocate(1);
      allocator.construct(valueAddress, _value);
      pValue = std::shared_ptr<value_type>(valueAddress);
    }

//    node(node* other) :
//      value{other->value},
//      pLeft{other->pLeft}, pRight{other->pRight}, pParent{other->pParent}{}

//    node(const node& other) :
//      value{other.value},
//      pLeft{other.pLeft}, pRight{other.pRight}, pParent{other.pParent}{}

//    node(node&& other) :
//      value{}, pLeft{}, pRight{}, pParent{}
//    {
//      std::swap(value, other.value);
//      std::swap(pLeft,other.pLeft);
//      std::swap(pRight, other.pRight);
//      std::swap(pParent, other.pParent);
//    }

    node& operator=(const node& other)
    {
      pValue = other.pValue;
      pLeft = other.pLeft;
      pRight = other.pRight;
      pParent = other.pParent;
      return *this;
    }

    node& operator=(node&& other)
    {
      std::swap(pValue, other.pVvalue);
      std::swap(pLeft,other.pLeft);
      std::swap(pRight, other.pRight);
      std::swap(pParent, other.pParent);
      return *this;
    }


    ~node(){}

    std::shared_ptr<value_type> pValue;
    std::shared_ptr<node> pLeft;
    std::shared_ptr<node> pRight;
    std::shared_ptr<node> pParent;
    Allocator allocator{};
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

    iterator(node* pointerToNode = nullptr) : pNode{pointerToNode}{}
    iterator(const iterator& other) : pNode{other.pNode} {}
    iterator(iterator&& other) : pNode{nullptr} {std::swap(pNode, other.pNode);}
    iterator& operator=(const iterator& other) {pNode = other.pNode; return *this;}
    iterator& operator=(iterator&& other){std::swap(pNode, other.pNode); return *this;}

    self_type operator++()                             // prefix increment
    {
      if (pNode->pRight != nullptr)                    // the node has a right child
      {
        pNode = pNode->pRight.get();                   // go to the right child
        while (pNode->pLeft != nullptr)                // then find the leftmost child
        {
          pNode = pNode->pLeft.get();
        }
      }
      else if (pNode->pParent.get() != nullptr)        // the node has no right child but has a parent
      {
        node* tmpNode{pNode->pParent.get()};           // find its parent
        while (tmpNode != nullptr &&
               tmpNode->pRight.get() != nullptr &&
               tmpNode->pRight.get() == pNode)         // iterate through parents
        {                                              // while the are right parents
          pNode = tmpNode;
          tmpNode = tmpNode->pParent.get();
        }
        if (pNode->pRight.get() != tmpNode)            //
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

    reference operator*() { return *pNode->pValue.get(); }
    pointer operator->() { return pNode->pValue.get(); }
    bool operator==(const self_type& other) { return pNode == other.pNode; }
    bool operator!=(const self_type& other) { return pNode != other.pNode; }

    node* pNode;
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

    self_type operator++()                              // prefix increment
    {
      if (pNode->pRight != nullptr)                     // the node has a right child
      {
        pNode = pNode->pRight.get();                    // go to the right child
        while (pNode->pLeft != nullptr)                 // then find the leftmost child
        {
          pNode = pNode->pLeft.get();
        }
      }
      else if (pNode->pParent.get() != nullptr)        // the node has no right child but has a parent
      {
        node* tmpNode{pNode->pParent.get()};           // find its parent
        while (tmpNode != nullptr &&
               tmpNode->pRight.get() != nullptr &&
               tmpNode->pRight.get() == pNode)         // iterate through parents
        {                                              // while the are right parents
          pNode = tmpNode;
          tmpNode = tmpNode->pParent.get();
        }
        if (pNode->pRight.get() != tmpNode)
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

    const_reference operator*() { return pNode->pValue.get(); }
    const_pointer operator->() { return std::addressof(pNode->pValue.get()); }
    bool operator==(const self_type& other) { return pNode == other.pNode; }
    bool operator!=(const self_type& other) { return pNode != other.pNode; }

    node* pNode;
  };


  iterator begin() const
  {
    if (nullptr == root)                                // map is empty
      return nullptr;

    iterator result{root.get()};
    while(result.pNode->pLeft.get() != nullptr)         // find the leftmost node
      result = result.pNode->pLeft.get();
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

  ~custom_map()
  {}

  reference at(const Key& key) const
  {
    auto iter{begin()};
    if (find(key) == end())
      throw std::out_of_range{"Key not found"};
    return iter->pValue.get();
  }

  iterator find(const Key& key) const
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
      nodesCount++;
      return std::make_pair<bool, iterator>(true, iterator{root.get()});
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
      root = std::make_shared<node>(std::move(newValue));
      nodesCount++;
      return std::make_pair<bool, iterator>(true, iterator{root.get()});
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

  std::pair<bool, iterator> addChildNode(rvalue_reference newValue, std::shared_ptr<node>& parent)
  {
    if(Compare()(newValue.first, parent->pValue->first))               // if template comparator gives TRUE
    {                                                                  // add value to the LEFT child node
      if(nullptr == parent->pLeft)
      {
        parent->pLeft = std::make_shared<node>                         // create new child, if it doesn't exist
                        (std::move(newValue), parent);
        nodesCount++;
        return std::make_pair(true, iterator{parent->pLeft.get()});
      }
      else
      {
        if (newValue.first == parent->pLeft->pValue->first)
          return std::make_pair(false, iterator{parent->pLeft.get()}); // item exists
        else
          return(addChildNode(std::move(newValue), parent->pLeft));    // or transfer it to the left child
      }
    }
    else                                                               // if template comparator gives FALSE
    {                                                                  // add value to the RIGHT child node
      if(nullptr == parent->pRight)
      {
        parent->pRight = std::make_shared<node>                        // create new child, if it doesn't exist
                         (std::move(newValue), parent);
        nodesCount++;
        return std::make_pair(true, iterator{parent->pRight.get()});
      }
      else
      {
        if (newValue.first == parent->pRight->pValue->first)
          return std::make_pair(false, iterator{parent->pRight.get()});// item exists
        else
          return(addChildNode(std::move(newValue), parent->pRight));   // or transfer it to the right child
      }
    }
  }

  std::pair<bool, iterator> addChildNode(const_reference newValue, std::shared_ptr<node>& parent)
  {
    if(Compare()(newValue.first, parent->pValue->first))               // if template comparator gives TRUE
    {                                                                  // add value to the LEFT child node
      if(nullptr == parent->pLeft)
      {
        parent->pLeft = std::make_shared<node>                         // create new child, if it doesn't exist
                        (newValue, parent);
        nodesCount++;
        return std::make_pair(true, iterator{parent->pLeft.get()});
      }
      else
      {
        if (newValue.first == parent->pLeft->pValue->first)
          return std::make_pair(false, iterator{parent->pLeft.get()}); // item exists
        else
          return(addChildNode(newValue, parent->pLeft));    // or transfer it to the left child
      }
    }
    else                                                               // if template comparator gives FALSE
    {                                                                  // add value to the RIGHT child node
      if(nullptr == parent->pRight)
      {
        parent->pRight = std::make_shared<node>                        // create new child, if it doesn't exist
                         (newValue, parent);
        nodesCount++;
        return std::make_pair(true, iterator{parent->pRight.get()});
      }
      else
      {
        if (newValue.first == parent->pRight->pValue->first)
          return std::make_pair(false, iterator{parent->pRight.get()});// item exists
        else
          return(addChildNode(newValue, parent->pRight));   // or transfer it to the right child
      }
    }
  }

private:
  std::shared_ptr<node> root{nullptr};  // MAP ROOT
  size_t nodesCount; // Number of nodes
};
