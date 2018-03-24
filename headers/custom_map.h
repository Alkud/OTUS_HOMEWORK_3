// custom_map.h in Otus homework #3 project

#pragma once

#include <iterator>

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
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

private:

  class node
  {
  public:
    node(value_type _value) :
      value{_value}, pLeft{nullptr}, pRight{nullptr},
      pParent{nullptr}{}

    ~node(){}

    value_type value;
    node* pParent;
    node* pLeft;
    node* pRight;
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

    iterator(node* _ptr) : ptr(_ptr) {}

    self_type operator++()            // prefix increment
    {
      if (ptr->pRight != nullptr)     // the node has a right child
      {
        ptr = ptr->pRight;            // go to the right child
        while (ptr->pLeft != nullptr) // then find the leftmost child
        {
          ptr = ptr->pLeft;
        }
        ptr = ptr->pLeft;
      }
      else                            // the node has no right child
      {
        node* tmpPtr{ptr->pParent};   // find its parent
        while (tmpPtr->pRight == ptr) // iterate through parents
        {                             // while the are right parents
          ptr = tmpPtr;
          tmpPtr = tmpPtr->pParent;
        }
        if (ptr->pRight != tmpPtr)    //
          ptr = tmpPtr;
      }
      return *this;
    }

    self_type operator++(int)         // postfix increment
    {
      self_type tmpIterator{*this};
      ++ptr;
      return tmpIterator;
    }

    reference operator*() { return *ptr; }
    pointer operator->() { return ptr; }
    bool operator==(const self_type& other) { return ptr == other.ptr; }
    bool operator!=(const self_type& other) { return ptr != other.ptr; }
   private:
    node* ptr;
  };

  class const_iterator : public std::iterator<
                                std::forward_iterator_tag, // iterator_category
                                value_type,                // value_type
                                difference_type,           // difference_type
                                pointer,                   // pointer
                                reference                  // reference
                                             >
          {
              public:
                  using self_type = const_iterator;
                  typedef std::forward_iterator_tag iterator_category;
                  const_iterator(pointer ptr) : ptr_(ptr) { }
                  self_type operator++() { self_type i = *this; ptr_++; return i; }
                  self_type operator++(int) { ptr_++; return *this; }
                  const reference operator*() { return *ptr_; }
                  const pointer operator->() { return ptr_; }
                  bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
                  bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
              private:
                  pointer ptr_;
  };


  iterator begin()
  {
    if (nullptr == root)              // map is empty
      return nullptr;

    iterator result{root};
    while(result->pLeft != nullptr) // find the left most node
      result = result->pLeft;
    return result;
  }

  iterator end();

  custom_map()
  {}

  ~custom_map()
  {}

private:
  node* root{nullptr};
};
