// custom_allocator.h in Otus homework #3 project
#pragma once

template<typename T, std::size_t n = 2>
struct custom_allocator
{
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  template<typename U> struct rebind { typedef custom_allocator<U> other;};

  custom_allocator();
  ~custom_allocator();

  pointer allocate(std::allocator<void>::const_pointer hint = 0)
  {

  }

  void deallocate(T* p)
  {

  }

  template <typename U, typename... Args>
  void construct (U* p, Args&&... args)
  {

  }

  template <typename U>
  void destroy (U* p)
  {

  }
};

template< class T1, class T2 >
bool operator==( const custom_allocator<T1>& lhs, const custom_allocator<T2>& rhs )
{

}

template< class T1, class T2 >
bool operator!=( const custom_allocator<T1>& lhs, const custom_allocator<T2>& rhs )
{

}

