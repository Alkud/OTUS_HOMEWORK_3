// custom_allocator.h in Otus homework #3 project
#pragma once

#include <cstdlib>
#include <cstring>
#include "memory_manager.h"


template<typename T, size_t factor = 4>
struct custom_allocator
{
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  template<typename U> struct rebind { typedef custom_allocator<U, factor> other;};

  custom_allocator(){}
  ~custom_allocator(){}

  pointer allocate(size_t n) throw (std::bad_alloc)
  {
    void* result{ manager.capture(n * sizeof(T), factor * n * sizeof(T)) }; /* allocate and reserve*/
    return reinterpret_cast<T*>(result);
  }

  void deallocate(T* p, std::size_t n)
  {
    manager.release(reinterpret_cast<void*>(p));
  }

  void reallocate(T* p, std::size_t n)
  {

  }


  template <typename U, typename... Args>
  void construct (U* p, Args&&... args)
  {
    new(p) U{std::forward<Args>(args)...};
  }

  template <typename U>
  void destroy (U* p)
  {
    p->~U();
  }

private:
  memory_manager manager{};
};

