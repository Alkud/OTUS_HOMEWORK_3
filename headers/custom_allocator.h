// custom_allocator.h in Otus homework #3 project
#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include "memory_manager.h"


template<typename T, size_t capacity = 10>
struct CustomAllocator
{
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  template<typename U> struct rebind { typedef CustomAllocator<U, capacity> other;};


  CustomAllocator(){}
  ~CustomAllocator()
  {
    while (!allocatedArrays.empty())
      allocatedArrays.pop_back();
  }

  pointer allocate(size_t n) throw (std::bad_alloc)
  {
    if (allocatedArrays.empty()
        || capacity - offset < n) // actual capacity is exceeded
    {
      allocatedArrays.push_back(std::unique_ptr<T[]>{new T [capacity]});
    }
    size_t _offset{offset};
    offset += n;
    size_t arrayNumber{_offset / capacity};
    size_t arrayIndex {_offset % capacity}    ;
    return &(allocatedArrays[arrayNumber].get())[arrayIndex];
  }

  void deallocate(T* p, std::size_t n)
  {
  }

  template <typename U, typename... Args>
  void construct (U* p, Args&&... args)
  {
    ::new(p) U{std::forward<Args>(args)...};
  }

  template <typename U>
  void destroy (U* p)
  {
    p->~U();
  }

  void destroy (T* p)
  {
  }

private:
  size_t offset{};
  std::vector<std::unique_ptr<T[]>> allocatedArrays{};
};
