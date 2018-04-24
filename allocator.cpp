// allocator.cpp in Otus homework #3 project
#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include "./headers/custom_allocator.h"
#include "./headers/custom_map.h"

template <typename Key, typename Value>
using CustomAllocatedMap =
std::map<
          Key, Value, std::less<Key>,
          CustomAllocator<std::pair<const Key, Value>, 100>
        >;

template <typename Key, typename Value>
using CustomAllocatedCustomMap =
CustomMap<
          Key, Value, std::less<Key>,
          CustomAllocator<Node<std::pair<const Key, Value>>, 100>
         >;

uint64_t factorial(uint n)
{
  uint64_t result{1};
  for (uint i{2}; i <= n; i++)
    result *= i;
  return result;
}

int main()
{
  std::map<int, int> map1{};
  for (int idx{}; idx < 10; idx++)
    map1.insert(std::pair<const int, int>{idx, factorial(idx)});

  for (auto item:map1)
    std::cout << item.first << " " << item.second << std::endl;

  CustomAllocatedMap<int, int> map2{};
  for (uint16_t idx{}; idx < 10; idx++)
    map2.insert(std::pair<const int, int>{idx, factorial(idx)});

  for (auto item:map2)
    std::cout << item.first << " " << item.second << std::endl;

  CustomMap<int, int> map3{};
  for (int idx{}; idx < 10; idx++)
    map3.insert(std::pair<const int, int>{idx, factorial(idx)});

  for (auto item:map3)
    std::cout << item.first << " " << item.second << std::endl;

  CustomAllocatedCustomMap<int, int> map4{};
  for (int idx{}; idx < 10; idx++)
    map4.insert(std::pair<const int, int>{idx, factorial(idx)});

  for (auto item:map4)
    std::cout << item.first << " " << item.second << std::endl;

  return 0;
}
